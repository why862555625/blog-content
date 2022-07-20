
TARGET:=006_main
OBJ:=$(TARGET).o

CC:=g++

PATHS:=/tmp/006_main/
BIN:=/usr/local/bin/


START_SH:=$(TARGET)_start
STOP_SH:=$(TARGET)_stop
LOG:=$(PATHS)$(TARGET).log


$(TARGET):$(OBJ)


install:$(TARGET)
	if [ -d $(PATHS) ];\
		then echo $(PATHS) exist; \
	else \
	  	mkdir $(PATHS);\
	  	cp $(TARGET) $(PATHS);\
  		ln -sv $(PATHS)$(TARGET) $(BIN);\
  		touch $(LOG);\
  		chmod a+rwx $(LOG);\
  		echo "$(TARGET)>$(LOG) & echo $(TARGET) running...">$(PATHS)$(START_SH);\
  		echo "killall $(TARGET)">$(PATHS)$(STOP_SH);\
  		chmod a+x $(PATHS)$(START_SH);\
  		chmod a+x $(PATHS)$(STOP_SH);\
  		ln -sv $(PATHS)$(START_SH) $(BIN);\
  		ln -sv $(PATHS)$(STOP_SH) $(BIN);\
  	fi;

clean:
	$(RM) $(TARGET) $(OBJ) $(BIN)$(TARGET) $(BIN)$(START_SH) $(BIN)$(STOP_SH)
	$(RM) -rf $(PATHS)

.PHONY:clean install