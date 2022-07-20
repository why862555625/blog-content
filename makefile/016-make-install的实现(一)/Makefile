
TARGET:=006_main
OBJ:=$(TARGET).o

CC:=g++

PATH:=/tmp/006_main/
BIN:=/usr/local/bin/


$(TARGET):$(OBJ)


install:$(TARGET)
	if [ -d $(PATH) ];\
		then echo $(PATH) exist; \
	else \
	  	/bin/mkdir $(PATH);\
	  	/bin/cp $(TARGET) $(PATH);\
  		/bin/ln -sv $(PATH)$(TARGET) $(BIN);\
  	fi;

clean:
	$(RM) $(TARGET) $(OBJ)
	$(RM) -rf $(PATH)


.PHONY:clean install