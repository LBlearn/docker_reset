#ifndef DOCKER_LIMIT_HPP
#define DOCKER_LIMIT_HPP

#include"resource_config.hpp"
#include"subsystem.hpp"
#include<vector>

using std::vector;

class DockerLimit{
private:
    ResourceConfig *resource;
    vector<SubSystemInterface*> sub_system_v;

    void init_sub_system_v();
public:
    DockerLimit(const string& memory_limit,const string& cpu_share,const string& cpu_set);
    //进行资源限制
    void limit(pid_t pid,const string& image_name_id);
    //释放资源限制
    void release();
    ~DockerLimit();
};

#endif