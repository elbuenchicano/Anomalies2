#ifndef MAIN_CONTROL_H_
#define MAIN_CONTROL_H_

#include "Support_Basic.h"

class MainControl {
  //variables...................................................................
  string            main_prop_file_;

  FileStorage       fs_main_;

  int               num_threads_;

  map<string, int>  activities_;

  //functions................................................................... 
  
  //train model 
  void train        ( );

  //test model
  void test         ( );

  //validating output test
  void validation   ( );

  
public:
  MainControl ( string );
  void run();
};



#endif //MAIN_CONTROL_H_