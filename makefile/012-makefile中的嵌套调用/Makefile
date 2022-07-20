
#-C 指定工作目录
#$$表示展开shell 中的变量


.PHONY:001 002 clean

DIR=001 002


all:$(DIR)

$(DIR):
	make -C $@


clean:
	echo $(shell for dir in $(DIR);do make -C $$dir clean;done)




all-v1:
	make -C ./001
	make -C ./002

clean-v1:
	make -C ./001  clean
	make -C ./002 clean