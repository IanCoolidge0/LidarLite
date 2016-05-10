/*
 * LidarHandler.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: Ian
 */

#include "LidarHandler.h"

/**
 * All distances in class are in inches
 */
LidarHandler::LidarHandler(Relay * onSwitch, uint32_t lidarPort):
   m_onSwitch(onSwitch),
   m_counter(lidarPort){
   m_onSwitch->Set(Relay::kOn);
   m_counter.SetSemiPeriodMode(true);
   m_resetCount = 0;
   m_storedCounter = 0;
   m_distance = 0;
   m_fastAverage = 0;
   m_mediumAverage = 0;
   m_slowAverage = 0;

   m_offset = 0;

   for(int i=0;i<100;i++)
      m_storedDistances[i] = 0;
}

double LidarHandler::getFastAverage() {
   return m_fastAverage;
}


double LidarHandler::getMediumAverageDEPRECATED() {
   return m_mediumAverage;
}

double LidarHandler::getSlowAverage() {
   return m_slowAverage;
}

void LidarHandler::run() {
    double distance = m_counter.GetPeriod() * 100000.0/2.54  - m_offset;

    //this will be true if distance is inf or NaN (i.e. the lidar has crashed)
    bool infDistance = distance - distance != 0;

    if(infDistance) {
       m_resetCount++;
       m_onSwitch->Set(Relay::kOff);
       Wait(0.3);
       m_onSwitch->Set(Relay::kOn);
    } else {
       m_distance = distance;

       m_storedDistances[m_storedCounter % 100] = distance;
       m_storedCounter++;
    }
}

double LidarHandler::getDistance() {
   return m_distance;
}

int LidarHandler::getResetCount() {
   return m_resetCount;
}

LidarHandler::~LidarHandler() {

}

