#include "beep.h"

//函数功能描述：蜂鸣器响一下 
void beep_on_and_off(int fd)
{

	ioctl(fd,0,1); //响
	//usleep(1000*1000);
	sleep(1);
	ioctl(fd,1,1); //不响
}