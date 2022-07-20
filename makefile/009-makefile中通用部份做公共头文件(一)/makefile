

#公共

SOURCE=$(wildcard  ./*.cpp ./*.c)
OBJ=$(patsubst  %.cpp,%.o,$(SOURCE))
OBJ:=$(patsubst  %.c,%.o,$(OBJ))



.PHONY:clean

$(TARGET):$(OBJ)
	$(CXX) $^ -o $@


clean:
	$(RM) $(TARGET) $(OBJ)


show:
	echo $(SOURCE)
	echo $(OBJ)