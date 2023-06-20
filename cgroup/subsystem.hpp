#ifndef SUBSYSTEM_HPP
#define SUBSYSTEM_HPP

#include<string>
#include<iostream>
#include<fstream>
#include"sys/stat.h"
#include"unistd.h"

#include"error.hpp"
using std::string;

class SubSystemInterface{
protected:
    string limit_string;
    //每个子系统的系统hierarchy的创建
    string new_dir;

    virtual void make_dir(const string& image_name) = 0;
public:
    //获取subsystem的名字
    virtual string get_name() = 0;
    //将cgruop和soruce绑定
    virtual void set(const string& image_name_id) = 0;
    //将进程添加到cgroup中，这个操作对于所有的子系统都一样
    virtual void apply(pid_t pid) = 0;
    //移除某个cgroup
    void remove();
    //移除某个容器的cgroup
    void remove(const string& container_id);

    SubSystemInterface() = default;
    SubSystemInterface(const string& limit);
    virtual ~SubSystemInterface() = default;
};

class MemorySubSystem:public SubSystemInterface{
private: 
    const string MEMORY = "memory.limit_in_bytes";
    void make_dir(const string& image_name);
public:
    string get_name();
    
    void set(const string& image_name_id);
    
    void apply(pid_t pid);

    MemorySubSystem();
    MemorySubSystem(const string& limit);
    ~MemorySubSystem();
};

class CpusetSubSystem:public SubSystemInterface{
private:
    const string CPUSET = "cpuset.cpus";
    const string ASSIST = "cpuset.mems";
    void make_dir(const string& image_name);
public:
    string get_name();
    
    void set(const string& image_name_id);
    
    void apply(pid_t pid);

    CpusetSubSystem();
    CpusetSubSystem(const string& limit);
    ~CpusetSubSystem();
};

class CpushareSubSystem:public SubSystemInterface{
private:
    const string CPUSHARE = "cpu.shares";
    void make_dir(const string& image_name);
public:
    string get_name();
    
    void set(const string& image_name_id);
    
    void apply(pid_t pid);

    CpushareSubSystem();
    CpushareSubSystem(const string& limit);
    ~CpushareSubSystem();
};

#endif