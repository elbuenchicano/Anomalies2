#include "MainControl.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
MainControl::MainControl(string file) :
  main_prop_file_(file) {

  string  activity_file;

  //reading the main file settings

  fs_main_.open(file, FileStorage::READ);
  assert(fs_main_.isOpened());


  fs_main_["main_num_threads"]    >> num_threads_;
  fs_main_["main_activity_file"]  >> activity_file;

  if (!num_threads_) num_threads_ = 1;
  
  activities_ = loadActivityList(activity_file);
}

/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void MainControl::run() {
  short op;
  fs_main_["main_operation"] >> op;
  switch (op)
  {
  case 0: 
    train();
    break;
  case 1:
    test();
    break;
  case 2:
    validation();
    break;
  case 3:
    break;
  case 10:
    break;
  default:
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void MainControl::train( ) {

  string  seq_dir,
          seq_token,
          out_dir;

  Ngrams_   ngram2;
  Mat_<int> histograms;
 
  //............................................................................
  fs_main_["train_seq_dir"]   >> seq_dir;
  fs_main_["train_seq_token"] >> seq_token;
  
  //............................................................................
  cutil_file_cont  file_list;
  list_files_all(file_list, seq_dir.c_str(), seq_token.c_str());
  
  //............................................................................
  for (auto & file : file_list) 
    loadActivities(file, ngram2, histograms, activities_);
  
  string name = cutil_LastName(seq_dir);

  ngram2.save2files(  cutil_string_fullfile(
                        list<string>({ seq_dir , name + "_NGh.txt"})), 
                      cutil_string_fullfile(
                        list<string>({ seq_dir , name + "_NGf.txt"}))
                    );

  FileStorage fs( 
        cutil_string_fullfile(list<string>({ seq_dir , name + "_histos.yml" })), 
        FileStorage::WRITE);

  fs << "data" << histograms;

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void MainControl::test() {

  string  histos,
          ng_histo,
          ng_freq,
          seq_dir,
          seq_token;

  Ngrams_   ngram2;
  Mat_<int> histograms;
  
  double      ngram_thr;
  //............................................................................
  fs_main_["test_histograms"] >> histos;
  fs_main_["test_ng_histo"]   >> ng_histo;
  fs_main_["test_ng_freq"]    >> ng_freq;

  fs_main_["test_seq_dir"]    >> seq_dir;
  fs_main_["test_seq_token"]  >> seq_token;

  fs_main_["test_ngram_thr"]  >> ngram_thr;

  //............................................................................
  ngram2.load_structures( ng_histo, ng_freq );
  FileStorage fs(histos, FileStorage::READ);
  fs["data"] >> histograms;
  
  //............................................................................
  DirectoryNode dir_test(seq_dir, nullptr);
  list_files_all_tree(&dir_test, seq_token.c_str());

  //............................................................................
  
  queue<DirectoryNode*> que;
  que.push(&dir_test);
  while (!que.empty()) {
    auto front = que.front();
    que.pop();
    if (front->_listFile.size())
      loadActivitiesTest( front->_listFile, front->_label, ngram2, 
                          histograms, ngram_thr, activities_);
    for (auto & sons : front->_sons)
      que.push(sons);    
  }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void MainControl::validation() {
  string  gt_token,
          val_dir,
          name;
  
  //............................................................................
  fs_main_["validation_gt_dir"]   >> val_dir;

  //MANDATORY GT MUST HAVE THE SAME NAME AS VIDEO TEST
  fs_main_["validation_gt_token"] >> gt_token;

  //............................................................................

  cutil_file_cont files;
  list_files(files, val_dir.c_str(), gt_token.c_str());
  for (auto &file : files) {
    name = cutil_LastName(file);
    list<string> folder_path{ val_dir, name };
    single_validation(file, cutil_string_fullfile(folder_path), name);
  }


}
