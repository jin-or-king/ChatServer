# ChatServer
基于muduo和redis实现的使用njinx tcp负载均衡的集群聊天服务器

编译方式
cd build 
rm -rf *
cmake ..
make

cd ./bin
./ChatServer <ip> <port>
./ChatClient <ip> <port>
