/*
 * Association.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: frm-ag
 */

#include <cob_leg_detection/association/association.h>
#include <cob_leg_detection/leg_feature.h>
#include <cob_leg_detection/detection/detection.h>

Association::Association(LegFeaturePtr leg, DetectionPtr detection, double assocation_probability) {
  this->leg_ = leg;
  this->detection_ = detection;
  this->association_probability_ = assocation_probability;
}

Association::~Association() {
  // TODO Auto-generated destructor stub
}

