
#include <stdio.h>
#include <functional>
#include <iostream>
#include <msg_utils/pi_msg_adaptor.h>

#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

int main(int argc, char *argv[]) {
    char url[64] = "ipc:///home/julian/test_reqrep.ipc";

    std::shared_ptr<PIAUTO::msg::PIMsgAdaptor> msg_adp = std::make_shared<PIAUTO::msg::PIMsgAdaptor>();

    int request_hander = msg_adp->registerOneMessageChannel({NN_REQ, url, "liaomeng", -1});
    printf("[REQUEST] request_hander: %d\n", request_hander);

    std::string request_msg = "hello, this is a test!";
    pi_msg_envelope_t env;
    env.length = request_msg.size();

    using pi_msg_callback_t = int(int handler, p_pi_msg_envelope_t p_env, const char *body, unsigned int len);
    std::function<pi_msg_callback_t> callback = [&](int handler, p_pi_msg_envelope_t p_env, const char *body, unsigned int len) {
        std::cout << "[REQUEST] get reply message here?" << std::endl;
        char recv[len+1];
        memcpy(recv, body, len);
        recv[len] = '\0';

        printf("[REQUEST] reply messge: %s\n", recv);

        return 0;
    };

    msg_adp->startRecvLoop();

    while (1) {
      // send request
      msg_adp->send(request_hander, &env, request_msg.c_str(), env.length, callback);
      sleep(1);
    }

    return 0;
}
