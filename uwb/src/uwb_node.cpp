#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <ros/ros.h>
#include <iostream>
#include "data_typedef.h"
#include <geometry_msgs/Point.h>
#include <uwb/uwb_position.h>
#include <string.h>



std::string id;

template<typename T> std::string toString(const T& t){
    std::ostringstream oss;  //创建一个格式化输出流
    oss<<t;             //把值传递如流中
    return oss.str();
}

int main (int argc, char** argv){
    ros::init(argc, argv, "uwb_node");
    ros::NodeHandle n;
    //  ros::Subscriber ground_truth = n.subscribe("robot01", 1000, callback);     //subscribe to mocap
    ros::Publisher uwb_pub = n.advertise<uwb::uwb_position>("uwb_position", 1000);

    //initialize
    uwb::uwb_position uwb_out;
    serial::Serial ros_ser;
    uint8 data_frame[128];



    try
    {
        ros_ser.setPort("/dev/ttyUSB0");
        ros_ser.setBaudrate(921600);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ros_ser.setStopbits (serial::stopbits_one);
        ros_ser.setTimeout(to);
        ros_ser.open();
    }
    catch (serial::IOException& e)
    {
        //ros_ser.setPort("/dev/ttyUSB1");
	    ROS_ERROR_STREAM("Unable to open port ");
        ROS_ERROR_STREAM(e.getErrorNumber());
        return -1;
    }
    if(ros_ser.isOpen()){
        ROS_INFO_STREAM("Serial Port opened");
    }else{
        return -1;
    }

    ros::Rate loop_rate(50);  // Attention!!  Make sure this value is according with serial rate
    while(ros::ok())
    {
        uint8 data_frame_sum = 0;
        if(ros_ser.available())
        {
            ros_ser.read(data_frame, 128);
        }

        //ROS_INFO_STREAM("Reading from serial port");
        if((data_frame[0] == 0x55) && (data_frame[1] == 0x01))
        {
            //ROS_INFO_STREAM("Read successfully");
            for (int i = 0; i < (128 - 1); i++)
            {
                data_frame_sum += data_frame[i];
            }

            if (data_frame_sum == data_frame[128 - 1])
            {
                ROS_INFO_STREAM("valid frame");
                //ROS_INFO_STREAM("id:" + data_frame[2]);
                uwb_out.id = toString(data_frame[2]);
                uwb_out.position.x = (float) Byte32(sint32, data_frame[6], data_frame[5], data_frame[4], 0) / 256000.0f;
                uwb_out.position.y = (float) Byte32(sint32, data_frame[9], data_frame[8], data_frame[7], 0) / 256000.0f;
                uwb_out.position.z = (float) Byte32(sint32, data_frame[12], data_frame[11], data_frame[10], 0) / 256000.0f;
                uwb_pub.publish(uwb_out);
                //ros::spinOnce();
                //ROS_INFO_STREAM("position x:"  pos.x  "position y:"  pos.y  "position z:"  pos.z);
                 //  std::cout << "position x: " << uwb_out.position << " ";
                // std::cout << "position y: " << pos.y << " ";
                // std::cout << "position z: " << pos.z << " ";
                //  std::cout << std::endl;

                // i_uwb_lps_tag.id = data_frame[2];

                // i_uwb_lps_tag.position.x = (float)Byte32(sint32 ,data_frame[6],data_frame[5],data_frame[4], 0) / 256000.0f;
                // i_uwb_lps_tag.position.y = (float)Byte32(sint32 ,data_frame[9],data_frame[8],data_frame[7], 0) / 256000.0f;
                // i_uwb_lps_tag.position.z = (float)Byte32(sint32 ,data_frame[12],data_frame[11],data_frame[10], 0) / 256000.0f;

                // i_uwb_lps_tag.velocity.x = (float)Byte32(sint32 ,data_frame[15],data_frame[14],data_frame[13], 0) / 2560000.0f;
                // i_uwb_lps_tag.velocity.y = (float)Byte32(sint32 ,data_frame[18],data_frame[17],data_frame[16], 0) / 2560000.0f;
                // i_uwb_lps_tag.velocity.z = (float)Byte32(sint32 ,data_frame[21],data_frame[20],data_frame[19], 0) / 2560000.0f;

                // i_uwb_lps_tag.dis_buf[0] = (float)Byte32(sint32 ,data_frame[24],data_frame[23],data_frame[22], 0) / 256000.0f;
                // i_uwb_lps_tag.dis_buf[1] = (float)Byte32(sint32 ,data_frame[27],data_frame[26],data_frame[25], 0) / 256000.0f;
                // i_uwb_lps_tag.dis_buf[2] = (float)Byte32(sint32 ,data_frame[30],data_frame[29],data_frame[28], 0) / 256000.0f;
                // i_uwb_lps_tag.dis_buf[3] = (float)Byte32(sint32 ,data_frame[33],data_frame[32],data_frame[31], 0) / 256000.0f;
                // i_uwb_lps_tag.dis_buf[4] = (float)Byte32(sint32 ,data_frame[36],data_frame[35],data_frame[34], 0) / 256000.0f;
                // i_uwb_lps_tag.dis_buf[5] = (float)Byte32(sint32 ,data_frame[39],data_frame[38],data_frame[37], 0) / 256000.0f;
                // i_uwb_lps_tag.dis_buf[6] = (float)Byte32(sint32 ,data_frame[42],data_frame[41],data_frame[40], 0) / 256000.0f;
                // i_uwb_lps_tag.dis_buf[7] = (float)Byte32(sint32 ,data_frame[45],data_frame[44],data_frame[43], 0) / 256000.0f;

                // buf[0] = data_frame[46];buf[1] = data_frame[47];buf[2] = data_frame[48];buf[3] = data_frame[49];
                // Byte_to_Float(&i_uwb_lps_tag.gyro.x,buf);

                // buf[0] = data_frame[50];buf[1] = data_frame[51];buf[2] = data_frame[52];buf[3] = data_frame[53];
                // Byte_to_Float(&i_uwb_lps_tag.gyro.y,buf);

                // buf[0] = data_frame[54];buf[1] = data_frame[55];buf[2] = data_frame[56];buf[3] = data_frame[57];
                // Byte_to_Float(&i_uwb_lps_tag.gyro.z,buf);

                // buf[0] = data_frame[58];buf[1] = data_frame[59];buf[2] = data_frame[60];buf[3] = data_frame[61];
                // Byte_to_Float(&i_uwb_lps_tag.acc.x,buf);

                // buf[0] = data_frame[62];buf[1] = data_frame[63];buf[2] = data_frame[64];buf[3] = data_frame[65];
                // Byte_to_Float(&i_uwb_lps_tag.acc.y,buf);

                // buf[0] = data_frame[66];buf[1] = data_frame[67];buf[2] = data_frame[68];buf[3] = data_frame[69];
                // Byte_to_Float(&i_uwb_lps_tag.acc.z,buf);

                // i_uwb_lps_tag.angle.x = (float) (Byte16(sint16, data_frame[83],  data_frame[82])) / 100.0f;
                // i_uwb_lps_tag.angle.y = (float) (Byte16(sint16, data_frame[85],  data_frame[84])) / 100.0f;
                // i_uwb_lps_tag.angle.z = (float) (Byte16(sint16, data_frame[87],  data_frame[86])) / 100.0f;


                // buf[0] = data_frame[88];buf[1] = data_frame[89];buf[2] = data_frame[90];buf[3] = data_frame[91];
                // Byte_to_Float(&i_uwb_lps_tag.Q.q0,buf);

                // buf[0] = data_frame[92];buf[1] = data_frame[93];buf[2] = data_frame[94];buf[3] = data_frame[95];
                // Byte_to_Float(&i_uwb_lps_tag.Q.q1,buf);

                // buf[0] = data_frame[96];buf[1] = data_frame[97];buf[2] = data_frame[98];buf[3] = data_frame[99];
                // Byte_to_Float(&i_uwb_lps_tag.Q.q2,buf);

                // buf[0] = data_frame[100];buf[1] = data_frame[101];buf[2] = data_frame[102];buf[3] = data_frame[103];
                // Byte_to_Float(&i_uwb_lps_tag.Q.q3,buf);

                // i_uwb_lps_tag.system_time = Byte32(uint32,data_frame[115], data_frame[114], data_frame[113],  data_frame[112]);

                // i_uwb_lps_tag.sensor_status = data_frame[116];

            }
            // for(int i=0; i<n; i++)
            // {
            //     //16进制的方式打印到屏幕
            //     std::cout << std::hex << (buffer[i] & 0xff) << " ";
            // }
            // std::cout << std::endl;

            //std_msgs::String serial_data;
            // //获取串口数据
            // serial_data.data = ros_ser.read(n);
            // ROS_INFO_STREAM("Read: " << serial_data.data);
            //将串口数据发布到主题sensor


        }
        loop_rate.sleep();
    }
}
