#include "Support_Basic.h"

////////////////////////////////////////////////////////////////////////////////
void loadObjects(std::string file, std::map<std::string, int> &info,
  std::map<int, std::string> &infor) {
  std::ifstream arc(file);
  for (std::string line; std::getline(arc, line); ) {
    auto vline = cutil_string_split(line);
    if (vline.size() > 1) {
      auto name = cutil_string_join(vline, 1);
      int id = stoi(vline[vline.size() - 1]);

      info[name] = id;
      infor[id] = name;
    }
  }
  arc.close();
}

////////////////////////////////////////////////////////////////////////////////
string set2str(set<int> & st) {
  stringstream ss;
  for (auto & it : st)
    ss << it << " ";
  return ss.str().substr(0, ss.str().size() - 1);
}

////////////////////////////////////////////////////////////////////////////////
void interaction_structure_building(  cutil_file_cont   & file_list,
                                      string &out,      string &src,  
                                      double rze, 
                                      double dist_obj,  double dist_sub, 
                                      double jaccard,   int    life,
                                      map<string, int>  & objs,
                                      map<int, string>  & objs_r){
  
  string  video_file,
          video_seq;

  short   frame_step;

  FileStorage fs(src, FileStorage::READ);

  //............................................................................
  fs["scripting_file"]  >> video_file;
  fs["scripting_cmd"]   >> video_seq;
  fs["scripting_step"]  >> frame_step;

  //............................................................................
  Sequence seq(frame_step, video_seq, video_file, rze, objs, objs_r);



}

////////////////////////////////////////////////////////////////////////////////


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///scripting

void script_video( string &cmd ) {
  string  dir,
          token,
          out_dir,
          out_token = ".yml";

  FileStorage fs(cmd, FileStorage::READ);
  //............................................................................
  fs["scripting_0_dir"]       >> dir;
  fs["scripting_0_token"]     >> token;
  fs["scripting_0_out_dir"]   >> out_dir;

  //............................................................................
  cutil_file_cont file_list;
  list_files_all(file_list, dir.c_str(), token.c_str());

  if (out_dir == "") out_dir = dir + "/";
  
  for (auto &file : file_list) {
    stringstream ss;
    auto str = out_dir + cutil_LastName(file) + out_token;  
    
    ss << "%YAML:1.0\n";
    ss << "video_file: \""  << file << "\"" <<endl;
    ss << "seq_file: \""    << file.substr(0, file.size() - 4) <<".txt" << "\"" << endl;

    cout << "Writting: "<<str << endl;
    ofstream arc(str);
    arc << ss.str();
    arc.close();
  }

}