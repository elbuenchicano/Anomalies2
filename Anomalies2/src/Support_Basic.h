#ifndef SUPPORT_BASIC_H_
#define SUPPORT_BASIC_H_
#include "Definitions.h"
#include "Sequence.h"
#include "Ngrams.h"
#include "AlgoUtil.h"
///_____________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////        GENERAL DEFINITIONS       //////////////////////////////

typedef Ngrams<string> Ngrams_;

///_____________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//TRAIN

//load activity set
map<string, int> loadActivityList(string &);

//load activitie list from file, file structure must be
//{frame_number [scores] win_label}
//sequence_file, ngram , histograms, activity keys
void loadActivities(string &, Ngrams_ &, Mat_<int> &, map<string, int> &);


///_____________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//TEST

//load activities for test
//-list file
//-parent path
void loadActivitiesTest(  FileListType  &,  
                          string        &, 
                          Ngrams_       &, 
                          Mat_<int>     &,
                          double        ,
                          map<string,int> &);


///_____________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////                 VALIDATION              ////////////////////////

//validates a gt file with tracklet outputs 
//-gt_file path
//-folder path
//-name
void single_validation(string &, string &, string &);

//Anomaly structure
struct AnomalyGt {
  int     id_ = 0,
          ini_ = 0,
          fin_ = 0,
          type_ = 0;
  string  desc_ = "";
  //subject id
  //initial frame
  //final frame
  //anomaly type
  //anomaly description
  AnomalyGt(int, int, int, int, string &);
  AnomalyGt() {};
  AnomalyGt operator =  (const AnomalyGt &);
};
bool operator == (const AnomalyGt &, const AnomalyGt &);
bool operator <  (const AnomalyGt &, const AnomalyGt &);

//load gt file into an ordered vector
void loadGt      (string          &/*file*/,
                  set<AnomalyGt>  &/*anomalies*/);



#endif //