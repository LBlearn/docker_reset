#ifndef SAVING_HPP
#define SAVING_HPP
#include<string>
#include<iostream>
#include<fstream>
#include<iomanip>
#include"error.hpp"
#include"utility.hpp"

using namespace std;

static const std::string CONTAINER_METEDATA_PATH = "../dockerHome/metedata/container/";

class ContainerMeteData
{
private:
    string image_name;
    string create_time;
    string container_id;
    string command;
    string status;
    int container_pid;
public:
    static void print_container_head();
    void print_container_message();
    string get_container_metedata_string();
    ContainerMeteData() = default;
    ContainerMeteData(const string& image_name,const string& create_time,
                     const string& container_id,const string& command,
                     const string& status,pid_t container_pid);
    ~ContainerMeteData() = default;
};

class ContainerDataMessage{
private:
    ContainerMeteData container_message;
    string container_id;
public:
    ContainerDataMessage();
    ContainerDataMessage(const string& image_name,const string& create_time,
                     const string& container_id,const string& command,
                     const string& status,pid_t container_pid);
    ContainerDataMessage(const string& path);
    ~ContainerDataMessage();
    void write_metedata();
    void print_metedate();
    ContainerMeteData get_metedata();
};

#endif