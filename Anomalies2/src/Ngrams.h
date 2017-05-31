#ifndef NGRAMS_H_
#define NGRAMS_H_
#include "CUtil.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <class t>
struct Ngrams {

  typedef std::map<t, double>  mapTI;
  typedef std::map<t, mapTI>   mapTM;

  mapTI   hashObj_;
  mapTM   hashFreq_;

  //............................................................................
  Ngrams() = default;

  void insertBigram(t a, t b) {

    hashFreq_[a][b] = hashFreq_[a].find(b) != hashFreq_[a].end() ?
                      hashFreq_[a][b] + 1 : 1;

    hashObj_[a] = hashObj_.find(a) != hashObj_.end() ? hashObj_[a] + 1 : 1;

    //hashObj_[b] = hashObj_.find(b) != hashObj_.end() ? hashObj_[b] + 1 : 1;
  }

  //using ngram (markov condition) the probability of some bigram appear 
  //P(A|B) = A(AB)/P(P)
  //using maximum likelihood
  //P(A|B) ~ count(AB) / count (B)   
  double probability(t a, t b) {
    if (hashObj_.find(a) == hashObj_.end() || hashObj_.find(b) == hashObj_.end())
      return 0;
    return hashFreq_[a][b] / hashObj_[b];
  }
  //============================================================================
  //histogram out
  //freq      out
  void save2files(std::string &hist, std::string &freq) {
    std::ofstream of1(hist);
    for (auto it = hashObj_.begin(); it != hashObj_.end(); ++it)
      of1 << it->first << "," << it->second << std::endl;
    of1.close();
    std::cout << "Saving hist in: " << hist << std::endl;
    //..........................................................................
    std::ofstream of2(freq);
    for (auto it = hashFreq_.begin(); it != hashFreq_.end(); ++it) {
      for (auto iti = it->second.begin(); iti != it->second.end(); ++iti)
        of2 << it->first << "," << iti->first << "," << iti->second << std::endl;
    }
    std::cout << "Saving freq in: " << freq << std::endl;
    of2.close();
  }
  //============================================================================
  //histogram out
  //freq      out
  void load_structures(std::string &hist, std::string &freq) {
    std::cout << "Loading hist from: " << hist << std::endl;
    std::ifstream of1(hist);
    for (string line; getline(of1, line);) {
      auto vline = cutil_string_split(line, ',');
      if (vline.size() > 1)
        hashObj_[vline[0]] = stoi(vline[1]);
    }
    of1.close();

    //..........................................................................
    std::ifstream of2(freq);
    std::cout << "Loading frequency table from: " << freq << std::endl;
    for (string line; getline(of2, line);) {
      auto vline = cutil_string_split(line, ',');
      if (vline.size() > 1) {
        auto a = vline[0];
        auto b = vline[1];
        hashFreq_[a][b] = stoi(vline[2]);
      }
    }
    of2.close();
  }
  //============================================================================
  //ng      ngrams model to append
  void append(Ngrams & ng) {
    for (auto & a : ng.hashFreq_) {
      for(auto & b : a.second){
        
      }
    }
  }
};

//============================================================================
//histogram out
//freq      out
void ngram_load_structures(std::string &, std::string &, Ngrams<int> & ng);

#endif