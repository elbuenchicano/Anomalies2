// Anomalies2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src\MainControl.h"

int main(int arg, char **argv){
 
  string tmp = "z:/tmp/anomalies/sibgrapi/main.yml";

  string file = arg < 2 ?  tmp : argv[1];

  MainControl mc(file);
  mc.run();
    
  return 0;
}

