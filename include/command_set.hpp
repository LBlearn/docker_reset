#ifndef COMMANDINTER_HPP
#define COMMANDINTER_HPP

#include<iostream>
#include<string>
#include<unistd.h>
#include<unordered_map>
#include<unordered_set>
#include<getopt.h>
#include<vector>
#include<experimental/filesystem>
#include<sys/mman.h>
#include<sys/mount.h>
#include<sys/stat.h>
#include<sys/syscall.h>
#include<iomanip>
#include<wait.h>

#include"utility.hpp"
#include"option_namespace.hpp"
#include"docker_limit.hpp"
#include"saving.hpp"
using namespace std;

namespace fs = std::experimental::filesystem;

class RunSource{
public:
    string image_name;
    string image_id;
    string bash_name;

    unordered_map<string,string> long_opt_map;
    unordered_map<char,string> short_opt_map;
    
    RunSource() = default;
    RunSource(const string& image_name,
                unordered_map<string,string>& long_opt_map,
                unordered_map<char,string>& short_opt_map);

    RunSource(const string& image_name,
                const string& image_id,
                unordered_map<string,string>& long_opt_map,
                unordered_map<char,string>& short_opt_map);

    // RunSource(string& image_name,
    //             string& bash_name,
    //             unordered_map<string,string>& long_opt_map,
    //             unordered_map<char,string>& short_opt_map);
                
    RunSource(const string& image_name,
                const string& bash_name,
                const string& image_id,
                unordered_map<string,string>& long_opt_map,
                unordered_map<char,string>& short_opt_map);

    void print_source_message();

    ~RunSource() = default;
};

class CommandInterface{
protected:
    unordered_map<string,string> long_opt_map;
    unordered_map<char,string> short_opt_map;

    //后续考虑从json文件读取配置
    unordered_set<char> short_opt_set;//存放短选项集合
    unordered_map<char,string> long2short_opt_map;//存放长选项集合

    char *short_opts;
    struct option *long_opts;

    inline void help();
    virtual void get_option(int argc,char **argv);
public:
    CommandInterface() = default;
    //进行值的深初始化，可以用智能指针吗？
    CommandInterface(char *short_opt,option *long_opt);
    /*
        short_opt:string类型短选项
        long_opt:vector<option>类型长选项
        func:将C++类型的变量封装成C类型成员参数，以便进行getopt_long函数的调用
    */
    CommandInterface(const string& short_opt,
                    vector<option>& long_opt);
    virtual void run(int argc,char **argv,const string& container_id);
    virtual ~CommandInterface();
};

class RunCommand:public CommandInterface{
private:
    string* image_name_ptr;
    string* shell_type_ptr;

    inline void help();
    void get_option(int argc,char **argv);
    //资源限制数组初始化
    void init_limit_v(vector<string>& limit_v);

    //打印map信息，测试使用
    void print_message();

    //将AUFS应用到docker中，不直接切换到镜像目录，而是创建一个AUFS挂载点
    void new_work_place(const string& container_id);
    //删除AUFS挂载部分
    void delete_work_place(const string& container_id);

    void run_image(const string& container_id,const string& command);
public:
    RunCommand();
    RunCommand(const string& short_opt,
                    vector<option>& long_opt);
    void run(int argc,char **argv,const string& container_id);
    ~RunCommand();
};

class ImageMessage{
private:
    string image_name;
    string tag;
    string create_time;
    string size;
public:
    ImageMessage() = default;
    ImageMessage(const string& image_path,const string& image_name,bool is_tar);
    ImageMessage(const string& image_path,const string& image_name);
    void print_messages();
    static void print_head();
    ~ImageMessage() = default;
};

class ContainersCommand:public CommandInterface{
private:    
    inline void help();
    void get_option(int argc,char **argv);
public:
    ContainersCommand();
    void run(int argc,char **argv,const string& container_id);
    ~ContainersCommand();
};

class ImagesCommand:public CommandInterface{
private:    
    inline void help();
    void get_option(int argc,char **argv);
public:
    ImagesCommand();
    void run(int argc,char **argv,const string& container_id);
    ~ImagesCommand();
};

class CommitCommand:public CommandInterface{
private:
    string container_id;

    inline void help();
    void save();
    void get_option(int argc,char **argv);
public:
    CommitCommand();
    void run(int argc,char **argv,const string& container_id);
    ~CommitCommand();
};

class StopContainer:public CommandInterface{
private:    
    inline void help();
    void get_option(int argc,char **argv);
public:
    StopContainer();
    void run(int argc,char **argv,const string& container_id);
    ~StopContainer();
};

class StartContainer:public CommandInterface{
private:    
    inline void help();
    void get_option(int argc,char **argv);
public:
    StartContainer();
    void run(int argc,char **argv,const string& container_id);
    ~StartContainer();
};

class ExecContainer:public CommandInterface{
private:    
    inline void help();
    void get_option(int argc,char **argv);
public:
    ExecContainer();
    void run(int argc,char **argv,const string& container_id);
    ~ExecContainer();
};

class KillContainer:public CommandInterface{
private:    
    inline void help();
    void get_option(int argc,char **argv);
public:
    KillContainer();
    void run(int argc,char **argv,const string& container_id);
    ~KillContainer();
};

#endif