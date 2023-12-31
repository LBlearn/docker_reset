#include"command_set.hpp"

/*
    RunSource FunctionF
*/

RunSource::RunSource(const string& image_name,
            unordered_map<string,string>& long_opt_map,
            unordered_map<char,string>& short_opt_map):
                image_name(image_name),
                short_opt_map(short_opt_map),long_opt_map(long_opt_map){
                    bash_name = "NULL";
                };

RunSource::RunSource(const string& image_name,
                const string& image_id,
                unordered_map<string,string>& long_opt_map,
                unordered_map<char,string>& short_opt_map):
                    image_name(image_name),image_id(image_id),
                    short_opt_map(short_opt_map),long_opt_map(long_opt_map){};

// RunSource::RunSource(string& image_name,
//             string& bash_name,
//             unordered_map<string,string>& long_opt_map,
//             unordered_map<char,string>& short_opt_map):
//                 image_name(image_name),bash_name(bash_name),
//                 short_opt_map(short_opt_map),long_opt_map(long_opt_map){};

RunSource::RunSource(const string& image_name,
                const string& bash_name,
                const string& image_id,
                unordered_map<string,string>& long_opt_map,
                unordered_map<char,string>& short_opt_map):
                    image_name(image_name),image_id(image_id),bash_name(bash_name),
                    short_opt_map(short_opt_map),long_opt_map(long_opt_map){};

void RunSource::print_source_message(){
    cout<<"shell_name："<<bash_name<<endl;
    cout<<"iamge_name："<<image_name<<endl;
    for(auto &val:short_opt_map){
        cout<<val.first<<" "<<val.second<<endl;
    }
    for(auto &val:long_opt_map){
        cout<<val.first<<" "<<val.second<<endl;
    }
}

/*
    CommandInterface Function
*/
inline void CommandInterface::help(){
    cout<<"Command options:"<<endl;
    cout<<"\trun             run the image to a container"<<endl;
    cout<<"\tcommit          save the container to image"<<endl;
    cout<<"\tcontainers      check the messages of the contaniers now"<<endl;
    cout<<"\timages          check the messages of the images now"<<endl;
    cout<<"\tstop            stop the detach container"<<endl;
    cout<<"\tstart           start the stopped container"<<endl;
    cout<<"\texec            start a shell for the container"<<endl;
    cout<<"\tkill            end the container of the container_id"<<endl;
}

CommandInterface::CommandInterface(char *short_opt,option *long_opt){
    // memcpy(short_opts,short_opt,sizeof(short_opt));
};
/*
    short_opt:string类型短选项
    long_opt:vector<option>类型长选项
    func:将C++类型的变量封装成C类型成员参数，以便进行getopt_long函数的调用
*/
CommandInterface::CommandInterface(const string& short_opt,
    vector<option>& long_opt){
    short_opts = string2char(short_opt);

    long_opts = (option *)malloc(sizeof(option)*long_opt.size());
    int index = 0;
    for(auto val:long_opt){
        long_opts[index++] = option(val);
        // cout<<long_opts[index - 1].name<<endl;
    }
}

void CommandInterface::get_option(int argc,char **argv){
    help();
};

void CommandInterface::run(int argc,char **argv,const string& container_id){
    get_option(argc,argv);
}

CommandInterface::~CommandInterface(){};

/*
    RunCommand Function
    all function 
*/
inline void RunCommand::help(){
    cout<<"Format:\n"
        <<"\tdocker run [-opt] [para] image_name [shell_type]"<<endl;
    cout<<"run Options:"<<endl;
    cout<<"\t-h,--help               Print the message"<<endl;
    cout<<"\t-T,--test               Use to test the function"<<endl;
    cout<<"\t-i                      长期运行"<<endl;
    cout<<"\t-t                      分配终端"<<endl;
    cout<<"\t-d                      后台运行镜像"<<endl;
    cout<<"\t-m,--memory             Set the limit of memory使用"<<endl;
    cout<<"\t-c,--cpuset             Set the limit of CPU使用"<<endl;
    cout<<"\t-C,--cpushare           Set the limit of CPU分配比例"<<endl;
}

RunCommand::RunCommand():CommandInterface(
    RunCommandPara::stl_short_opt_def,
    RunCommandPara::stl_long_opt_def){
    
    short_opt_set = unordered_set<char>(RunCommandPara::stl_short_opt_def.begin(),RunCommandPara::stl_short_opt_def.end());

    for(int i = 0;i < RunCommandPara::stl_long_opt_def.size()-1;++i){
        long2short_opt_map[(char)RunCommandPara::stl_long_opt_def[i].val] = RunCommandPara::stl_long_opt_def[i].name;
    }    
};

