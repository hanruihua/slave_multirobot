#ifndef TOFSENSEINIT_H
#define TOFSENSEINIT_H

#include "../NLink/TOFSense/nframe0.h"
#include "../NProtocol/nprotocolbase.h"
#include "serial/serial.h"
#include <map>
#include <nlink/tofsense_cascade.h>
#include <nlink/tofsense_frame0.h>
#include <ros/ros.h>
#include <unordered_map>

class NFrameExtraction;

namespace TOFSense {

class Init {
public:
  explicit Init(NFrameExtraction *frameExtraction, serial::Serial *serial);
  //  ~Init();

private:
  void initFrame0(NFrameExtraction *frameExtraction);

  void test(NFrameExtraction *frameExtraction);

  NFrame0 *protocolFrame0_;

  std::unordered_map<NProtocolBase *, ros::Publisher> publishers_;

  std::map<int, nlink::tofsense_frame0> frame0Map_;

  serial::Serial *serial_;

  /** 根据实际情况自行配置参数 **/
  const int frequency_ = 10;        //查询频率,最大10Hz
  const size_t nodesCount_ = 8;     //级联节点数
  const bool isInquireMode_ = true; //主动查询模式
  /** 根据实际情况自行配置参数 **/

  ros::NodeHandle nh;
  ros::Timer timerScan_;
  ros::Timer timerRead_;
  int nodesReaded_ = 0;
};

} // namespace TOFSense
#endif // TOFSENSEINIT_H
