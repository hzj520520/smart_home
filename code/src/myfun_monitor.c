#include "myfun_monitor.h"

//函数功能描述：监控界面功能
int monitor_func(struct LcdInfo* lcd)
{
	flag_func=0;
	printf("\n---------进入monitor界面----------\n");
	
		
	//定义结构体变量
	struct jpg_data video_buf; //640*480
	
	//初始化摄像头	
	linux_v4l2_yuyv_init("/dev/video7"); 
	
	//开启摄像头捕捉
	linux_v4l2_start_yuyv_capturing();   
	
	draw_monitor_interface(lcd); //画监控界面 按钮
	while(1)
	{
		//获取摄像头捕捉的画面
		linux_v4l2_get_yuyv_data (&video_buf); 
		
		//显示摄像头捕捉的画面
		show_video_data(lcd, 0, 0, video_buf.jpg_data , video_buf.jpg_size);
		
		get_flag_func_in_monitor_interface();
		switch( flag_func )
		{
			case SCREENSHOT_KEY:
									flag_func=0;
									printf("截图\n");
									screenshot_func(video_buf.jpg_data, video_buf.jpg_size);
									break;
			case LOOK_SCREENSHOT_KEY:
									flag_func=0;
									printf("查看截图\n");
									look_screenshot_func(lcd);
									break;
			case GO_BACK_HOME:
						flag_func=0;
						printf("x=%d y=%d flag_func=%d\n", get_x, get_y, flag_func);
						get_x=get_y=0;	
						
							//摄像头解除初始化
						linux_v4l2_yuyv_quit();

						printf("go back main face\n");

						return 0; 		//成功返回
										
		}
				
	}
			
}


/*
 *功能描述：截图函数
 *@pjpg_buf：		指向存放jpg数据的起始地址
 *@jpg_buf_size：	jpg数据大小
 */
int screenshot_func(unsigned char *pjpg_buf, unsigned int jpg_buf_size)
{
	static int i_num = 0;  //截图次数
	char filename[256]={0}; //jpg文件名
	char timebuf[256]={0};  //时间字符串
	
    time_t timep;  
    struct tm *p_tm;  
    timep = time(NULL);  
    p_tm = localtime(&timep); /*获取本地时区时间*/  
    printf("time%04d%02d%02d%02d%02d%02d.jpg\n", (p_tm->tm_year+1900), (p_tm->tm_mon+1), p_tm->tm_mday, p_tm->tm_hour,p_tm->tm_min, p_tm->tm_sec); 
	sprintf(timebuf, "time%04d%02d%02d%02d%02d%02d", (p_tm->tm_year+1900), (p_tm->tm_mon+1), p_tm->tm_mday, p_tm->tm_hour,p_tm->tm_min, p_tm->tm_sec);

	i_num++; //截图次数加1
	sprintf(filename, "../mypic_and_music/myvideo_screenshot_jpg/%s.jpg", timebuf); //以当前时间来命名 jpg图片

	write_bin_file(filename, pjpg_buf, jpg_buf_size); //写二进制文件 将jpg数据写到jpg文件里
	
	//提示截图成功
	printf("截图成功");	
	return 0;
}

/*
 * 功能描述：查看截图函数
 */
int look_screenshot_func(struct LcdInfo* lcd)
{
	printf("查看截图\n");
	flag_func = 0;
	picture_func(lcd, MONITOR_JPG_DIR, 600, 480); 
	
	draw_monitor_interface(lcd); //返回监控界面 画monitor界面
	
	return 0;
}


/*写二进制文件函数
 *@filename: 		指向存放jpg数据的文件
 *@pjpg_buf：		指向存放jpg数据的起始地址
 *@jpg_buf_size：	jpg数据大小
 */
int write_bin_file(char* filename, unsigned char *pjpg_buf, unsigned int jpg_buf_size)
{
	FILE *fp = fopen(filename, "wb");
	if(NULL == fp)
	{
		printf("fopen %s failed\n", filename);
		return -1;
	}
	
	fwrite(pjpg_buf, jpg_buf_size, 1, fp);
	
	fclose(fp);
	return 0;
}


//画monitor界面
void draw_monitor_interface(struct LcdInfo* lcd)
{
	lcd_draw_picture(lcd ,   0,    0, MYUSR_INTERFACE); 				    //画主界面背景
	lcd_draw_picture(lcd , 640,    0, SCREENSHOT_BUTTON_JPG);  				//画截图按钮	
	lcd_draw_picture(lcd , 640,  110, LOOK_SCREENSHOT_BUTTON_JPG);  		//画查看截图按钮
	lcd_draw_picture(lcd , 640,  320, GO_BACK_HOME_BUTTON_JPG);  			//画返回按钮	
}


//功能描述: 在monitor界面 获取flag_func的值
void get_flag_func_in_monitor_interface()
{

		if(get_x>640 && get_x<740 && get_y>0 && get_y<100)  //截图
		{			
				get_x=get_y=0;     //坐标重置为0
				flag_func = SCREENSHOT_KEY;
		}
		
		else if(get_x>640 && get_x<740 && get_y>110 && get_y<210) //查看截图
		{
				get_x=get_y=0;    //坐标重置为0
				flag_func = LOOK_SCREENSHOT_KEY;

		}
		else if(get_x>640 && get_x<740 && get_y>320 && get_y<420)// 返回用户主界面
		{	
				get_x=get_y=0;    //坐标重置为0
				flag_func = GO_BACK_HOME;				
		}

}