#ifndef DOCKER_HPP
#define DOCKER_HPP

#include"command_set.hpp"
#include"error.hpp"

class Docker{
private:
    CommandInterface *command_ptr = nullptr;
    string container_id;
    int argc;
    char **argv;
public:
    Docker();
    void init_docker(int argc,char **argv);
    void start();
    ~Docker();
};


#endif