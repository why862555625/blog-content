
A:=321123

RS1:=
RS2:=
RS3:=
RS4:=


ifeq ($(A),123)
	RS1:=123
else
	ifeq ($(A),321)
		RS1:=321
	else
		RS1:=no-123-321
	endif

endif

ifndef A
	RS3:=yes
else
	RS3:=no
endif

ifndef FLAG
	FLAG:=default-flag
endif


all:
	echo $(RS1)
	echo $(RS3)
	echo flag=$(FLAG)