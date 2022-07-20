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













Makefile中编译静态链接库  .lib  .a
静态链接库：会把库中的代码编译到二进制文件中，当程序编译完成后，该库文件可以删除
而动态链接库不行，动态链接库必须与程序同时部署，还要保证程序能加载得到库文件

与动态库相比，静态库可以不用部署(己经被加载到程序里面了)，而且运行时速度更快(因为不用去加载)
但是会导致程序体积更大，并且库中的内容如果有更新，则需要重新编译生成程序

打个比方，你要出去玩，在路上要走三天三夜，
动态库就是，你背着一袋粮食上路，饿了就吃一口
静态库就是，你吃饱了出发，不用带粮食

aTest.cpp   libaTest.a

g++ -c aTest.cpp -o aTest.o
ar -r libaTest.a aTest.o

g++ -lSoTest -L./001 -laTest -L./002 main.cpp -o main

objdump -DC main>main.txt
