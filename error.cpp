#include"error.hpp"

inline string get_type_name(oper_type type){
    switch (type){
        case NO_COMMAND:
            return "NO_COMMAND";
            break;
        case COMMAND_NOT_EXIST:
            return "COMMAND_NOT_EXIST";
            break;
        case NO_ARGUMENT:
            return "NO_ARGUMENT";
            break;
        case ARGUMENT_NOT_EXIST:
            return "ARGUMENT_NOT_EXIST";
            break;
        case OPTION_NOT_EXIST:
            return "OPTION_NOT_EXIST";
            break;
        case STACK_ALLOC_ERROR:
            return "STACK_ALLOC_ERROR";
            break;
        case MOUNT_PROC_FAIL:
            return "MOUNT_PROC_FAIL";
            break;
        case EXECV_ERROR:
            return "EXECV_ERROR";
            break;
        case IMAGE_NOT_EXSIT:
            return "IMAGE_NOT_EXSIT";
            break;
        case MOUNT_ERROR:
            return "MOUNT_ERROR";
            break;
        case MKDIR_ERROR:
            return "MKDIR_ERROR";
            break;
        case PIVOT_ROOT_SYSCALL_ERROR:
            return "PIVOT_ROOT_SYSCALL_ERROR";
            break;
        case CHDIR_ERROR:
            return "CHDIR_ERROR";
            break;
        case RMDIR_ERROR:
            return "RMDIR_ERROR";
            break;
        case OPEN_FILE_ERROR:
            return "OPEN_FILE_ERROR";
            break;
        case CHMOD_ERROR:
            return "CHMOD_ERROR";
            break;
        case VOLUME_PARA_ERROR:
            return "VOLUME_PARA_ERROR";
            break;
        default:
            return "NOT DEFINE ERROR"; 
            break;
    }
    
}

void print_error(const char* func_name,oper_type type){
    if(type != SUCCESS){
        cout<<"ERROR!!\n\n  func："<<func_name<<endl;
        cout<<"  error type："<<get_type_name(type)<<endl;
        exit(0);
    }
}


void print_error(const char* func_name,const string& error_str){
    cout<<"ERROR!!\n\n  func："<<func_name<<endl;
    cout<<"  error type："<<error_str<<endl;
    exit(0);
}