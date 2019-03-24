#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <sys/select.h>
#include <time.h>

#include "searchdir.h"
#include "doublelinklist.h"
#include "lcdinfo.h"
#include "mypthread.h"        //mypthread.h包含main创建的线程函数

#include "yuyv.h"
#include "main.h"             //main.h包含各种宏定义，全局变量声明（不能定义）
#include "myfun_pic.h"
#include "myfun_mp3.h"
#include "myfun_car.h"       //车库管理
#include "myfun_monitor.h"   //监控
		 

  /**********函数声明********/
void show_gif_jpg(struct LcdInfo* lcd);         //显示动态图 
void get_flag_func_in_home_interface();         //在主界面 通过坐标获取flag_func的值
void draw_home_interface(struct LcdInfo* lcd);  //画主界面  


int main(int argc, char *argv[])
{	
	//初始化lcd
	struct LcdInfo* lcd = lcd_init("/dev/fb0");		
		
	//显示开机动态图
	printf("----show---gif---jpg-----\n");
	show_gif_jpg(lcd);
	
	int ret=0;
	
	//创建一个子线程--->打开触摸屏，获取触摸屏坐标
	pthread_t pth1_id=0;
	ret = pthread_create(&pth1_id, NULL, get_xy_thread_func, NULL);
	if(ret < 0)
	{
		perror("create thread1 error");
		return -1;
	}
		
	//创建一个子线程--->获取键盘输入
	pthread_t keyboard_pth1_id=0;
	ret = pthread_create(&keyboard_pth1_id, NULL, get_keyboard_thread_func, NULL);
	if(ret < 0)
	{
		perror("create keyboard_pth1_id error");
		return -1;
	}
	
	//创建一个子线程--->服务器初始化
	pthread_t server_pth1_id=0;
	ret = pthread_create(&server_pth1_id, NULL, server_thread_func, NULL);
	if(ret < 0)
	{
		perror("create server_pth1_id error");
		return -1;
	}

   //进入主界面
	while(1) //获取坐标 可对应相关功能函数
	{
		if(flag_func==0)
		{
			flag_func=-1; //确保不重画主界面
	
			printf("\n---------进入main界面----------\n");
			//画主界面 和 功能按键
			draw_home_interface(lcd);  
			
			//提示输入数字 界面
			printf("in mainface , input <%d>---picture   <%d>---mp3 <%d>---car  <%d>---monitor\n", GO_TO_PIC, GO_TO_MP3, GO_TO_CAR, GO_TO_MONITOR);
		}
		
		//在主界面 通过坐标获取flag_func的值
		get_flag_func_in_home_interface();   
			
		//通过flag_func, 实现相应功能
		switch(flag_func)
		{
			case GO_TO_PIC: 
							picture_func(lcd, MYPIC_JPG_DIR, 600, 480); //1.进入相册界面 图片放缩为600*480
							break;
			case GO_TO_MP3:
							mp3_func(lcd);                 	 //2.进入MP3界面	
							break;
			case GO_TO_CAR:
							car_func(lcd);                	//3.进入CAR界面	
							break;
			case GO_TO_MONITOR:
							monitor_func(lcd);				//4.进入监控界面	
							break;
		}					
		
		if(flag_func == EIXT_KEY) //退出程序
		{
			
			printf("退出程序\n");
			break;
		}
		
	}
	
		
    //发送终止信号给thread线程，如果成功则返回0，否则为非0值。发送成功并不意味着thread会终止。	
	pthread_cancel(pth1_id);
	pthread_cancel(server_pth1_id);
	pthread_cancel(keyboard_pth1_id);
	
	   
	pthread_join(pth1_id, NULL); 				//等待子线程
	pthread_join(server_pth1_id, NULL); 		//等待子线程
	pthread_join(keyboard_pth1_id, NULL); 		//等待子线程
	
	lcd_draw_picture(lcd , 0, 0, EIXT_PROGRAM_JPG);
	
	lcd_destroy(lcd);//解除映射 并关掉lcd->fd
	close(fd_ts);
    
	printf("quit ok\n");
	
	return 0;
}//main


//在主界面 获取flag_func的值
void get_flag_func_in_home_interface()
{
		//由坐标确定 对应flag_func的值	
		if(get_x>0 && get_x<100 && get_y>0 && get_y<100)
		{			
				get_x=get_y=0;  //坐标重置为0
				flag_func = GO_TO_PIC;
		}
		
		else if(get_x>110 && get_x<210 && get_y>0 && get_y<100)
		{		
				get_x=get_y=0;  //坐标重置为0
				flag_func = GO_TO_MP3;
		}
		else if(get_x>220 && get_x<320 && get_y>0 && get_y<100)//车库界面
		{	
				get_x=get_y=0;  //坐标重置为0
				flag_func = GO_TO_CAR;				
		}
		else if(get_x>330 && get_x<430 && get_y>0 && get_y<100)//监控界面
		{
				get_x=get_y=0;  //坐标重置为0			
				flag_func = GO_TO_MONITOR;				
		}
		else if(get_x>0 && get_x<100 && get_y>320 && get_y<420)//退出程序
		{
				get_x=get_y=0;  //坐标重置为0
				flag_func = EIXT_KEY;				
		}

}

//显示动态图
void show_gif_jpg(struct LcdInfo* lcd)
{
	char buf[100]={0};
	for(int i=0; i<35; i++)
	{
		//合成动态图每一张的路径 
		sprintf(buf, "../mypic_and_music/gif_jpg/Frame%d.jpg", i);
		//显示每一张动态图片
		lcd_draw_picture(lcd , 0, 0, (const char *)buf);
		usleep(10*1000);	
	}
	sleep(1);
}


//画主界面
void draw_home_interface(struct LcdInfo* lcd)
{
	lcd_draw_picture(lcd , 0,       0, MYUSR_INTERFACE); 				//画主界面背景
	
	lcd_draw_picture(lcd , 0,       0, PIC_BUTTON_JPG);  				//画相册按钮
	lcd_draw_picture(lcd , 110,     0, MP3_BUTTON_JPG);  			    //画mp3按钮	
	lcd_draw_picture(lcd , 110*2,   0, CAR_MANAGEMENT_BUTTON_JPG);      //画车库管理按钮
	lcd_draw_picture(lcd , 110*3,   0, MONITOR_BUTTON_JPG);  		    //画监控按钮
		
	lcd_draw_picture(lcd , 0,       320, EXIT_BUTTON_JPG);  		    //画退出程序按钮 左下角
}
 

