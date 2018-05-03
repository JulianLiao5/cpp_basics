
/*  test demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>  /* For htonl and ntohl */
#include <unistd.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <functional>
#include <iomanip>
#include <iterator>

#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include <nanomsg/reqrep.h>

#include <Eigen/Dense>

#include <opencv2/viz/vizcore.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/eigen.hpp>
#include <opencv2/core/affine.hpp>

#include "msg_utils/pi_msg_adaptor.h"

#include "msckf_vio/vio_com.h"
#include "msckf_vio/math_utils.hpp"

using namespace std;
using namespace Eigen;
using namespace cv;
using namespace cv::viz;

using namespace PIAUTO::msg;
using namespace msckf_vio;

static std::vector<cv::Affine3d> path;

int processVIOPos(int handler, p_pi_msg_envelope_t p_env, const char* body, unsigned int len)
{
    std::cout << "one vio pos recieved" << std::endl;
    if(p_env->type == PIMSG_LOCALIZATION_VIO_PUBLISH_POS)
    {
        PILocalizationVioMsg msg;
        memcpy(&msg, body, sizeof(PILocalizationVioMsg));

        Eigen::Matrix3d rot = quaternionToRotation({msg.qx, msg.qy, msg.qz, msg.qw});

        cv::Affine3d::Mat3 rot_cv;
        eigen2cv(rot, rot_cv);

        cv::Vec3d pos = {msg.tx, msg.ty, msg.tz};
        path.push_back(cv::Affine3d::Identity().rotate(rot_cv).translate(pos));

        std::cout << "one pos: " << std::setprecision(7) << msg.tx << " "<< msg.ty << " " << msg.tz << " "
                    << msg.qx << " "<< msg.qy << " " << msg.qz << " " << msg.qw << std::endl;
    }
}


int main (int argc, char **argv)
{
    int rc;
    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [vio target you subscribe to]" << std::endl;
        return -1;
    }

    auto msg_adp = std::make_shared<PIMsgAdaptor>();

    char url_t[64];

    sprintf(url_t, "%s:%s", argv[1], "4444");
    int sub_handler = msg_adp->registerOneMessageChannel({NN_SUB, url_t, "vio", -1});

    msg_adp->addSubscriberToSubMsg([&](int handler, p_pi_msg_envelope_t p_env, const char* body, unsigned int len){
        processVIOPos(handler, p_env, body, len);
        return 1;
    });

    viz::Viz3d vz("show_trajectories");
    auto win_size = vz.getWindowSize();

    auto cam = vz.getViewerPose();
    vz.setViewerPose(cam.translate({0.0, 0.0, 100.0}));

    auto pos_txt_widget = viz::WText("pos:", {0, 10});
    vz.showWidget("3dpos_txt", pos_txt_widget);

#if 1
    // draw grid lines
    for(int i = -400; i <= 400; i += 50)
    {
        string id = "x_" + to_string(i);
        vz.showWidget(id, WLine({-400.0, 1.0*i, 0}, {400.0, 1.0*i, 0}, Color::gray()));    

        id = "y_" + to_string(i);
        vz.showWidget(id, WLine({1.0*i, -400.0, 0}, {1.0*i, 400.0, 0}, Color::gray()));                
    }   
#endif

    path.push_back(cv::Affine3d::Identity());
    vz.showWidget("path", WTrajectory(path, WTrajectory::BOTH, 0.2, Color::brown()));

    msg_adp->startRecvLoop();

    int i = 0;
    while(!vz.wasStopped())
    {
        vz.removeWidget("path");
        vz.showWidget("path", WTrajectory(path, WTrajectory::BOTH, 0.2, Color::brown()));

        auto pos = path.back().translation();
        string txt = "pos: " + to_string(pos[0]) + ", " + to_string(pos[1]) + ", " + to_string(pos[2]);
        pos_txt_widget.setText(txt);

        vz.spinOnce(20, true);
    }

    vz.resetCamera();
    vz.spin();    

    return 0;
}
