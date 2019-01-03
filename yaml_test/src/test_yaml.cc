#include <iostream>

#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>

int main(int argc, char *argv[]) {
    YAML::Node config = YAML::LoadFile("../config/config.yaml");

    if (config["debug"]) {
        int debug = config["debug"]["on"].as<int>();
        std::cout << "debug: " << debug << std::endl;
    }

    int controller_id = config["controller_id"].as<int>();
    std::cout << "controller id: " << std::hex << controller_id << std::endl;

    return 0;
}
