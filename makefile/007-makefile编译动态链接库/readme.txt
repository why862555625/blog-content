6 Makefile 中编译动态链接库 .dll   .so  库文件
动态链接库：不会把代码编译到二进制文件中，而是在运行时才去加载，所以只需要维护一个地址
-fPIC 产生位置无关的代码
-shared 共享
-l(小L) 指定动态库
-I(大i) 指定头文件目录，默认当前目录
-L 手动指定库文件搜索目录，默认只链接共享目录

动态链接库  好处是程序可以和库文件分离，可以分别发版，然后库文件可以被多处共享
动态  运行时才去加载  动态加载
链接  指库文件和二进制程序分离，用某种特殊手段维护两者之间的关系
库    库文件 .dll   .so

SoTest.cpp   libSoTest.so

g++ -shared -fPIC SoTest.cpp -o libSoTest.so
g++ -lSoTest -L./ test.cpp -o test


g++ -lSoTest -L./001  main.cpp -o main

编译时指定了要依赖的动态库，但是运行时，找不到.so文件
dyld: Library not loaded: libSoTest.so
  Referenced from: /Users/jose/CLionProjects/0303/./main
  Reason: image not found
zsh: abort      ./main

1
动态库编译完成之后要发布，否则程序运行时找不到

Linux 默认动态库路径配置文件
/etc/ld.so.conf     /etc/ld.so.conf.d/*.conf

/usr/lib
/usr/local/lib


2 运行时手动指定动态库目录
 DYLD_LIBRARY_PATH=./001
export DYLD_LIBRARY_PATH

LD_LIBRARY_PATH=./001
export LD_LIBRARY_PATH
