#ifndef UTILITY_HPP
#define UTILITY_HPP
#include<string>
#include<vector>
#include<algorithm>
#include<cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <sstream>
#include <iomanip>

#include"option_namespace.hpp"
#include"error.hpp"
#include"ctime"
using std::string;

extern "C"{
char * string2char(const string& s);
}

namespace Path{
    static const string IMAGE_PATH = "../dockerHome/root/image";
};

string get_image_path(const string& image_name);

void is_valid(int argc,char **argv,oper_type& type);

//将10进制时间转化成16进制时间并返回字符串
string hex2str(time_t num);

//获取路径文件夹的字节大小
size_t get_dir_size(const char *dir);

//获取路径文件夹的字节大小，返回字符串
string get_dir_size_str(const char *dir);

//获取文件大小，返回字符串
string get_file_size(const char* fname);

//获取指定文件夹的创建时间
string get_dir_time(const char *dir);

//获取指定文件夹下的所有文件名
std::vector<string> get_dir_all(const char *dir);
#endif
