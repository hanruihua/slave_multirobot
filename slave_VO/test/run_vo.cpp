// -------------- test the visual odometry -------------

#include "slaveslam/run_vo.h"

Mat color;
Mat depth;
std_msgs::Header head;
double stemp1 =0.0;
double stemp2 = 0.0;
double stemp = 0.0;
Eigen::Vector3d initial_position(0,0,0);
int flag = 0;

void depthCallback(const sensor_msgs::Image::ConstPtr& msg )
{
    //ROS_INFO("depth"+stemp1);
    cv_bridge::CvImageConstPtr cv_ptr;
    cv_ptr = cv_bridge::toCvCopy(msg);
    depth = cv_ptr->image;
    stemp1 = msg->header.stamp.toSec();
   // std::cout<<"depth"<<stemp1<<std::endl;
   // Mat color = cv::imread(msg->data());
}

void colorCallback(const sensor_msgs::Image::ConstPtr& msg )
{
    //ROS_INFO("receive color image");
    cv_bridge::CvImageConstPtr cv_ptr;
    cv_ptr = cv_bridge::toCvShare(msg);
    color = cv_ptr->image;
    head = msg->header;
    stemp2 = msg->header.stamp.toSec();
    //std::cout<<"color"<<stemp2<<std::endl;
}

void callback(const sensor_msgs::ImageConstPtr& depth_msg, const sensor_msgs::ImageConstPtr& color_msg)
{
    ROS_INFO("receive image");
    cv_bridge::CvImageConstPtr color_ptr;
    cv_bridge::CvImageConstPtr depth_ptr;
    color_ptr = cv_bridge::toCvCopy(color_msg);
    depth_ptr = cv_bridge::toCvCopy(depth_msg);

    color = color_ptr->image;
    depth = depth_ptr->image;

//    cv::imshow("image", color );
//    cv::imshow("image2", depth );
//    cv::waitKey(1);

    head = color_ptr->header;
    stemp1 = color_ptr->header.stamp.toSec();
    stemp2 = depth_ptr->header.stamp.toSec();
}

void slave1_callback(geometry_msgs::Pose2D msgIn1)
{
    if (flag == 0)
    {
        ROS_INFO("Place the initial position from mocap");
        initial_position(0) = msgIn1.x;
        initial_position(1) = msgIn1.y;
        flag = 1;
    }
}



int main ( int argc, char** argv )
{
    //ros
    ros::init(argc, argv, "slavevo");
    ros::NodeHandle n;

    ros::Publisher vo_pub = n.advertise<geometry_msgs::PointStamped>("vo_position", 100);
   // ros::Subscriber mocap_sub = n.subscribe("/slave1/ground_pose", 100, slave1_callback);

    message_filters::Subscriber<sensor_msgs::Image> depth_sub(n, "/camera/depth_registered/image", 1);
    message_filters::Subscriber<sensor_msgs::Image> color_sub(n, "/camera/rgb/image_color", 1);


   // message_filters::TimeSynchronizer<sensor_msgs::Image, sensor_msgs::Image> sync(depth_sub, color_sub, 10);
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> MySyncPolicy;

    message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(100), depth_sub, color_sub);
    sync.registerCallback(boost::bind(&callback, _1, _2));

//    ros::Subscriber depth_sub = n.subscribe("/camera/depth/image", 100, depthCallback);
//    ros::Subscriber color_sub = n.subscribe("/camera/rgb/image_color", 100, colorCallback);
  //  ros::Rate loop_rate(10);

    ros::Time begin = ros::Time::now();

    //eigen


    Eigen::Vector3d current_position(0,0,0);
    Eigen::Affine3d T = Eigen::Affine3d::Identity();
    Eigen::Matrix3d rotation;
    Eigen::Vector3d trans;

    geometry_msgs::PointStamped position;

    if ( argc != 2 )
    {
        cout<<"usage: run_vo parameter_file"<<endl;
        return 1;
    }

    slaveslam::Config::setParameterFile ( argv[1] );
    slaveslam::VisualOdometry::Ptr vo ( new slaveslam::VisualOdometry );
    slaveslam::Camera::Ptr camera ( new slaveslam::Camera );
    
    // visualization
    cv::viz::Viz3d vis("Visual Odometry");
    cv::viz::WCoordinateSystem world_coor(1.0), camera_coor(0.5);
    cv::Point3d cam_pos( 0, -1.0, -1.0 ), cam_focal_point(0,0,0), cam_y_dir(0,1,0);
    cv::Affine3d cam_pose = cv::viz::makeCameraPose( cam_pos, cam_focal_point, cam_y_dir );
    vis.setViewerPose( cam_pose );
    
    world_coor.setRenderingProperty(cv::viz::LINE_WIDTH, 2.0);
    camera_coor.setRenderingProperty(cv::viz::LINE_WIDTH, 1.0);
    vis.showWidget( "World", world_coor );
    vis.showWidget( "Camera", camera_coor );
    ros::Rate r(5);
    while (ros::ok())
    {
//        for ( int i=0; i<rgb_files.size(); i++ )
//        {
//            Mat color = cv::imread ( rgb_files[i] );
//            Mat depth = cv::imread ( depth_files[i], -1 );
        if ( color.data==nullptr || depth.data==nullptr )
                ROS_INFO("There are not any image");
        else
//        {
//            double diff = stemp1 - stemp2;
//            std::cout<<"diff"<<diff<<std::endl;
//        }
        {
            slaveslam::Frame::Ptr pFrame = slaveslam::Frame::createFrame();
            pFrame->camera_ = camera;
            pFrame->color_ = color;
            pFrame->depth_ = depth;
            pFrame->time_stamp_ = stemp1;
//
//            boost::timer timer;

            vo->addFrame ( pFrame );
//           // cout<<"VO costs time: "<<timer.elapsed()<<endl;
//
         if ( vo->state_ == slaveslam::VisualOdometry::LOST )
         {
             initial_position = current_position;
             vo->state_ = slaveslam::VisualOdometry::INITIALIZING;
         }

            SE3 Tcw = pFrame->T_c_w_.inverse();
//
//            // show the map and the camera pose
            cv::Affine3d M(
                    cv::Affine3d::Mat3(
                            Tcw.rotation_matrix()(0,0), Tcw.rotation_matrix()(0,1), Tcw.rotation_matrix()(0,2),
                            Tcw.rotation_matrix()(1,0), Tcw.rotation_matrix()(1,1), Tcw.rotation_matrix()(1,2),
                            Tcw.rotation_matrix()(2,0), Tcw.rotation_matrix()(2,1), Tcw.rotation_matrix()(2,2)
                    ),
                    cv::Affine3d::Vec3(
                            Tcw.translation()(0,0), Tcw.translation()(1,0), Tcw.translation()(2,0)
                    )
            );

            cv::cv2eigen(M.rotation(), rotation);
            cv::cv2eigen(M.translation(), trans);
            T.rotate(rotation);
            T.pretranslate(trans);
            current_position = rotation * initial_position + trans;
//           // initial_position = current_position;
//            //std::cout << current_position<< std::endl;
//
            position.point.x = current_position(0);
            position.point.y = current_position(1);
            position.point.z = current_position(2);

            position.header = head;

            vis.setWidgetPose( "Camera", M);
            vis.spinOnce(1, false);
            
            vo_pub.publish(position);
//          ros::spinOnce();

        }
        ros::spinOnce();
        r.sleep();

//            seq++;
//            //cv::imshow("image", color );
//            cv::waitKey(1);

//

    }

    return 0;
}
