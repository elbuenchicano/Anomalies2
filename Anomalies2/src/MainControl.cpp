#include "MainControl.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
MainControl::MainControl(string file) :
  main_prop_file_(file) {

  string main_object_file;

  //reading the main file settings

  fs_main_.open(file, FileStorage::READ);
  assert(fs_main_.isOpened());
  
  fs_main_["main_object_file"]  >> main_object_file;
  fs_main_["main_num_threads"]  >> num_threads_;

  if (!num_threads_) num_threads_ = 1;
  
  //loading objects
  loadObjects(main_object_file, objects_, objects_rev_);
  cout << "Reading: " << file << endl;
}

/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void MainControl::run() {
  short op;
  fs_main_["main_operation"] >> op;
  switch (op)
  {
  case 0:
    scripting();
  case 1:
    //graphBuilding();
    break;
  case 2:
    //train();
    break;
  case 3:
    //test();
    break;
  case 10:
    //show();
    break;
  default:
    break;
  }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void MainControl::structureBuiling( ) {

  double  distance_obj_thr,
          distance_sub_thr;

  float   rze;

  int     time_life,
          frame_step;

  string  out_dir,
          seq_dir,
          seq_token,
          out_token,
          video_file;

  double  jaccard;
  
  //............................................................................
  fs_main_["structure_building_seq_dir"]          >> seq_dir;
  fs_main_["structure_building_seq_token"]        >> seq_token;
  fs_main_["structure_building_distance_sub_thr"] >> distance_sub_thr;
  fs_main_["structure_building_distance_obj_thr"] >> distance_obj_thr;
  fs_main_["structure_building_time_life"]        >> time_life;
  fs_main_["structure_building_out_dir"]          >> out_dir;
  fs_main_["structure_building_out_token"]        >> out_token;
  fs_main_["structure_building_jaccard"]          >> jaccard;
  fs_main_["structure_building_step"]             >> frame_step;
  
  //............................................................................
  cutil_file_cont  file_list;
  list_files_all(file_list, seq_dir.c_str(), seq_token.c_str());
  auto list_group = cutil_cont_split(file_list, num_threads_);
    
  for (auto & thr : list_group) {
         
  }

  //graphBuild( seq_file, video_file, str, rze, distance_obj_thr, distance_sub_thr,
  //            time_life, jaccard);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void MainControl::scripting() {

  string  file;
  int     cmd;

  //............................................................................
  fs_main_["scripting_file"] >> file;
  fs_main_["scripting_cmd"]  >> cmd;

  //............................................................................
  typedef void (*pf) (string&);
  vector<pf> functions{script_video};

  if (file != "" && cmd < functions.size())
    functions[cmd](file);
  else
    cout << "\n!error in file or command!";
}

