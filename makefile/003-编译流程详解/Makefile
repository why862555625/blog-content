
# 这样好吗 ？
# 这样不好，这样不讲武德

# 第一次编译两小时
# 第二次编译五分钟
# 这样分开来写，保证只编译有改动的代码

#calc:
#	gcc add.cpp sub.cpp multi.cpp calc.cpp -o calc



calc:add.o sub.o multi.o
	gcc add.o sub.o multi.o calc.cpp -o calc

add.o:add.cpp
	gcc -c add.cpp -o add.o

sub.o:sub.cpp
	gcc -c sub.cpp -o sub.o

multi.o:multi.cpp
	gcc -c multi.cpp -o multi.o