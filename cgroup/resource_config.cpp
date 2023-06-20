#include"resource_config.hpp"

string ResourceConfig::get_memory_limit()
{
    return memory_limit;
}

string ResourceConfig::get_cpu_share()
{
    return cpu_share;
}

string ResourceConfig::get_cpu_set()
{
    return cpu_set;
}

void ResourceConfig::print_message()
{
    using std::cout;
    using std::endl;
    cout<<"memory_limit = "<<memory_limit<<endl;
    cout<<"cpu_set = "<<cpu_set<<endl;
    cout<<"cpu_share = "<<cpu_share<<endl;
}

ResourceConfig::ResourceConfig(const string &memory,const string &cpushare,const string &cpuset)
    :memory_limit(memory),cpu_share(cpushare),cpu_set(cpuset){
}