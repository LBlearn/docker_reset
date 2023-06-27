#ifndef OPTION_NAMESPACE_HPP
#define OPTION_NAMESPACE_HPP

#include<string>
#include<vector>
#include<getopt.h>

namespace CommandType{
    static std::vector<std::string> commands_options{"run","images","containers","commit","--help","stop","start","kill","exec"}; 
};

namespace RunCommandPara{
    static char *short_opt_def = "htidm:c:C:v:";
    static option long_opt_def[] = {
        {"help",no_argument,nullptr,'h'},
        {"memory",required_argument,nullptr,'m'},
        {"cpuset",required_argument,nullptr,'c'},
        {"cpushare",required_argument,nullptr,'C'},
        {"volume",required_argument,nullptr,'v'},
        {0,0,0,0}
        };
    
    static std::string stl_short_opt_def = "htidm:c:C:v:";
    static std::vector<option> stl_long_opt_def = {
        {"help",no_argument,nullptr,'h'},
        {"test",no_argument,nullptr,'T'},
        {"memory",required_argument,nullptr,'m'},
        {"cpuset",required_argument,nullptr,'c'},
        {"cpushare",required_argument,nullptr,'C'},
        {"volume",required_argument,nullptr,'v'},
        {0,0,0,0}
        };
};

namespace CommitCommandPara{
    static char *short_opt_def = "htn:T";
    static option long_opt_def[] = {
        {"help",no_argument,nullptr,'h'},
        {"name",required_argument,nullptr,'n'},
        {"test",no_argument,nullptr,'T'},
        {0,0,0,0}
        };
    
    static std::string stl_short_opt_def = "htn:T";
    static std::vector<option> stl_long_opt_def = {
        {"help",no_argument,nullptr,'h'},
        {"name",required_argument,nullptr,'n'},
        {"test",no_argument,nullptr,'T'},
        {0,0,0,0}
        };
};

namespace ImagesCommandPara{
    static char *short_opt_def = "ht";
    static option long_opt_def[] = {
        {"help",no_argument,nullptr,'h'},
        {"test",no_argument,nullptr,'t'},
        {0,0,0,0}
        };
    
    static std::string stl_short_opt_def = "ht";
    static std::vector<option> stl_long_opt_def ={
        {"help",no_argument,nullptr,'h'},
        {"test",no_argument,nullptr,'T'},
        {0,0,0,0}
        };
};

namespace ContainersCommandPara{
    static char *short_opt_def = "ht";
    static option long_opt_def[] = {
        {"help",no_argument,nullptr,'h'},
        {"test",no_argument,nullptr,'t'},
        {0,0,0,0}
        };
    
    static std::string stl_short_opt_def = "ht";
    static std::vector<option> stl_long_opt_def ={
        {"help",no_argument,nullptr,'h'},
        {"test",no_argument,nullptr,'T'},
        {0,0,0,0}
        };
};

namespace StopCommandPara{
    static char *short_opt_def = "ht";
    static option long_opt_def[] = {
        {"help",no_argument,nullptr,'h'},
        {"test",no_argument,nullptr,'t'},
        {0,0,0,0}
        };
    
    static std::string stl_short_opt_def = "ht";
    static std::vector<option> stl_long_opt_def ={
        {"help",no_argument,nullptr,'h'},
        {"test",no_argument,nullptr,'T'},
        {0,0,0,0}
        };
};

namespace StartCommandPara{
    static char *short_opt_def = "ht";
    static option long_opt_def[] = {
        {"help",no_argument,nullptr,'h'},
        {"test",no_argument,nullptr,'t'},
        {0,0,0,0}
        };
    
    static std::string stl_short_opt_def = "ht";
    static std::vector<option> stl_long_opt_def ={
        {"help",no_argument,nullptr,'h'},
        {"test",no_argument,nullptr,'T'},
        {0,0,0,0}
        };
};

namespace ExecCommandPara{
    static char *short_opt_def = "ht";
    static option long_opt_def[] = {
        {"help",no_argument,nullptr,'h'},
        {"test",no_argument,nullptr,'t'},
        {0,0,0,0}
        };
    
    static std::string stl_short_opt_def = "ht";
    static std::vector<option> stl_long_opt_def ={
        {"help",no_argument,nullptr,'h'},
        {"test",no_argument,nullptr,'T'},
        {0,0,0,0}
        };
};


namespace KillCommandPara{
    static char *short_opt_def = "ht";
    static option long_opt_def[] = {
        {"help",no_argument,nullptr,'h'},
        {"test",no_argument,nullptr,'t'},
        {0,0,0,0}
        };
    
    static std::string stl_short_opt_def = "ht";
    static std::vector<option> stl_long_opt_def ={
        {"help",no_argument,nullptr,'h'},
        {"test",no_argument,nullptr,'T'},
        {0,0,0,0}
        };
};
#endif