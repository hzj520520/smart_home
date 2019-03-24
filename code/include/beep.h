#ifndef BEEP_H
#define BEEP_H

#include <unistd.h>
#include <sys/ioctl.h>


//函数功能描述：蜂鸣器响一下 
void beep_on_and_off(int fd);

#endif//BEEP_H
