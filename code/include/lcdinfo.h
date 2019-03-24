#ifndef LCD_INFO_H
#define LCD_INFO_H

#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include "jpeglib.h"


#define LCD_WIDTH  			800
#define LCD_HEIGHT 			480
#define FB_SIZE				(LCD_WIDTH * LCD_HEIGHT * 4)



//设计一个lcd结构体
struct LcdInfo
{
	int fd;//文件描述符
	unsigned int *mmp;//映射空间首地址
	int lcdW;
	int lcdH;
	unsigned long lcdsize;
};

struct RgbData
{
	unsigned char *rgb;
	int w;
	int h;
	int pixel_types;
};



//初始化lcd
struct LcdInfo* lcd_init(const char *dev);
//销毁
bool lcd_destroy(struct LcdInfo* lcd);
//绘制点
bool lcd_draw_point(struct LcdInfo* lcd, int x, int y, unsigned int  color);
//绘区域颜色
bool lcd_draw_rect(struct LcdInfo* lcd, int x, int y, int w, int h, unsigned int  color);
//绘区域颜色
bool lcd_draw_picture(struct LcdInfo* lcd , int x, int y, const char *picture);

bool jpeg_to_rgb(struct RgbData *rgbData, const char *file);
bool lcd_draw_data(struct LcdInfo* lcd, int x, int y, int w, int h, unsigned char* rgb_data);

 //把图片放缩为w*h
bool pictrue_zoom_wh(struct RgbData* rgbdata, int w, int h);

//显示摄像头数据
int show_video_data(struct LcdInfo* lcd, unsigned int x, unsigned int y, unsigned char *pjpg_buf, unsigned int jpg_buf_size);

#endif//LCD_INFO_H 
