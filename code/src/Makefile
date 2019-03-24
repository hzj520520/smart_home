#!/bin/bash

SRC = $(wildcard *.c )
OBJ = $(SRC:%.c=%.o)

LIB = libmylib.a

#指定头文件路径
I_DIR = -I../include  -I../lib/libjpeg
#指定静态库和动态库路径 和库名称
LIB_DIR = -L../lib/libjpeg -L../lib  -ljpeg  -lpthread -lapi_v4l2_arm1


#arm平台 arm-linux-gcc
$(LIB):  $(OBJ)
	ar  -rc    $@  $^   
	
$(OBJ):$(SRC)
	arm-linux-gcc  -c $(subst .o,.c,$@) -o $@  $(I_DIR) $(LIB_DIR) -Wall



#定义伪目标
.PHONY:clean
clean:
	rm -f   $(LIB)  $(OBJ) 
