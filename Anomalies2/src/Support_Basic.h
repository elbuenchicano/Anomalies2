#ifndef SUPPORT_BASIC_H_
#define SUPPORT_BASIC_H_
#include "Definitions.h"
#include "Tracklet.h"
#include "Sequence.h"

///load objects from file
//in : file
//out: map and reverse map
void loadObjects(std::string, std::map<std::string, int> &, std::map<int, std::string> &);

//it transform int set to a single string
string set2str(set<int> &);

//this fucntion builds tracklets and theirs structure
//list of files 
//string out, source
//double resize image, distance to object, distance to subject, jaccard
//int time life
void interaction_structure_building(  cutil_file_cont &, string &, string &,
                                      double, double, double, double, int,
                                      map<string, int> &, map<int, string> );
  
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///scripting

void script_video( string & );

#endif //