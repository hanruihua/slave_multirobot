#include "../NProtocol/nframeextraction.h"
#include "../serial/initserial.h"
#include "init.h"
#include <ros/ros.h>
#include <ros/time.h>
#include <ros/timer.h>

int main(int argc, char **argv) {

  ros::init(argc, argv, "TOFSense_Processor");
  ros::NodeHandle nodeHandle;
  serial::Serial serial;
  initSerial(argc, argv, &serial);

  NFrameExtraction frameExtraction;
  TOFSense::Init tofsenseInit(&frameExtraction, &serial);

  while (ros::ok()) {
    auto availableBytes = serial.available();
    std::string strReceived;
    if (availableBytes) {
      serial.read(strReceived, availableBytes);
      frameExtraction.unpackData(strReceived);
    }
    ros::spinOnce();
  }
  return 1;
}

//  ros::Rate loopRate(1000);
//  while (ros::ok()) {
//    auto availableBytes = serial.available();
//    std::string strReceived;
//    if (availableBytes) {
//      serial.read(strReceived, availableBytes);
//      frameExtraction.unpackData(strReceived);
//    }
// ros::spinOnce();
//    loopRate.sleep();
//  }
