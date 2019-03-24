#!/bin/bash

ARM_TARGET= arm_smarthome_server

SRC = $(wildcard ./src/*.c )
OBJ = $(SRC:%.c=%.o)

#指定头文件路径
I_DIR = -I./include  -I./lib/libjpeg

#指定静态库和动态库路径 和库名称
LIB_DIR = -L./lib/libjpeg -L./lib  -ljpeg  -lpthread -lapi_v4l2_arm1

#arm平台 arm-linux-gcc
$(ARM_TARGET): main.c 
	arm-linux-gcc   $^  -o $@  $(I_DIR) $(LIB_DIR) -L. -lmylib -Wall

client:client.c 
	gcc  $^  -o $@  -lpthread -Wall

#定义伪目标
.PHONY:clean
clean:
	rm -f   $(ARM_TARGET)  $(OBJ) main.o
