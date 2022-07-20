# makefile

## c/c++编译过程

1. 预处理    gcc -E main.cpp >main.ii
1. 编译       gcc -S main.ii 得到名为 main.s 的汇编文件
1. 汇编       gcc -c main.s 得到main.o (obj)的二进制文件
1. 链接       gcc -lstdc++ main.o 得到名为a.out的可执行文件

## makefile 基本语法

目标： 依赖

tab   命令

目标：一般指要编译的目标，也可以是一个动作。

依赖： 执行当前目标所要依赖的选项，包括其他目标，某个具体文件或者库等。一个目标可以有多个依赖。

命令： 该目标下要执行的具体命令，可以没有，也可以有多条。多条时  每条命令占一行。



## makefile 变量

##### 系统变量

$* 不包含扩展名的目标文件名称

$+ 所有的依赖文件，以空格分隔

$< 表示规则中的第一个条件

$? 所有时间戳比目标文件晚的依赖，以空格分隔

$@ 目标文件的完整名称

$^ 所有不重复的依赖文件，以空格分隔

$% 如果目标是规党成员，则该变量表示目标的归档成员名称

##### 系统常量(make -p)

AS   汇编程序的名称，默认为as

CC  c编译器名称  默认为cc 

CPP c预编译器名称 默认为 cc -E 

CXX  c++ 编译器名称 默认为g++

RM  文件删除程序别名  默认为rm -f

LDFLAGS  

**CFLAGS**： 指定[头文件](https://so.csdn.net/so/search?q=头文件&spm=1001.2101.3001.7020)（.h文件）的路径，如：CFLAGS=-I/usr/include -I/path/include。同样地，安装一个包时会在安装路径下建立一个include目录，当安装过程中出现问题时，试着把以前安装的包的include目录加入到该变量中来。

**LDFLAGS**：gcc 等编译器会用到的一些优化参数，也可以在里面指定库文件的位置。用法：LDFLAGS=-L/usr/lib -L/path/to/your/lib。每安装一个包都几乎一定的会在安装目录里建立一个lib目录。如果明明安装了某个包，而安装另一个包时，它愣是说找不到，可以抒那个包的lib路径加入的LDFALGS中试一下。

**LIBS**：告诉链接器要链接哪些库文件，如LIBS = -lpthread -liconv

##### 自定义变量

定义： 变量名 = value

使用：$(变量名)

### makefile 中的伪目标和模式匹配

伪目标 .PHONY:clean  （防止有重名文件   直接执行不会判断目标是否存在）

​	声明目标为伪目标之后，makefile将不会判断目标是否存在或该目标是否需要更新



模式匹配 %目标:%依赖
目标和依赖相同部份，可用%来通配
%.o:%.cpp

#wildcard       $(wildcard  ./*.cpp) 获取当前目录下所有的.cpp文件
#patsubst         $(patsubst  %.cpp，%.o，./*.cpp) 将对应的cpp 文件名替换成 .o 文件名



### Makefile 中编译动态链接库 .dll   .so  库文件

动态链接库：不会把代码编译到二进制文件中，而是在运行时才去加载，所以只需要维护一个地址
-fPIC 产生位置无关的代码
-shared 共享
-l(小L) 指定动态库
-I(大i) 指定头文件目录，默认当前目录
-L 手动指定库文件搜索目录，默认只链接共享目录

**动态链接库**   好处是程序可以和库文件分离，可以分别发版，然后库文件可以被多处共享
**动态**  运行时才去加载  动态加载
**链接**  指库文件和二进制程序分离，用某种特殊手段维护两者之间的关系
库    库文件 .dll   .so

**编译指令** g++ -shared -fPIC SoTest.cpp -o libSoTest.so



动态库编译完成之后要发布，否则程序运行时找不到

1. 将文件放到指定目录

Linux 默认动态库路径配置文件
/etc/ld.so.conf     /etc/ld.so.conf.d/*.conf 	/usr/lib	/usr/local/lib

2.  运行时手动指定动态库目录

   **mac:**

   DYLD_LIBRARY_PATH=动态库目录
   export DYLD_LIBRARY_PATH

   **linux:**

   LD_LIBRARY_PATH=动态库目录
   export LD_LIBRARY_PATH



## Makefile 静态链接库

静态链接库： 会把库中的代码编译到二进制文件中，当程序编译完成后，该库的文件可以删除。（会导致程序变大）



## makefile 中通用部分做公共头文件

导入  include ../makefile

makefile  中，都是先展开所有变量在调用指令。

=  赋值，终值，不管写在哪里都取这个值。

:=  也是赋值，但是只受前行影响，不会收到后面的赋值影响。



## makefile 调用shell 命令

```makefile

FILE=abc
A:=$(shell ls ../)
B:=$(shell pwd)
C :=$(shell if [ ! -f $(FILE) ];then touch $(FILE);fi;)
a:
	echo $(A)
	echo $(B)
	echo $(C)

clean:
	$(RM) $(FILE)
```



## makefile 嵌套调用

```makefile
	make -C 工作目录  => 调用工作目录下的Makefile
	make -C ./001  
```



## makefile 条件判断 循环

ifeq  判断是否相等，相等返回true，不相等返回false

ifneq   判断是否不相等，相等返回true，不相等返回false

ifdef 判断变量是否存在，存在返回true，不存在返回false

Ifndef 判断变量是否不存在，不存在返回true，存在返回false

ifeq，ifneq 与条件之间要有空格，不然会报错



命令行传参  make -f Makefile FLAG=456  如果有Makefile ，则可写成 make FLAG=456

  $(shell for v in $(TARGET); do touch *$$*v-txt;done)



## makefile 自定义函数

定义

```makefile
define FUNC1

	echo $(0) $(1)
	return 123
#	echo $(1) $(2)

#	echo func1
#	echo $(A) $(B)
endef
```

调用 

```
$(call FUNC1,abc,123)
```



## makefile install 

1. 将源文件编译成二进制可执行文件（包括各种库文件）
2. 创建目录，将可执行文件拷贝到制定目录（安装目录）
3. 加全局可执行路径  全局脚本
4. 卸载：重置编辑环境，删除无关文件



 = /usr/local/bin/



链接到全局 ln  -sv $(path)$(target)   $(BIN)













