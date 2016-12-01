#ifndef MAIN_CONTROL_H_
#define MAIN_CONTROL_H_

#include "Support_Basic.h"

class MainControl {
  //variables...................................................................
  string            main_prop_file_;

  FileStorage       fs_main_;

  map<string, int>  objects_;

  map<int, string>  objects_rev_;

  int               num_threads_;

  //functions................................................................... 
  
  //creates a script for each video file and sequence, it presumes that video
  //has the same name of video file
  void scripting ( );
  
  //construct the interaction structure
  void structureBuiling ( );



public:
  MainControl ( string );
  void run();
  
};



#endif //MAIN_CONTROL_H_