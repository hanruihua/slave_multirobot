#include "init.h"

#include "../NLink/TOFSense/nreadframe0.h"
#include "../NProtocol/ncommon.h"
#include "../NProtocol/nframeextraction.h"

namespace TOFSense {

Init::Init(NFrameExtraction *frameExtraction, serial::Serial *serial)
    : serial_(serial) {
  initFrame0(frameExtraction);

  //  test(frameExtraction);
}

void Init::initFrame0(NFrameExtraction *frameExtraction) {
  protocolFrame0_ = new NFrame0;
  frameExtraction->appendProtocol(protocolFrame0_);
  protocolFrame0_->setDataHandle([=] {
    if (!publishers_[protocolFrame0_]) {
      ros::NodeHandle nodeHandle;
      if (isInquireMode_) {
        publishers_[protocolFrame0_] =
            nodeHandle.advertise<nlink::tofsense_cascade>(
                "nlink_tofsense_cascade", 50);
      } else {
        publishers_[protocolFrame0_] =
            nodeHandle.advertise<nlink::tofsense_frame0>(
                "nlink_tofsense_frame0", 50);
      }
    }
    auto data = protocolFrame0_->data();
    nlink::tofsense_frame0 frame0MsgData;

    frame0MsgData.id = data.id;
    frame0MsgData.systemTime = data.systemTime;
    frame0MsgData.distance = data.distance();
    frame0MsgData.distanceStatus = data.distanceStatus;
    frame0MsgData.signalStrength = data.signalStrength;

    if (isInquireMode_) {
      frame0Map_[data.id] = frame0MsgData;

    } else {
      publishers_.at(protocolFrame0_).publish(frame0MsgData);
    }
  });

  if (isInquireMode_) {

    timerScan_ = nh.createTimer(ros::Duration(1.0 / frequency_),
                                [=](const ros::TimerEvent &) {
                                  frame0Map_.clear();
                                  nodesReaded_ = 0;
                                  timerRead_.start();
                                },
                                false, true);
    timerRead_ =
        nh.createTimer(ros::Duration(0.006),
                       [=](const ros::TimerEvent &) {
                         if (nodesReaded_ >= nodesCount_) {
                           nlink::tofsense_cascade msgCascade;
                           for (const auto &msg : frame0Map_) {
                             msgCascade.nodes.push_back(msg.second);
                           }
                           publishers_.at(protocolFrame0_).publish(msgCascade);
                           timerRead_.stop();
                         } else {
                           serial_->write(TOFSense::commandRead(nodesReaded_));
                           ++nodesReaded_;
                         }
                       },
                       false, false);
  }
}

void Init::test(NFrameExtraction *frameExtraction) {
  frameExtraction->unpackData(
      NCommon::StringToHex("57 00 ff 00 9e 8f 00 00 ad 08 00 00 03 00 ff 3a"));
}

} // namespace TOFSense
