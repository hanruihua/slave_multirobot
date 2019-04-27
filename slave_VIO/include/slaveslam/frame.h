#ifndef FRAME_H
#define FRAME_H

#include "slaveslam/camera.h"
#include "slaveslam/common_include.h"

class Frame
{
    public:
        typedef std::shared_ptr<Frame> Ptr;
        unsigned long id_;
        double time_stamp_;
        SE3 T_c_w;
        Camera::Ptr camera_;
        Mat color_ , depth_;

    public:
        Frame();
        Frame(long id, double time_stamp = 0, SE3 T_c_w = SE3(), Camera::Ptr camera = nullptr, Mat color = Mat(), Mat depth = Mat());
        ~Frame();

        //factory function
        static Frame::Ptr createFrame();

        //find the depth in depth map
        double findDepth(const cv::KetPoint& kp);

        //Get Camera Center
        Vector3d getCamCenter() const;

        //check if a point is in this frame
        bool isInFrame(const Vector3d& pt_world);
};



#endif