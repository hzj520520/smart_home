#ifndef MYPTHREAD_H
#define MYPTHREAD_H

#include <stdio.h>
#include <stdlib.h>
       
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>

#include <linux/input.h>
#include <sys/select.h>

#include <sys/stat.h>   //open
#include <sys/types.h>  //open
#include <fcntl.h>      //open

#include "main.h"

void *server_thread_func(void *arg);            //线程---服务器 
int server_init(const char* ip, int port);      //服务器初始化

void *get_xy_thread_func(void *arg);            //线程--打开触摸屏，获取触摸屏坐标
int get_xy(int fd, int *x, int *y);             //获取触摸屏坐标 函数

void *get_keyboard_thread_func(void *arg);      //线程--获取键盘输入的数据

#endif