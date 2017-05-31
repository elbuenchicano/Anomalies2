#include "Support_Basic.h"


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////        TRAIN       ////////////////////////////////////

//..............................................................................
map<string, int> loadActivityList(string & file) {
  map<string, int>  ans;
  ifstream    arc(file);
  string      temp;
  for (int i = 0; !arc.eof(); ++i) {
    getline(arc, temp);
    ans[temp] = i;
  }
  return ans;
}

//..............................................................................
//..............................................................................
inline list<string> loadActivitiesFromFile(string &file) {
  list<string> ans;
  fstream arc(file);
  string  line;
  while (!arc.eof()) {
    getline(arc, line);
    if (line.size() > 5) {
      auto vec = cutil_string_split(line, ',');
      ans.push_back(*vec.rbegin());
    }
  }
  return ans;
}

//..............................................................................
//..............................................................................
void loadActivities(  string            &file, 
                      Ngrams_           &bigram, 
                      Mat_<int>         &histograms,
                      map<string, int>  &keys){
  
  string        prev,
                curr;
  
  Mat_<int> histogram(1, static_cast<int>(keys.size()));
  histogram = histogram * 0;

  //............................................................................
  auto loaded = loadActivitiesFromFile(file);

  //............................................................................
  prev = loaded.front();
  loaded.pop_front();
  ++histogram(0, keys[prev]);
  //++histogram(0, keys[prev]);

  for (auto it = loaded.begin(); it != loaded.end(); ++it) {
    ++histogram(0, keys[*it]);
    //bigram.insertBigram(keys[prev], keys[*it]);
    bigram.insertBigram(prev, *it);
    prev = *it;
  }
  //++bigram.hashObj_[keys[prev]];
  ++bigram.hashObj_[prev];
  histograms.push_back(histogram);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////        TEST       /////////////////////////////////////


//..............................................................................
//..............................................................................
inline  void loadActivitiesFromFile(  string        &file, 
                                      list<string>  &act, 
                                      list<string>  &frames) {
  
  fstream arc(file);
  string  line;
  while (!arc.eof()) {
    getline(arc, line);
    if (line.size() > 2) {
      auto vec = cutil_string_split(line, ',');
      act.push_back(*vec.rbegin());
      frames.push_back(*vec.begin());
    }
  }
}

//..............................................................................
//..............................................................................
//determine if some traclet is anomalous given the colleted information
//tracklet file, ngram and train histograms
void isAnomalous( string            &file, 
                  Ngrams_           &bigram, 
                  map<string, int>  &keys,
                  Mat_<int>         &histo_out,
                  double            prob_thr = 0.1
                  ){

  string        prev, 
                curr;

  double        probability;

  list<string>  activities,
                frames;

  ///this part of the code may write directly in the file, however data is tiny
  stringstream  out_buf;
  //............................................................................
  Mat_<int> histogram(1, static_cast<int>(keys.size()));
  histogram = histogram * 0;

  //............................................................................
  loadActivitiesFromFile(file, activities, frames);

  //............................................................................
  prev = activities.front();
  activities.pop_front();
  ++histogram(0, keys[prev]);
  
  ///0 denotes no anomalous - 1 denotes the existence of an anomalous pattern
  ///must have a coma between frame an anomaly flag
  out_buf << frames.front() << ",0\n";
  frames.pop_front();

  for (auto it = activities.begin(), fr = frames.begin(); 
            it != activities.end(); 
            ++it, ++fr) {
    ++histogram(0, keys[*it]);

    //probability = bigram.probability(keys[prev], keys[*it]); 
    probability = bigram.probability( prev, *it);
    
    out_buf << *fr << ",";
    if (probability < prob_thr)
      out_buf << 1 << "ngram\n";
    else
      out_buf << 0 << endl;

    prev = *it;
  }
  list<string>  out_vec{  cutil_file_path(file),
                          cutil_LastName(file) + ".out"};
  
  auto out_file = cutil_string_fullfile(out_vec);
  cutil_write_2_file ( out_file , out_buf.str());
  cout << "Writing into file: " << out_file << endl;

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//..............................................................................
void loadActivitiesTest(  FileListType      &files, 
                          string            &path,
                          Ngrams_           &bigram,
                          Mat_<int>         &train_histo,
                          double            ngram_thr,
                          map<string, int>  &activities) {
  auto dir_name = cutil_LastName(path);
  for (auto & file : files) {
    isAnomalous(file, bigram, activities, train_histo, ngram_thr);
  }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//- vector string from gt file structure must be frame_ini,frame_fin,label 
//+ timeline vector with marked anomalies 
static inline void mark_time_line( vector<string> & gt, vector<bool> &ans) {
  for (auto line = ++gt.begin(); line != gt.end(); ++line) {
    auto line_s = cutil_string_split(*line, ',');
    for (int i = stoi(line_s[0]); i <= stoi(line_s[1]); ++i)
      ans[i] = true;
  }
}

////////////////////////////////////////////////////////////////////////////////
//..............................................................................
//- vector string from gt file structure must be frame_ini,frame_fin,label 
//+ timeline vector with marked anomalies 
static inline void mark_time_line_out(vector<string> & gt, vector<bool> &ans) {
  
  auto frame_d = stoi(cutil_string_split(gt[1], ',')[0]) -
                 stoi(cutil_string_split(gt[0], ',')[0]);
  
  if( frame_d == 1 ){
    for (auto &line : gt) {
      auto line_s = cutil_string_split(line, ',');
      if(stoi(line_s[1]))
        ans[stoi(line_s[0])] = true;
    }
  }
  else {
    for (auto &line : gt) {
      auto line_s = cutil_string_split(line, ',');
      if (stoi(line_s[1])) {
        auto pos = stoi(line_s[0]);
        for ( int i = 0; 
              (i < frame_d) && ( (pos+i) < static_cast<int>( ans.size()) ); 
              ++i)
          if (stoi(line_s[1]))
            ans[stoi(line_s[1])] = true;
      }
    }
  }

}

////////////////////////////////////////////////////////////////////////////////
//..............................................................................
void single_validation(string &gt_file, string &folder, string &name) {
  int frame_number;
  cutil_file_cont out_list;

  //............................................................................
  auto gt_lines = cutil_load2strv(gt_file);
  
  if (gt_lines.size() < 2) {
    cout << "No anomaly detected in file " << gt_file << endl;
    return;
  }

  frame_number = stoi(gt_lines[0]);
  vector<bool> gt_time_line(frame_number, false);
  vector<bool> out_time_line(frame_number, false);
  mark_time_line(gt_lines, gt_time_line);

  //............................................................................
  list_files(out_list, folder.c_str(), ".out");
  
  for (auto &file : out_list) {
    auto gt_lines_out = cutil_load2strv(file);
    mark_time_line_out(gt_lines_out, out_time_line);
  }

  //............................................................................
  Metric_units metric;

  for (size_t i = 0; i < out_time_line.size(); ++i) {
    if (gt_time_line[i] == out_time_line[i]) {
      //true true = true positive
      if (gt_time_line[i]) ++metric.tp_;
      //false false = true negative
      else ++metric.tn_;
    }
    //different
    else {
      //gt.true pred.false = false negative
      if (gt_time_line[i]) ++metric.fn_;
      //gt.false pred.true = false positive
      else ++metric.fp_;
    }
  }

  //............................................................................
  stringstream ss;
  ss << supp_FalsePositiveRate(metric) << " " << supp_Recall(metric) << endl;
  cutil_write_2_file_a(cutil_string_fullfile({folder, name+".metric"}),ss.str());
  
}





////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//##############################################################################
//ANOMALY GT
AnomalyGt::AnomalyGt(int id, int ini, int fin, int type, string & desc) :
  id_(id),
  ini_(ini),
  fin_(fin),
  type_(type),
  desc_(desc) {
}
////////////////////////////////////////////////////////////////////////////////
bool operator==(const AnomalyGt &an, const AnomalyGt &an2) {
  return (an2.ini_ == an.ini_);
}

////////////////////////////////////////////////////////////////////////////////
bool operator<(const AnomalyGt &an, const AnomalyGt &an2) {
  return (an.ini_ < an2.ini_);
}

////////////////////////////////////////////////////////////////////////////////
AnomalyGt AnomalyGt::operator=(const AnomalyGt &an) {
  ini_ = an.ini_;
  fin_ = an.fin_;
  id_ = an.id_;
  type_ = an.type_;
  desc_ = an.desc_;
  return *this;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void loadGt(string &file, set<AnomalyGt> &anomalies) {
  ifstream arc(file);
  assert(arc.is_open());
  for (string line; getline(arc, line);) {
    auto vline = cutil_string_split(line);
    anomalies.insert(AnomalyGt
    (stoi(vline[0]),
      stoi(vline[1]),
      stoi(vline[2]),
      stoi(vline[3]),
      vline[4]
    )
    );
  }
  //copy(tmp.begin(), tmp.end(), back_inserter(anomalies));
  arc.close();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Function HG from
//ACTION DETECTION USING MULTIPLE SPATIAL - TEMPORAL INTEREST POINT FEATURES 
inline static
int HG(const AnomalyGt &Q_g_i, set<AnomalyGt> &Q_d, double delta) {

  double  left_int,
    right_int,
    A_n_B,
    temp_jac,
    Q_g_i_ = Q_g_i.fin_ - Q_g_i.ini_;

  for (auto Q_d_k = Q_d.begin(); Q_d_k != Q_d.end(); ++Q_d_k) {
    left_int = std::max(Q_d_k->ini_, Q_g_i.ini_);
    right_int = std::min(Q_d_k->fin_, Q_g_i.fin_);
    A_n_B = right_int - left_int;
    temp_jac = A_n_B / Q_g_i_;
    if (temp_jac > 0 && temp_jac > delta) {
      Q_d.erase(Q_d_k);
      return 1;
    }
  }
  return 0;
}

inline static
int TD(const AnomalyGt &Q_d_j, set<AnomalyGt> &Q_g, double delta) {

  double  left_int,
    right_int,
    A_n_B,
    temp_jac,
    Q_d_j_ = Q_d_j.fin_ - Q_d_j.ini_;

  for (auto Q_g_k = Q_g.begin(); Q_g_k != Q_g.end(); ++Q_g_k) {
    left_int = std::max(Q_g_k->ini_, Q_d_j.ini_);
    right_int = std::min(Q_g_k->fin_, Q_d_j.fin_);
    A_n_B = right_int - left_int;
    temp_jac = A_n_B / Q_d_j_;
    if (temp_jac > 0 && temp_jac > delta) {
      Q_g.erase(Q_g_k);
      return 1;
    }
  }
  return 0;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//compute precision recall using technique found in
//ACTION DETECTION USING MULTIPLE SPATIAL - TEMPORAL INTEREST POINT FEATURES 
void computePrecisionRecall(set<AnomalyGt>      Q_g_,
                            set<AnomalyGt>      Q_d,
                            double         &    prec,
                            double         &    recall,
                            double              jac_prec,
                            double              jac_recall) {
  prec = 0;
  recall = 0;


  set<AnomalyGt> Q_g = Q_g_, Q_dp = Q_d, Q_dr = Q_d;
  for (auto &Q_g_i : Q_g)
    prec += HG(Q_g_i, Q_dp, jac_prec);
  prec /= Q_g.size();

  for (auto &Q_d_j : Q_d)
    recall += TD(Q_d_j, Q_g_, jac_recall);
  recall /= Q_dr.size();
}

