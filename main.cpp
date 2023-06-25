#include"docker.hpp"

int main(int argc,char **argv){
    Docker docker;
    oper_type type;
    //验证参数是否有效
    is_valid(argc,argv,type);
    //初始化docker参数
    docker.init_docker(argc,argv);
    //启动容器
    docker.start();
}