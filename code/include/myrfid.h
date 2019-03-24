#ifndef _MYRFID_H
#define _MYRFID_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <strings.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ioctl.h>



//配置串口属性
int tty_setattr(int tty_fd);

//计算校验和
unsigned char BCC_VALUE(unsigned char *buf);

//发送A命令
int send_A(int tty_fd);

//发送B命令
unsigned int send_B(int tty_fd);

//获取卡号ID
unsigned int get_card_id(int tty_fd);



#endif