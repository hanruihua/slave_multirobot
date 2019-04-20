#ifndef CAMERA_H
#define CAMERA_H
#include "slaveslam/common_include.h"

namespace slaveslam
{
    class camera
    {
       public:
       typedef std::shared_ptr<camera> Ptr;
       float fx_, fy_, cx_, cy_, depth_scale;
       
       Camera();
       Camera( float fx, float fy, float cx, float cy, float depth_scale = 0):
       fx_ (fx), fy_(fy), cx_(cx), cy_(cy), depth_scale_(depth_scale) {}

       //coordinate transform: world, camera, pixel
       Vector3d world2camera(const Vector3d& p_w, const SE3& T_c_w);
       
    }


}