RunCommand::RunCommand(const string& short_opt,
                vector<option>& long_opt)
                :CommandInterface(short_opt,long_opt){
    
    short_opt_set = unordered_set<char>(RunCommandPara::stl_short_opt_def.begin(),RunCommandPara::stl_short_opt_def.end());

    for(int i = 0;i < RunCommandPara::stl_long_opt_def.size()-1;++i){
        long2short_opt_map[(char)RunCommandPara::stl_long_opt_def[i].val] = RunCommandPara::stl_long_opt_def[i].name;
    }    
};

void RunCommand::get_option(int argc,char **argv){
    char ch;
    int index;
    while((ch = getopt_long(argc,argv,RunCommandPara::short_opt_def,RunCommandPara::long_opt_def,&index)) != -1){
        if(short_opt_set.find(ch) != short_opt_set.end()){
            short_opt_map[ch] = (optarg? optarg : "NULL");
        }
        else if(long2short_opt_map.find(ch) != long2short_opt_map.end()){
            long_opt_map[long2short_opt_map[ch]] = (optarg? optarg : "NULL");
        }
        else{
            help();
            print_error(__func__,OPTION_NOT_EXIST);
        }
    }
    //当选项不为help时运行镜像，根据是否有-t选项，获取shell的name和image的name
    if(long_opt_map.find("help") == long_opt_map.end()
        || short_opt_map.find('h') == short_opt_map.end()){
        
        if(short_opt_map.find('t') != short_opt_map.end()){
            shell_type_ptr = new string(argv[argc-1]);
            image_name_ptr = new string(argv[argc-2]);
        }else{
            image_name_ptr = new string(argv[argc-1]);
        }
    }
    // print_message();
}

void RunCommand::init_limit_v(vector<string> &limit_v){
    if(short_opt_map.find('m') != short_opt_map.end()){
        limit_v.push_back(short_opt_map['m']);
    }
    else{
        limit_v.push_back("NULL");
    }

    if(short_opt_map.find('C') != short_opt_map.end()){
        limit_v.push_back(short_opt_map['C']);
    }
    else{
        limit_v.push_back("NULL");
    }

    if(short_opt_map.find('c') != short_opt_map.end()){
        limit_v.push_back(short_opt_map['c']);
    }
    else{
        limit_v.push_back("NULL");
    }
};

void RunCommand::print_message(){
    cout<<"shell_type:"<<*shell_type_ptr<<endl
        <<"image name:"<<*image_name_ptr<<endl;
    cout<<"short opts:"<<endl;
    for(const auto& val :short_opt_map){
        cout<<val.first<<"\t"<<val.second<<endl;
    }

    cout<<"long opts:"<<endl;
    for(const auto& val :long_opt_map){
        cout<<val.first<<"\t"<<val.second<<endl;
    }
}

//AUFS文件系统的创建子函数
namespace aufs_sub_func{
    const static string AUFS_PATH = "../dockerHome/aufs/";
    void mkdir_container_aufs_root(const string &image_ID){
        string container_aufs_root = AUFS_PATH+image_ID;
        // cout<<container_aufs_root<<endl;
        if(mkdir(container_aufs_root.c_str(),0777) != 0){
            print_error(__func__,MKDIR_ERROR);
        }
    }
    
    //需要复制一次吗？？？
    void create_read_only_layer(const string &image_name,const string &container_ID){
        //创建容器的aufs系统根目录
        mkdir_container_aufs_root(image_name+container_ID);
        //获取镜像路径
        string source_dir = get_image_path(image_name);
        //将镜像复制到aufs的只读目录中,先创建只读目录
        string read_only_dir = AUFS_PATH+image_name + container_ID + "/" + "read_only_layer";
        if(mkdir(read_only_dir.c_str(),0777) != 0){
            print_error(__func__,MKDIR_ERROR);
        }

        string copy_command = "sudo cp -r "+source_dir+"/* "+read_only_dir;
        system(copy_command.c_str());
    }

    void create_write_layer(const string &image_name,const string &container_ID){
        // //创建容器的aufs系统根目录
        mkdir_container_aufs_root(image_name+container_ID);

        string write_only_layer = AUFS_PATH + image_name+container_ID + "/" + "rw_layer";
        if(mkdir(write_only_layer.c_str(),0777) != 0){
            print_error(__func__,MKDIR_ERROR);
        }
    }

    void create_mnt_layer(const string &image_name,const string &container_ID){
        string mnt_dir = AUFS_PATH + image_name+container_ID + "/" + "mnt";
        if(mkdir(mnt_dir.c_str(),0777) != 0){
            print_error(__func__,MKDIR_ERROR);
        }              
    }

    void mount_aufs2mnt(const string &image_name,const string &container_ID){
        string mount_command = "sudo mount -t aufs -o dirs="
                                + AUFS_PATH+ image_name+container_ID+"/rw_layer/:" 
                                + get_image_path(image_name) + "/"+ " none " 
                                + AUFS_PATH+ image_name+container_ID+"/mnt/";
        // cout<<mount_command<<endl;   
        system(mount_command.c_str()); 
    }

