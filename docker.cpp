#include"docker.hpp"

Docker::Docker(){
    //通过现在的时间离格林尼治时间的毫秒数来创建ContainerId
    timespec time;
    timespec_get(&time,TIME_UTC);
    container_id = hex2str(time.tv_nsec);
}

void Docker::init_docker(int argc, char **argv){
    this->argc = argc;
    this->argv = argv;
    string command(argv[1]);
    if(!command.compare("run")){
        // cout<<"RUN COMMAND"<<endl;
        this->command_ptr = new RunCommand();
    }
    else if(!command.compare("images")){
        // cout<<"IMAGES COMMAND"<<endl;
        this->command_ptr = new ImagesCommand();
    }
    else if(!command.compare("containers")){
        // cout<<"IMAGE COMMAND"<<endl;
        this->command_ptr = new ContainersCommand();
    }
    else if(!command.compare("commit")){
        this->command_ptr = new CommitCommand();
    }
    else{
        this->command_ptr = new CommandInterface();
    }
}

void Docker::start(){
    this->command_ptr->run(argc,argv,container_id);
}

Docker::~Docker(){
    delete this->command_ptr;
}