#ifndef __MYFUN_PIC_H__
#define __MYFUN_PIC_H__

#include "searchdir.h"
#include "doublelinklist.h"
#include "lcdinfo.h"
#include "main.h"

//函数功能描述：相册功能
void picture_func(struct LcdInfo* lcd, const char *jpgdirpath, int w, int h);

//函数功能描述：在pic界面 得到flag_func的值  
void get_flag_func_in_pic_interface();  
  
  
#endif