    //下面是删除挂载部分的函数
    //取消aufs挂载，并删除mnt目录
    void unmount_aufs(const string&& container_ID){
        string mnt_dir = AUFS_PATH + container_ID + "/" + "mnt";
        
        if(umount(mnt_dir.c_str()) != 0){
            print_error(__func__,"UNMOUNT_ERROR");
        }
        if(rmdir(mnt_dir.c_str()) != 0){
            print_error(__func__,RMDIR_ERROR);
        }
    }

    //删除write_dir和根目录
    void rm_write_dir(const string&& container_ID){
        string write_only_dir = AUFS_PATH + container_ID + "/" + "write_layer";
        string total_dir = AUFS_PATH + container_ID ;

        string rm_command = "rm -rf "+total_dir+"";
        system(rm_command.c_str());
    }

    //返回容器应该改的真的pivot
    string get_real_root(const string& container_ID){
        string mnt_dir = AUFS_PATH + container_ID + "/" + "mnt";
        return mnt_dir;
    }
}

//run命令的子函数
namespace run_sub_func{
    //shell和t进行绑定
    void start_bash(string& shell){
        char *c_shell = new char[shell.size()+1];
        strcpy(c_shell,shell.c_str());
        char* const chiled_args[] = {c_shell,NULL};
        if(execv(chiled_args[0],chiled_args) == -1)
            print_error(__func__,EXECV_ERROR);
        delete []c_shell;
    }

    //切换root文件系统到mnt目录
    void set_rootdir(const string& image_name){
        // cout<<get_current_dir_name()<<endl;
        string path = aufs_sub_func::get_real_root(image_name);

        //判断镜像是否存在，当镜像不存在时结束进程，打印错误信息
        if(access(path.c_str(),F_OK) == -1)
            print_error(__func__,IMAGE_NOT_EXSIT);

        chdir(path.c_str());
                // cout<<get_current_dir_name()<<endl;
        chroot(".");
    }

    //用pivot_root系统调用切换root文件系统
    void pivot_root(const string& image_name,const string& image_id){

        string new_root = aufs_sub_func::get_real_root(image_name+image_id);

        // cout<<new_root<<endl;

        if(mount("/", "/", "", MS_PRIVATE|MS_REC,"") != 0)
            print_error(__func__,MOUNT_ERROR);

        //判断镜像是否存在，当镜像不存在时结束进程，打印错误信息
        if(access(new_root.c_str(),F_OK) == -1)
            print_error(__func__,IMAGE_NOT_EXSIT);

        //将新的目录重新挂载一次，bind mount
        if(mount(new_root.c_str(), new_root.c_str(), "bind", MS_BIND|MS_REC,"") != 0)
            print_error(__func__,MOUNT_ERROR);

        //文件夹不存在则创建pivotRoot存储old_root，创建old_root目录
        string old_root = new_root + "/pivot_root";
        if(access(old_root.c_str(),F_OK) == -1)
            if(mkdir(old_root.c_str(), 0777)!=0)
                print_error(__func__,MKDIR_ERROR);

        //pivot_root系统调用
        if(syscall(SYS_pivot_root, new_root.c_str(), old_root.c_str()) != 0)
            print_error(__func__,PIVOT_ROOT_SYSCALL_ERROR);
    
        //将当前的工作目录修改成根目录
        if(chdir("/")!=0)
            print_error(__func__,CHDIR_ERROR);
            
        //将原来的rootfs取消挂载并删除
        if(umount2("/pivot_root",MNT_DETACH) != 0)
            print_error(__func__,"UNMOUNT_ERROR");

        if(rmdir("/pivot_root")!=0)
            print_error(__func__,RMDIR_ERROR);

        if(mount("tmpfs","/dev","tmpfs",MS_NOSUID|MS_STRICTATIME,"mode=755") != 0)
            print_error(__func__,MOUNT_ERROR);
    }

    void pivot_root(const string& image_name){

        string new_root = aufs_sub_func::get_real_root(image_name);
        // cout<<new_root<<endl;

        if(mount("/", "/", "", MS_PRIVATE|MS_REC,"") != 0)
            print_error(__func__,MOUNT_ERROR);

        //判断镜像是否存在，当镜像不存在时结束进程，打印错误信息
        if(access(new_root.c_str(),F_OK) == -1)
            print_error(__func__,IMAGE_NOT_EXSIT);

        //将新的目录重新挂载一次，bind mount
        if(mount(new_root.c_str(), new_root.c_str(), "bind", MS_BIND|MS_REC,"") != 0)
            print_error(__func__,MOUNT_ERROR);

        //文件夹不存在则创建pivotRoot存储old_root，创建old_root目录
        string old_root = new_root + "/pivot_root";
        if(access(old_root.c_str(),F_OK) == -1)
            if(mkdir(old_root.c_str(), 0777)!=0)
                print_error(__func__,MKDIR_ERROR);

        //pivot_root系统调用
        if(syscall(SYS_pivot_root, new_root.c_str(), old_root.c_str()) != 0)
            print_error(__func__,PIVOT_ROOT_SYSCALL_ERROR);
    
        //将当前的工作目录修改成根目录
        if(chdir("/")!=0)
            print_error(__func__,CHDIR_ERROR);
            
        //将原来的rootfs取消挂载并删除
        if(umount2("/pivot_root",MNT_DETACH) != 0)
            print_error(__func__,"UNMOUNT_ERROR");

        if(rmdir("/pivot_root")!=0)
            print_error(__func__,RMDIR_ERROR);

        if(mount("tmpfs","/dev","tmpfs",MS_NOSUID|MS_STRICTATIME,"mode=755") != 0)
            print_error(__func__,MOUNT_ERROR);
    }

