#ifndef TRACKLET_H_
#define TRACKLET_H_

#include "Definitions.h"
#include "Tracking.h"

struct NodeTracklet {
  typedef pair<int, int>  NodePair;
  set<NodePair>           objects_;
  int                     action_ = 0,      //for default 0 is to stay
                          frame_number_;
};

struct Tracklet {

  int     old_  =  0,
          id_   = -1;

  bool    visited_ = true,
          picked_ = false;

  shared_ptr<TrackingBase>  trk_;

  list<NodeTracklet>        nodes_;

  Tracklet(){}

};

#endif // !TRACKLET_H