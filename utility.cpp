#include"utility.hpp"

char * string2char(const string& s){
    auto opt = s.c_str();
    char* ch_p = (char*)malloc(s.size()+1);
    memcpy(ch_p,opt,s.size()+1);
    return ch_p;
}

string get_image_path(const string& image_name){
    return Path::IMAGE_PATH + "/" + image_name;
}

void is_valid(int argc,char **argv,oper_type& type){
    if(argc <= 1){
        type = NO_COMMAND;
        print_error(__func__,type);
    }
    auto commands = CommandType::commands_options;
    for(string& val:commands)
        if(!val.compare(argv[1])){
            type = SUCCESS;
            return ;
        }
    type = COMMAND_NOT_EXIST;
    print_error(__func__,type);
}

string hex2str(time_t num)
{
    std::vector<char> char_map{'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    string str;
    while(num){
        str.push_back(char_map[num%16]);
        num /= 16;
    }
    srand((int)time(NULL));
    while(str.size()<11){
        int index = rand()%15;
        str.push_back(char_map[index]);
    }
    str.push_back('_');
    reverse(str.begin(),str.end());
    return str;
}

size_t get_dir_size(const char *dir){
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    long long int totalSize=0;
 
    if ((dp = opendir(dir)) == NULL)
    {
        print_error(__func__,"PATH_NOT_EXSIT");
    }
     
     //先加上自身目录的大小
    lstat(dir, &statbuf);
    totalSize+=statbuf.st_size;
 
    while ((entry = readdir(dp)) != NULL)
    {
        char subdir[256];
        sprintf(subdir, "%s/%s", dir, entry->d_name);
        lstat(subdir, &statbuf);
         
        if(S_ISDIR(statbuf.st_mode))
        {
            if (strcmp(".", entry->d_name) == 0 ||
                strcmp("..", entry->d_name) == 0)
            {
                continue;
            }
 
            long long int subDirSize = get_dir_size(subdir);
            totalSize+=subDirSize;
        }
        else
        {
            totalSize+=statbuf.st_size;
        }
    }
 
    closedir(dp);   
    
    return totalSize;
}

string get_dir_size_str(const char *dir){
    auto total_size = get_dir_size(dir);
    std::stringstream ss;
    
    int div = 1;
    string units = "B";
    if(total_size > 1000000000){
        div = 1000000000;
        units = "GB";
    }else if(total_size > 1000000){
        div = 1000000;
        units = "MB";
    }else{

    }

    ss<<std::fixed<<std::setprecision(2)<<1.0*total_size/div;
    string size_str = ss.str();
    size_str += units;
    return size_str;
}

string get_dir_time(const char *dir){
    struct stat result;
    if(stat(dir, &result)==0)
    {
        auto mod_time = result.st_mtime;
        time(&mod_time);
        string time = asctime(gmtime(&mod_time));
        time.resize(time.size()-1);
        return time;
    }else
        print_error(__func__,"DIR_ERROR");
}

std::vector<string> get_dir_all(const char *dir){
    std::vector<string> all_files;
    DIR* d = opendir(dir);
    if (d == NULL){
        print_error(__func__,"DIR_ERROR");
    }

    struct dirent* entry;
    while ( (entry=readdir(d)) != NULL){
        string temp = entry->d_name;
        if(temp.compare(".") && temp.compare(".."))
           all_files.emplace_back(temp);
    }
    closedir(d);
    return all_files;
}

string get_file_size(const char* fname)
{
    struct stat statbuf;
    if(stat(fname,&statbuf) != 0)
        print_error(__func__,"FILE_NOT_EXSIT");
    auto total_size = statbuf.st_size;
    std::stringstream ss;
    
    int div = 1;
    string units = "B";
    if(total_size > 1000000000){
        div = 1000000000;
        units = "GB";
    }else if(total_size > 1000000){
        div = 1000000;
        units = "MB";
    }else{

    }

    ss<<std::fixed<<std::setprecision(2)<<1.0*total_size/div;
    string size_str = ss.str();
    size_str += units;
    return size_str;
}


string get_time_now()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep));
    return tmp;
}

string delete_file(const string& path){
    if(unlink(path.c_str()) != 0){
            print_error(__func__,RMDIR_ERROR);
        }
}

//分离metedate
std::vector<string> split(const string& s, char spliter){
    string tmp;
    std::vector<string> ans;
    for(int i = 0; i < s.size();i++){
        if(s[i]!=spliter) {
            tmp.push_back(s[i]);
        }
        else {
            ans.push_back(tmp);
            tmp = string();
        }
    }
    if(s[s.size()-1]!= spliter) ans.push_back(tmp);
    return ans;
}