    //挂载文件系统
    void mount_proc(){
    // cout<<__func__<<endl;
        if(mount("none","/proc","proc",0,nullptr) != 0)
            print_error(__func__,MOUNT_PROC_FAIL);
        mount("none","/sys","sys",0,nullptr);
    }
}

//数据卷相关子函数
static const int HOME_INDEX = 0;
static const int CONTAINER_INDEX = 1;

//将数据卷参数分解成两条路径
vector<string> split_volume(const string& volume){
    int index = 0;
    for(;index < volume.size() && volume[index] != ':';++index);

    if(index == volume.size()){
        print_error(__func__,VOLUME_PARA_ERROR);
    }

    vector<string> volume_urls;
    volume_urls.emplace_back(string(volume.begin(),volume.begin()+index));
    volume_urls.emplace_back(string(volume.begin()+index+1,volume.end()));

    return volume_urls;
}

//数据卷的挂载
void mount_volume(vector<string> volume_urls,const string& container_ID){
    string home_url = volume_urls[HOME_INDEX];
    //获取父目录的路径
    fs::path parentPath = fs::current_path().parent_path();

    string container_url(parentPath.c_str());
    container_url += "/dockerHome/aufs/"+container_ID+"/mnt"+volume_urls[CONTAINER_INDEX];

    // cout<<home_url<<endl<<container_url<<endl;

    //数据卷目录不存在时，创建数据卷目录
    if(access(home_url.c_str(),F_OK) != 0){
        if(mkdir(home_url.c_str(),0777) != 0){
            cout<<"home??"<<endl;
            print_error(__func__,MKDIR_ERROR);
        }
    }

    //容器内目录不存在时，创建容器内目录
    if(access(container_url.c_str(),F_OK) != 0){
        if(mkdir(container_url.c_str(),0777) != 0){
            cout<<"container??"<<endl;
            print_error(__func__,MKDIR_ERROR);
        }
    }

    string mount_command = "sudo mount -t aufs -o dirs="
                            + home_url + " none " + container_url;
    system(mount_command.c_str());
}

void volume_set(const string& volume,const string& container_ID = "0"){
    
    cout<<endl<<volume<<endl;
    auto volume_urls = split_volume(volume);
}

void unmoumt_volume(const string& volume,const string& container_ID){
    auto volume_urls = split_volume(volume);
    fs::path parentPath = fs::current_path().parent_path();

    string container_url(parentPath.c_str());
    container_url += "/dockerHome/aufs/"+container_ID+"/mnt"+volume_urls[CONTAINER_INDEX];

    if(umount(container_url.c_str()) != 0){
        print_error(__func__,"UMOUNT_ERROR");
    }
}
//end volume

void RunCommand::new_work_place(const string& container_id){
    // aufs_sub_func::create_read_only_layer(*image_name_ptr,container_id);
    aufs_sub_func::create_write_layer(*image_name_ptr,container_id);
    aufs_sub_func::create_mnt_layer(*image_name_ptr,container_id);
    
    string chmod_command = "sudo chmod -R 777 "+aufs_sub_func::AUFS_PATH+*image_name_ptr+container_id;
    system(chmod_command.c_str());

    aufs_sub_func::mount_aufs2mnt(*image_name_ptr,container_id);

    if(short_opt_map.find('v') == short_opt_map.end()){
        return ;
    }
    
    auto volume_urls = split_volume(short_opt_map['v']);
    if(volume_urls.size() != 2 || 
        volume_urls[HOME_INDEX].compare("") == 0 ||
        volume_urls[CONTAINER_INDEX].compare("") == 0){
        print_error(__func__,VOLUME_PARA_ERROR);
    }
    
    mount_volume(volume_urls,*image_name_ptr+container_id);
}

void RunCommand::delete_work_place(const string& container_id){
    aufs_sub_func::unmount_aufs(*image_name_ptr+container_id);
    aufs_sub_func::rm_write_dir(*image_name_ptr+container_id);
}

int runClone(void *args){
    using namespace run_sub_func;
    // cout<<__func__<<endl;
    auto source = reinterpret_cast<RunSource*>(args);

    // source->print_source_message();

    // set_rootdir(source->image_name);
    // pivot_root(source->image_name);

    //有-t选项才绑定shell，并改变pivot_root
    if(source->short_opt_map.find('t') != source->short_opt_map.end()){
        pivot_root(source->image_name,source->image_id);
        mount_proc();
        start_bash(source->bash_name);
    }

    if(source->short_opt_map.find('d') != source->short_opt_map.end()){
    //要后台运行的话加一个死循环
        setsid();
        while(1) ;
            sleep(1);    
    }
}

