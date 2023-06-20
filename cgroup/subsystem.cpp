#include "subsystem.hpp"

namespace system_path{
    static std::string& getmemControlPath(){
        static std::string memControlPath = "/sys/fs/cgroup/memory/dummyDocker";
        return memControlPath;
    }
    static std::string& getcpuShareControlPath(){
        static std::string cpuShareControlPath = "/sys/fs/cgroup/cpu/dummyDocker";
        return cpuShareControlPath;
    }
    static std::string& getcpuSetControlPath(){
        static std::string cpuSetControlPath = "/sys/fs/cgroup/cpuset/dummyDocker";
        return cpuSetControlPath;
    }
};


/*
    SubSystemInterface Function
*/
//remove通用，只要删除new_dir(容器的目录)就行了
void SubSystemInterface::remove()
{
    if(rmdir(new_dir.c_str())!=0)
        print_error(__func__,RMDIR_ERROR);
}

void SubSystemInterface::remove(const string& container_id){
    string delete_container_path = new_dir+"/"+container_id;
    cout<<delete_container_path<<endl;
    if(rmdir(delete_container_path.c_str())!=0)
        print_error(__func__,RMDIR_ERROR);
}

SubSystemInterface::SubSystemInterface(const string &limit)
    :limit_string(limit)
{
}

/*
    MemorySubSystem Function
*/

void MemorySubSystem::make_dir(const string& image_name_id)
{
    mkdir(system_path::getmemControlPath().c_str(),0777);

    new_dir = system_path::getmemControlPath()+"/"+image_name_id;
    //创建系统子系统的子Cgroup文件夹，要分两部创建居然s

    if(mkdir(new_dir.c_str(),0777)!=0)
        print_error(__func__,MKDIR_ERROR);

}

string MemorySubSystem::get_name()
{
    return "memory subsystem";
}

void MemorySubSystem::set(const string& image_name_id)
{
    make_dir(image_name_id);
    std::ofstream file;
    file.open(new_dir+"/"+MEMORY); 
    if(!file)
        print_error(__func__,OPEN_FILE_ERROR);
    if(limit_string.compare("NULL") != 0)
        file << limit_string;
    file.close(); 
}

void MemorySubSystem::apply(pid_t pid)
{
    std::ofstream file;
    file.open(new_dir+"/tasks");
    if(!file)
        print_error(__func__,OPEN_FILE_ERROR);
    file << pid; 
    file.close();
}

MemorySubSystem::MemorySubSystem()
{
}

MemorySubSystem::MemorySubSystem(const string &limit):SubSystemInterface(limit)
{
}

MemorySubSystem::~MemorySubSystem()
{
}


/*
    CpusetSubSystem Function
*/

//对cpuset写入的修正
void add_0_to_cpusetmems(const string& path){
    std::ofstream file;

    if(chmod(path.c_str(),0777) == -1)
        print_error(__func__,CHMOD_ERROR);

    // cout<<path<<endl;
    file.open(path); 

    if(!file)
        print_error(__func__,OPEN_FILE_ERROR);
        
    file<<0;

    file.close(); 
}

void CpusetSubSystem::make_dir(const string& image_name_id)
{
    mkdir(system_path::getcpuSetControlPath().c_str(),0777);

    add_0_to_cpusetmems(system_path::getcpuSetControlPath()+"/"+ASSIST);

    new_dir = system_path::getcpuSetControlPath()+"/"+image_name_id;
    //创建系统子系统的子Cgroup文件夹
    if(mkdir(new_dir.c_str(),0777)!=0)
        print_error(__func__,MKDIR_ERROR);
}

string CpusetSubSystem::get_name()
{
    return "cpu_set subsystem";
}

void CpusetSubSystem::set(const string& image_name_id)
{
    make_dir(image_name_id);

    add_0_to_cpusetmems(new_dir+"/"+ASSIST);

    std::ofstream file;
    file.open(new_dir+"/"+CPUSET); 

    if(chmod((new_dir+"/"+CPUSET).c_str(),0777) == -1)
        print_error(__func__,CHMOD_ERROR);

    if(!file)
        print_error(__func__,OPEN_FILE_ERROR);
    
    if(limit_string.compare("NULL") != 0)
        
    file<<image_name_id;

    file.close(); 
}

void CpusetSubSystem::apply(pid_t pid)
{
    std::ofstream file;
    file.open(new_dir+"/tasks");
    // cout<<new_dir+"/tasks"<<endl;

    if(chmod((new_dir+"/tasks").c_str(),0777) == -1)
        print_error(__func__,CHMOD_ERROR);

    if(!file)
        print_error(__func__,OPEN_FILE_ERROR);
    file << pid; 
    file.close();
}

CpusetSubSystem::CpusetSubSystem()
{
}

CpusetSubSystem::CpusetSubSystem(const string &limit):SubSystemInterface(limit)
{
}

CpusetSubSystem::~CpusetSubSystem()
{
}


/*
    CpushareSubSystem Function
*/

void CpushareSubSystem::make_dir(const string& image_name_id)
{
    mkdir(system_path::getcpuShareControlPath().c_str(),0777);

    new_dir = system_path::getcpuShareControlPath()+"/"+image_name_id;
    //创建系统子系统的子Cgroup文件夹
    if(mkdir(new_dir.c_str(),0777)!=0)
        print_error(__func__,MKDIR_ERROR);
}

string CpushareSubSystem::get_name()
{
    return "cpu_share subsystem";
}

void CpushareSubSystem::set(const string& image_name_id)
{   
    make_dir(image_name_id);
    std::ofstream file;
    file.open(new_dir+"/"+CPUSHARE); 
    if(!file)
        print_error(__func__,OPEN_FILE_ERROR);
    if(limit_string.compare("NULL") != 0)
        file << limit_string;
    file.close(); 
}

void CpushareSubSystem::apply(pid_t pid)
{
    std::ofstream file;
    file.open(new_dir+"/tasks");
    if(!file)
        print_error(__func__,OPEN_FILE_ERROR);
    file << pid; 
        file.close();
}

CpushareSubSystem::CpushareSubSystem()
{
}

CpushareSubSystem::CpushareSubSystem(const string &limit):SubSystemInterface(limit)
{
}

CpushareSubSystem::~CpushareSubSystem()
{
}