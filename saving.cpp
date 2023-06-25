#include"saving.hpp"

void ContainerMeteData::print_container_head()
{
    cout<<std::left<<setw(25)<<"CONTAINER_ID"
        <<std::left<<setw(15)<<"IMAGE_NAME"
        <<std::left<<setw(25)<<"CREATED_TIME"
        <<std::left<<setw(10)<<"STATUS"
        <<std::left<<setw(10)<<"COMMAND"
        <<endl;

}

void ContainerMeteData::print_container_message()
{
    cout<<std::left<<setw(25)<<container_id
        <<std::left<<setw(15)<<image_name
        <<std::left<<setw(25)<<create_time
        <<std::left<<setw(10)<<status
        <<std::left<<setw(30)<<command
        <<endl;
}

string ContainerMeteData::get_container_metedata_string()
{
    return image_name + "|" + create_time + "|" + container_id
            + "|" + command + "|" + status + "|" + to_string(container_pid);
}

ContainerMeteData::ContainerMeteData(const string& image_name,const string& create_time,
                     const string& container_id,const string& command,
                     const string& status,pid_t container_pid)
    :image_name(image_name),create_time(create_time),container_id(container_id),command(command),status(status),container_pid(container_pid)
{
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++

ContainerDataMessage::ContainerDataMessage(){

}
ContainerDataMessage::ContainerDataMessage(const string& image_name,const string& create_time,
                     const string& container_id,const string& command,
                     const string& status,pid_t container_pid)
                     :container_id(container_id),container_message(ContainerMeteData(image_name,create_time,container_id,command,status,container_pid)){
}

ContainerDataMessage::ContainerDataMessage(const string& path){
    ifstream in(path);
    string line;
    // 将镜像的元数据每一行放给line的string变量
    while(getline(in,line)){
    // Containerid imagename command createTime pid
        auto tmp = split(line, '|');
        if(tmp.size() != 6) 
            print_error(__func__,"CONTAINER_METEDATA_ERROR");
        container_message = ContainerMeteData(tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],stoi(tmp[5]));
    }
}

ContainerDataMessage::~ContainerDataMessage(){

}

void ContainerDataMessage::write_metedata(){
    std::ofstream ofs;
    string path = CONTAINER_METEDATA_PATH+this->container_id + ".txt"; 
    cout<<path<<endl;
    ofs.open(path.c_str(),ios_base::app|ios_base::out);
    if(!ofs.is_open()){
        print_error(__func__,"OPEN_FILE_ERROR");
    }
    ofs<< container_message.get_container_metedata_string()<<endl;
    ofs.close();
};

void ContainerDataMessage::print_metedate(){
    container_message.print_container_message();
};

ContainerMeteData ContainerDataMessage::get_metedata(){
    return this->container_message;
}