void RunCommand::run_image(const string& container_id,const string& command){
    // print_message();
    
    static const int STACK_SIZE= (1024 * 1024);
    char *stack;                    /* Start of stack buffer */
    char *stackTop; 
    stack = (char*)mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if(stack == MAP_FAILED){
        print_error(__func__,STACK_ALLOC_ERROR);
    }
    stackTop = stack + STACK_SIZE; 

    RunSource* source = nullptr;
    //若没有-t选项，没有shell_type_ptr为空的
    if(short_opt_map.find('t') != short_opt_map.end()){
        source = new RunSource(*image_name_ptr,*shell_type_ptr,container_id,long_opt_map,short_opt_map);
    }else{
        source = new RunSource(*image_name_ptr,container_id,long_opt_map,short_opt_map); 
    }

    //调转到aufs下的容器目录
    new_work_place(container_id);


    pid_t childPid = clone(runClone, stackTop,
                CLONE_NEWIPC|
                CLONE_NEWNS |
                CLONE_NEWUTS|
                CLONE_NEWPID|
                CLONE_NEWNET|
                SIGCHLD,
                source);

    //元数据的操作
    string status = (short_opt_map.find('t') != short_opt_map.end() ? "TR" : "R");
    ContainerDataMessage container(*image_name_ptr,get_time_now(),*image_name_ptr+container_id,command,status,childPid);
    container.write_metedata();
    // container.get_metedata().print_container_head();
    // container.print_metedate();

    vector<string> limit_v;

    init_limit_v(limit_v);

    // for(auto val:limit_v)
    //  cout<<val<<endl;

    DockerLimit docker_limit(limit_v[0],limit_v[1],limit_v[2]);
    docker_limit.limit(childPid,*image_name_ptr+container_id);



    if(short_opt_map.find('t') != short_opt_map.end()){
        waitpid(childPid,nullptr,0);
        if(childPid){
            //这是父进程进行的操作
            string data_path = CONTAINER_METEDATA_PATH + *image_name_ptr+container_id+".txt";
            
            // std::cout<<data_path<<endl;
            
            delete_file(data_path.c_str());
            docker_limit.release();
            if(short_opt_map.find('v') != short_opt_map.end()){
                unmoumt_volume(short_opt_map['v'],*image_name_ptr+container_id);
            }

        // cout<<container_id<<endl;

            delete_work_place(container_id);
        }
    }
}

void RunCommand::run(int argc,char **argv,const string& container_id){
    get_option(argc,argv);
    // print_message();

    if(long_opt_map.find("help") != long_opt_map.end() || 
        short_opt_map.find('h') != short_opt_map.end()){
        help();
    }
    else{
        string command;
        for(int i = 0;i < argc;++i)
            (command += argv[i]) += " ";
        run_image(container_id,command);
    }
}  

RunCommand::~RunCommand(){
    free(short_opts);
    free(long_opts);
    if(image_name_ptr){
        delete image_name_ptr;
    }
    if(shell_type_ptr){
        delete shell_type_ptr;
    }
};

//end RunCommand

//ContainersCommand SubFunc
void print_container_message(){
    string container_path = "../dockerHome/metedata/container/";
    auto all_files = get_dir_all(container_path.c_str());
    ContainerMeteData::print_container_head();
    for(auto& val:all_files){
        auto temp_path = container_path + val;
        ContainerDataMessage messages(temp_path);
        messages.print_metedate();
    }
}

/*
    ContainersCommand Function
*/   

inline void ContainersCommand::help(){
    cout<<"Format:\n"
        <<"\tdocker containers [-opt] [para]"<<endl;
    cout<<"Containters Options:"<<endl;
    cout<<"\t-h,--help               Print the message"<<endl;
    cout<<"\t-t.--test               Use to test the function"<<endl;
    exit(0);
}

ContainersCommand::ContainersCommand():CommandInterface(
    ContainersCommandPara::stl_short_opt_def,
    ContainersCommandPara::stl_long_opt_def){
};

void ContainersCommand::get_option(int argc,char **argv){
    // cout<<"func = "<<__func__<<endl;
    int ch;
    int index;
    while((ch = getopt_long(argc,argv,ContainersCommandPara::short_opt_def,ContainersCommandPara::long_opt_def,&index)) != -1){
        // cout<<(char)ch<<endl;
        switch (ch){
            case 'h':
                help();
                break;
            case 't':
                cout<<"test successful"<<endl;
                break;
            default:
                print_error(__func__,OPTION_NOT_EXIST);
                break;
        }
    }
};

void ContainersCommand::run(int argc,char **argv,const string& container_id){
    get_option(argc,argv);

    print_container_message();
}

