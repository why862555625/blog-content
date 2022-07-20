

#自定义函数，不是真正的函数，本质上是多行命令，放在外面了
#这里的自定义函数，没有返回值

LS1=$(call FUNC3)

define FUNC3
	echo $(shell ls)
	$(RM) a b c d
endef

LS2:=$(call FUNC3)


define FUNC2
	return 123
endef

default:
#	echo $(call FUNC2)
#	echo return 123
	$(call FUNC3)
	$(LS2)
	$(LS1)




A:=123
B:=$(A)

define FUNC1

	echo $(0)

#	echo $(1) $(2)

#	echo func1
#	echo $(A) $(B)
endef

A:=456
all:
	$(call FUNC1,abc,def,$(A))

#	echo $(A)  $(B)

A:=789