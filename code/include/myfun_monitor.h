#ifndef __MYFUN_MONITOR_H__
#define __MYFUN_MONITOR_H__

#include <time.h>
#include "searchdir.h"
#include "doublelinklist.h"
#include "lcdinfo.h"
#include "main.h"
#include "yuyv.h"

#include "myfun_pic.h" //查看截图时，用到函数picture_func; 


//函数功能描述：监控功能 
int monitor_func(struct LcdInfo* lcd);   

//函数功能描述：查看截图函数
int look_screenshot_func(struct LcdInfo* lcd);

/*
 *功能描述：截图函数
 *@pjpg_buf：		指向存放jpg数据的起始地址
 *@jpg_buf_size：	jpg数据大小
 */
int screenshot_func(unsigned char *pjpg_buf, unsigned int jpg_buf_size);


/*写二进制文件函数
 *@filename: 		指向存放jpg数据的文件
 *@pjpg_buf：		指向存放jpg数据的起始地址
 *@jpg_buf_size：	jpg数据大小
 */
int write_bin_file(char* filename, unsigned char *pjpg_buf, unsigned int jpg_buf_size);  

//功能描述: 画monitor界面
void draw_monitor_interface(struct LcdInfo* lcd); 


//功能描述: 在monitor界面 获取flag_func的值
void get_flag_func_in_monitor_interface();  


  
#endif