ContainersCommand::~ContainersCommand(){
    free(short_opts);
    free(long_opts);
}

//end ContainersCommand Function


/*
    读取镜像信息子函数
*/

void print_images_message(){
    string image_path = "../dockerHome/root/image/";
    auto all_files = get_dir_all(image_path.c_str());
    ImageMessage::print_head();
    for(auto& val:all_files){
        auto temp = image_path + val;
        ImageMessage message(image_path,val);
        message.print_messages();
    }

    string tar_image_path = "../dockerHome/root/tar_image/";
    all_files = get_dir_all(tar_image_path.c_str());
    for(auto& val:all_files){
        auto temp = tar_image_path + val;
        ImageMessage message(tar_image_path,val,true);
        message.print_messages();
    }
}

//End SubFunc
 
/*
    ImageCommand Function
*/   

inline void ImagesCommand::help(){
    cout<<"Format:\n"
        <<"\tdocker images [-opt] [para]"<<endl;
    cout<<"Image Options:"<<endl;
    cout<<"\t-h,--help               Print the message"<<endl;
    cout<<"\t-t.--test               Use to test the function"<<endl;
    exit(0);
}

ImagesCommand::ImagesCommand():CommandInterface(
    ImagesCommandPara::stl_short_opt_def,
    ImagesCommandPara::stl_long_opt_def){
};

void ImagesCommand::get_option(int argc,char **argv){
    // cout<<"func = "<<__func__<<endl;
    int ch;
    int index;
    while((ch = getopt_long(argc,argv,RunCommandPara::short_opt_def,RunCommandPara::long_opt_def,&index)) != -1){
        // cout<<(char)ch<<endl;
        switch (ch){
            case 'h':
                help();
                break;
            case 't':
                cout<<"test successful"<<endl;
                break;
            default:
                print_error(__func__,OPTION_NOT_EXIST);
                break;
        }
    }
};

void ImagesCommand::run(int argc,char **argv,const string& container_id){
    get_option(argc,argv);
    print_images_message();
}

ImagesCommand::~ImagesCommand(){
    free(short_opts);
    free(long_opts);
}

//end ImageCommand Function

/*
    CommitCommand Function
*/

inline void CommitCommand::help(){
    cout<<"Format:\n"<<"\tdocker commit [-opt] [para] container_id"<<endl;
    cout<<"Commit Options:"<<endl;
    cout<<"\t-h,--help               Print the message"<<endl;
    cout<<"\t-T,--test               Use to test the function"<<endl;
    cout<<"\t-n,--name               Use to define the name of saving image"<<endl;
    cout<<"\t-t                      saving the iamge as tar"<<endl;
};

void CommitCommand::get_option(int argc,char **argv){
    char ch;
    int index;
    while((ch = getopt_long(argc,argv,CommitCommandPara::short_opt_def,CommitCommandPara::long_opt_def,&index)) != -1){
        // cout<<ch<<endl;
        if(short_opt_set.find(ch) != short_opt_set.end())
            short_opt_map[ch] = (optarg? optarg : "NULL");
        else if(long2short_opt_map.find(ch) != long2short_opt_map.end())
            long_opt_map[long2short_opt_map[ch]] = (optarg? optarg : "NULL");
        else{
            help();
            print_error(__func__,OPTION_NOT_EXIST);
        }
    }
    container_id = argv[argc-1];
    //非help命令情况下，必须有container_id的参数
    if(!container_id.compare("commit") && short_opt_map.find('h') == short_opt_map.end())
        print_error(__func__,"LACK_CONTAINER_ID");
    // cout<<container_id<<endl;
};

void CommitCommand::save(){
    fs::path parentPath = fs::current_path().parent_path();

    string container_url(parentPath.c_str());

    // string container_path = container_url+"/dockerHome/aufs/"+container_id;
    // cout<<container_path<<endl;

    // if(access(container_id.c_str(),F_OK) != 0)
    //     print_error(__func__,"ERROR_CONTAINER_ID");
    container_url += "/dockerHome/aufs/"+container_id+"/mnt";

    string saving_url(parentPath.c_str());
    //包装成tar还是普通文件夹
    if(short_opt_map.find('t') != short_opt_map.end()){
        saving_url+= "/dockerHome/root/tar_image/"+short_opt_map['n']+".tar ";
            // cout<<container_url<<endl<<saving_url<<endl;
        string tar_command = "sudo tar -Pczf "+saving_url+" -C "+container_url+" .";
        system(tar_command.c_str());
    }else{
        saving_url+= "/dockerHome/root/image/"+short_opt_map['n'];
        if(access(saving_url.c_str(),F_OK) == 0)
            print_error(__func__,"PATH_EXSIT_NOW");
        if(mkdir(saving_url.c_str(),0777) != 0)
            print_error(__func__,MKDIR_ERROR);
        string cp_command = "sudo cp -r "+container_url+"/* "+saving_url;
        system(cp_command.c_str());
    }
};

