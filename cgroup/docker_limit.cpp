#include "docker_limit.hpp"

DockerLimit::DockerLimit(const string& memory_limit,const string& cpu_share,const string& cpu_set)
{
    resource = new ResourceConfig(memory_limit,cpu_share,cpu_set);
    }

void DockerLimit::init_sub_system_v()
{
    // cout<<__func__<<endl;
    if(resource->get_memory_limit().compare("NULL") != 0)
        sub_system_v.push_back(new MemorySubSystem(resource->get_memory_limit()));
    if(resource->get_cpu_set().compare("NULL") != 0)
        sub_system_v.push_back(new CpusetSubSystem(resource->get_cpu_set()));
    if(resource->get_cpu_share().compare("NULL") != 0)
        sub_system_v.push_back(new CpushareSubSystem(resource->get_cpu_share()));
}


void DockerLimit::limit(pid_t pid,const string& image_name_id)
{
    init_sub_system_v();
    for(auto val:sub_system_v){
        // cout<<val->get_name()<<endl;
        val->set(image_name_id);
        val->apply(pid);
    }
}

void DockerLimit::release()
{
    for(auto val:sub_system_v)
        val->remove();
}

DockerLimit::~DockerLimit()
{
    delete resource;
    for(auto val:sub_system_v)
        delete val;
}