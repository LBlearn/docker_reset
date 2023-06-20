# docker_reset
项目可以使用命令行的方式模拟一个容器的运行，现已实现run、images、containers、commit命令

+ run命令的实现参数有
  - -it：实现容器绑定用户传入的shell运行
  - --volume、-v：绑定容器卷
  - --memory、-m：实现内存限制
  - --cpuset、-c：实现cpu限制
  - --cpushare、-C：实现cpushare限制
  - 注：run中实现了namespace和cgroup机制，并通过pivot_root系统调用、AUFS系统挂载将容器的运行从镜像未知转移到dockerHome的aufs目录，每一个容器都会在aufs下新建一个目录，目录下的mnt为挂载目录，即容器
  的运行时根目录，write_layer是进行改变时写的目录（AUFS机制） 
+ images命令：查看现存的镜像
  - 注：镜像都放在dockerHome/image目录下，tar_image存放的是tar镜像
+ commit命令：实现将指定的容器ID打包成镜像
  - -t：打包成tar包
  - --name、-n：指定打包镜像的名字
+ containers命令：查看现存的容器