CommitCommand::CommitCommand():CommandInterface(
    RunCommandPara::stl_short_opt_def,
    RunCommandPara::stl_long_opt_def){
    
    short_opt_set = unordered_set<char>(CommitCommandPara::stl_short_opt_def.begin(),CommitCommandPara::stl_short_opt_def.end());

    for(int i = 0;i < CommitCommandPara::stl_long_opt_def.size()-1;++i){
        long2short_opt_map[(char)CommitCommandPara::stl_long_opt_def[i].val] = CommitCommandPara::stl_long_opt_def[i].name;
    }    
};

void CommitCommand::run(int argc,char **argv,const string& container_id){
    get_option(argc,argv);
    if(short_opt_map.find('h') != short_opt_map.end())
        help();
    else
        save();
};

CommitCommand::~CommitCommand(){

};

//End Commit Function

/*
    ImageMessage Func
*/
ImageMessage::ImageMessage(const string& image_path,const string& image_name){
    this->image_name = image_name;
    this->create_time =get_dir_time((image_path+image_name).c_str());
    this->tag = "latest";
    this->size = get_dir_size_str((image_path+image_name).c_str());
}

ImageMessage::ImageMessage(const string& image_path,const string& image_name,bool is_tar){
    this->image_name = image_name;
    this->create_time =get_dir_time((image_path+image_name).c_str());
    this->tag = "latest";
    this->size = get_file_size((image_path+image_name).c_str());
}

void ImageMessage::print_head()
{   cout<<setprecision(2)<<setiosflags(ios::left);
    cout<<setw(20)<<"IMAGE_NAME"
        <<setw(10)<<"TAG"
        <<setw(30)<<"CREATED_TIME"
        <<setw(15)<<"SIZE"<<endl;
}

void ImageMessage::print_messages(){
    cout<<std::left<<setw(20)<<image_name
        <<std::left<<setw(10)<<tag
        <<std::left<<setw(30)<<create_time
        <<std::left<<setw(15)<<size<<endl;
}

//end ImageMessage Func

/*
    Stop Func
*/
inline void StopContainer::help(){
    cout<<"Format:\n"
        <<"\tdocker stop [-opt] container_id"<<endl;
    cout<<"Containters Options:"<<endl;
    cout<<"\t-h,--help               Print the message"<<endl;
    cout<<"\t-t.--test               Use to test the function"<<endl;
    exit(0);
}
    
void StopContainer::get_option(int argc,char **argv){
    int ch;
    int index;
    while((ch = getopt_long(argc,argv,StopCommandPara::short_opt_def,StopCommandPara::long_opt_def,&index)) != -1){
        // cout<<(char)ch<<endl;
        switch (ch){
            case 'h':
                help();
                break;
            case 't':
                cout<<"test successful"<<endl;
                break;
            default:
                print_error(__func__,OPTION_NOT_EXIST);
                break;
        }
    }

}
    
StopContainer::StopContainer():CommandInterface(
    StopCommandPara::stl_short_opt_def,
    StopCommandPara::stl_long_opt_def){
}

void StopContainer::run(int argc,char **argv,const string& container_id){
    get_option(argc,argv);
    
    string id_para = argv[argc-1];

    string path = "../dockerHome/metedata/container/" + id_para + ".txt";

    // cout<<path<<endl;

    if(access(path.c_str(),F_OK) != 0)
        print_error(__func__,"CONTAINER_NOT_EXSIT");

    ContainerDataMessage messages(path);
    pid_t container_pid = messages.get_pid();
    
    cout<<container_pid<<endl;

    kill(container_pid,SIGSTOP);

    messages.change_status("S");

    // messages.print_metedate();

    messages.write_metedata();
}

StopContainer::~StopContainer(){

}

//end stop command

/*
    start command
*/


inline void StartContainer::help(){
    cout<<"Format:\n"
        <<"\tdocker start [-opt] container_id"<<endl;
    cout<<"Containters Options:"<<endl;
    cout<<"\t-h,--help               Print the message"<<endl;
    cout<<"\t-t.--test               Use to test the function"<<endl;
    exit(0);
}

void StartContainer::get_option(int argc,char **argv){
    int ch;
    int index;
    while((ch = getopt_long(argc,argv,StartCommandPara::short_opt_def,StartCommandPara::long_opt_def,&index)) != -1){
        // cout<<(char)ch<<endl;
        switch (ch){
            case 'h':
                help();
                break;
            case 't':
                cout<<"test successful"<<endl;
                break;
            default:
                print_error(__func__,OPTION_NOT_EXIST);
                break;
        }
    }
}

StartContainer::StartContainer():CommandInterface(
    StartCommandPara::stl_short_opt_def,
    StartCommandPara::stl_long_opt_def){
}

