#ifndef ERROR_HPP
#define ERROR_HPP

#include<string>
#include<iostream>
using std::string;
using std::cout;
using std::endl;

enum oper_type{
    SUCCESS = 0,
    NO_COMMAND,
    COMMAND_NOT_EXIST,
    NO_ARGUMENT, 
    ARGUMENT_NOT_EXIST,
    OPTION_NOT_EXIST,
    STACK_ALLOC_ERROR,
    MOUNT_PROC_FAIL,
    EXECV_ERROR,
    IMAGE_NOT_EXSIT,
    MOUNT_ERROR,
    MKDIR_ERROR,
    PIVOT_ROOT_SYSCALL_ERROR,
    CHDIR_ERROR,
    RMDIR_ERROR,
    OPEN_FILE_ERROR,
    CHMOD_ERROR,
    VOLUME_PARA_ERROR
};

void print_error(const char* func_name,oper_type type);

void print_error(const char* func_name,const string& error_str);

#endif