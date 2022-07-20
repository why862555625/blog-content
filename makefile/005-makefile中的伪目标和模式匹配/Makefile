
#伪目标 .PHONY:clean
#声明目标为伪目标之后，makefile将不会判断目标是否存在或该目标是否需要更新
.PHONY:clean show


#模式匹配 %目标:%依赖
#目标和依赖相同部份，可用%来通配
#%.o:%.cpp


#wildcard				$(wildcard  ./*.cpp) 获取当前目录下所有的.cpp文件
#patsubst 				$(patsubst  %.cpp，%.o，./*.cpp) 将对应的cpp 文件名替换成 .o 文件名



#OBJ= sub.o multi.o calc.o add.o
OBJ=$(patsubst %.cpp,%.o,$(wildcard ./*.cpp))
TARGET=calc


$(TARGET):$(OBJ)
	$(CXX) $^ -o $@

%.o:%.cpp
	$(CXX) -c $^ -o $@

clean:
	$(RM) *.o $(TARGET)

show:
	#echo $(AS)
	#echo $(CC)
	#echo $(CPP)
	#echo $(CXX)
	#echo $(RM)

	#echo $(wildcard  ./*.cpp)
	#echo $(patsubst %.cpp,%.o,$(wildcard ./*.cpp))
	echo $(OBJ)