void StartContainer::run(int argc,char **argv,const string& container_id){
    get_option(argc,argv);
    
    string id_para = argv[argc-1];

    string path = "../dockerHome/metedata/container/" + id_para + ".txt";

    // cout<<path<<endl;

    if(access(path.c_str(),F_OK) != 0)
        print_error(__func__,"CONTAINER_NOT_EXSIT");

    ContainerDataMessage messages(path);
    pid_t container_pid = messages.get_pid();
    
    cout<<container_pid<<endl;

    kill(container_pid,SIGCONT);

    messages.change_status("R");

    // messages.print_metedate();

    messages.write_metedata();
}

StartContainer::~StartContainer(){
    
}

/*
    exec command
*/

inline void ExecContainer::help(){
    cout<<"Format:\n"
        <<"\tdocker exec [-opt] container_id shell_name"<<endl;
    cout<<"Containters Options:"<<endl;
    cout<<"\t-h,--help               Print the message"<<endl;
    cout<<"\t-t.--test               Use to test the function"<<endl;
    exit(0);
}

void ExecContainer::get_option(int argc,char **argv){
    int ch;
    int index;
    while((ch = getopt_long(argc,argv,ExecCommandPara::short_opt_def,ExecCommandPara::long_opt_def,&index)) != -1){
        // cout<<(char)ch<<endl;
        switch (ch){
            case 'h':
                help();
                break;
            case 't':
                cout<<"test successful"<<endl;
                break;
            default:
                print_error(__func__,OPTION_NOT_EXIST);
                break;
        }
    }
}

ExecContainer::ExecContainer():CommandInterface(
    ExecCommandPara::stl_short_opt_def,
    ExecCommandPara::stl_long_opt_def){
}

void ExecContainer::run(int argc,char **argv,const string& container_id){
    get_option(argc,argv);
    
    string id_para = argv[argc-2];

    string path = "../dockerHome/metedata/container/" + id_para + ".txt";

    // cout<<path<<endl;

    if(access(path.c_str(),F_OK) != 0)
        print_error(__func__,"CONTAINER_NOT_EXSIT");

    ContainerDataMessage messages(path);
    pid_t container_pid = messages.get_pid();
    
    cout<<container_pid<<endl;

    kill(container_pid,SIGCONT);

    messages.change_status("RT");

    // messages.print_metedate();

    messages.write_metedata();

    //启动终端部分，改改再

    string temp_shell = argv[argc-1];
    run_sub_func::set_rootdir(messages.get_container_id());
    run_sub_func::mount_proc();
    run_sub_func::start_bash(temp_shell);
}
ExecContainer::~ExecContainer(){
    
}

//end exec command

/*
    kill command
*/

inline void KillContainer::help(){
    cout<<"Format:\n"
        <<"\tdocker kill [-opt] container_id"<<endl;
    cout<<"Containters Options:"<<endl;
    cout<<"\t-h,--help               Print the message"<<endl;
    cout<<"\t-t.--test               Use to test the function"<<endl;
    exit(0);
}

void KillContainer::get_option(int argc,char **argv){
    int ch;
    int index;
    while((ch = getopt_long(argc,argv,KillCommandPara::short_opt_def,KillCommandPara::long_opt_def,&index)) != -1){
        // cout<<(char)ch<<endl;
        switch (ch){
            case 'h':
                help();
                break;
            case 't':
                cout<<"test successful"<<endl;
                break;
            default:
                print_error(__func__,OPTION_NOT_EXIST);
                break;
        }
    }
}

KillContainer::KillContainer():CommandInterface(
    KillCommandPara::stl_short_opt_def,
    KillCommandPara::stl_long_opt_def){
}

void KillContainer::run(int argc,char **argv,const string& container_id){
    get_option(argc,argv);
    
    string id_para = argv[argc-1];

    string path = "../dockerHome/metedata/container/" + id_para + ".txt";

    // cout<<path<<endl;

    if(access(path.c_str(),F_OK) != 0)
        print_error(__func__,"CONTAINER_NOT_EXSIT");

    ContainerDataMessage messages(path);
    pid_t container_pid = messages.get_pid();
    
    cout<<container_pid<<endl;

    kill(container_pid,SIGKILL);

    /*
        添加删除一堆目录的操作
    */

//  vector<string> limit_v;

//  init_limit_v(limit_v);

//  for(auto val:limit_v)
//  cout<<val<<endl;

//  DockerLimit docker_limit(limit_v[0],limit_v[1],limit_v[2]);
//  docker_limit.limit(childPid,*image_name_ptr+container_id);

    string data_path = CONTAINER_METEDATA_PATH + messages.get_container_id() +".txt";
            
//  std::cout<<data_path<<endl;
            
    delete_file(data_path.c_str());
            
//  docker_limit.release();
    
    if(short_opt_map.find('v') != short_opt_map.end()){
        unmoumt_volume(short_opt_map['v'],messages.get_container_id());
    }

    aufs_sub_func::unmount_aufs(messages.get_container_id());
    aufs_sub_func::rm_write_dir(messages.get_container_id());

}

KillContainer::~KillContainer(){
    
}

//end kill command