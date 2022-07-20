

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