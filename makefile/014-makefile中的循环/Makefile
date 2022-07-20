
#循环
#makefile 中只有一个循环 foreach，只支持 GNU Make ，其它平台的make ，可以用shell 中的循环来实现
#可以在循环中逐个的修改值



TARGET:=a b c d
FILE:=$(foreach v, $(TARGET),$v.txt)

all:
	#echo $(TARGET)
	#echo $(foreach v, $(TARGET),$v)
	#touch $(TARGET)
	#touch $(foreach v, $(TARGET),$v.txt)
	#mkdir $(foreach v, $(TARGET),$v_txt)
	#echo $(FILE)

	for v in $(TARGET);\
		do touch $$v.txt;\
	done;\

	$(shell for v in $(TARGET); do touch $$v-txt;done)


clean:
	$(RM) -rf $(TARGET) *txt
