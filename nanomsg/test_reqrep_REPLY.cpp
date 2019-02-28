
#include <stdio.h>
#include <iostream>
#include <msg_utils/pi_msg_adaptor.h>

#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

int main(int argc, char *argv[]) {
    char url[64] = "ipc:///home/julian/test_reqrep.ipc";

    std::shared_ptr<PIAUTO::msg::PIMsgAdaptor> msg_adp = std::make_shared<PIAUTO::msg::PIMsgAdaptor>();

    int reply_hander = msg_adp->registerOneMessageChannel({NN_REP, url, "liaomeng", -1});
    printf("[REPLY] reply_hander: %d\n", reply_hander);

    msg_adp->addSubscriberToRepMsg([&](int handler, p_pi_msg_envelope_t p_env, const char* body, unsigned int len){
        std::cout << "[REPLY] get request message here?" << std::endl;
        char recv[len+1];
        memcpy(recv, body, len);
        recv[len] = '\0';

        char reply_msg[100] = "wagawaga, get your message, this is my reply!";

        pi_msg_envelope_t env;
        memcpy(&env, p_env, sizeof(pi_msg_envelope_t));
        env.length = 100;

        // send reply
        return msg_adp->send(reply_hander, &env, reply_msg, 100, nullptr);

    });

    msg_adp->startRecvLoop();
    while(1);

    return 0;
}
