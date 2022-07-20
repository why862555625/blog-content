
OBJ=add.o sub.o multi.o calc.o
TARGET=calc




$(TARGET):$(OBJ)
	$(CXX) $^ -o $@

add.o:add.cpp
	$(CXX) -c $^ -o $@

sub.o:sub.cpp
	$(CXX) -c $^ -o $@

multi.o:multi.cpp
	$(CXX) -c $^ -o $@

calc.o:calc.cpp
	$(CXX) -c $^ -o $@

clean:
	$(RM) *.o $(TARGET)

show:
	echo $(AS)
	echo $(CC)
	echo $(CPP)
	echo $(CXX)
	echo $(RM)




