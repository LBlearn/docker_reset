#ifndef RESOURCE_CONFIG_HPP
#define RESOURCE_CONFIG_HPP

#include<string>
#include<iostream>
using std::string;

class ResourceConfig{
private:
    string memory_limit;
    string cpu_share;
    string cpu_set;
public:
    string get_memory_limit();
    string get_cpu_share();
    string get_cpu_set();

    void print_message();

    ResourceConfig() = default;
    ResourceConfig(const string &memory,const string &cpushare,const string &cpuset);
    ~ResourceConfig() = default; 
};

#endif