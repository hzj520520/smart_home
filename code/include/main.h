#ifndef __MAIN_H__
#define __MAIN_H__


#define MYUSR_INTERFACE  "../mypic_and_music/myinterface_jpg/myuser_interface.jpg"     //用户主界面
#define MUSIC_INTERFACE  "../mypic_and_music/myinterface_jpg/music_interface.jpg"      //音乐界面
#define PIC_INTERFACE    "../mypic_and_music/myinterface_jpg/pic_interface.jpg"        //相册界面
 
#define CAR_INTERFACE      "../mypic_and_music/myinterface_jpg/car_interface.jpg"       //车库界面
#define RED_CAR_SHOW       "../mypic_and_music/myinterface_jpg/redcar_show.jpg"         //redcar_show 小车模型
#define CAR_IS_FULL_TIPS   "../mypic_and_music/myinterface_jpg/car_is_full_tips.jpg"    //车位已满提示图
#define MAX_CAR_NUM     	3   													    //最大停车位 数目


#define MYPIC_JPG_DIR      "../mypic_and_music/mypic_jpg"        			  //相册目录
#define MONITOR_JPG_DIR    "../mypic_and_music/myvideo_screenshot_jpg"        //监控截图目录
#define MYMUSIC_MP3_DIR    "../mypic_and_music/mymusic_mp3"      			  //mp3目录
#define GIF_JPG_DIR        "../mypic_and_music/gif_jpg"          			  //gif动态图目录


#define EIXT_PROGRAM_JPG  "../mypic_and_music/myinterface_jpg/exit_program.jpg"    			   //退出程序界面

/*****     主界面按钮    ******/
#define PIC_BUTTON_JPG  		     "../mypic_and_music/myinterface_jpg/pic_button.jpg"     				//相册按钮
#define MP3_BUTTON_JPG 			     "../mypic_and_music/myinterface_jpg/mp3_button.jpg"     				//mp3按钮
#define CAR_MANAGEMENT_BUTTON_JPG  	 "../mypic_and_music/myinterface_jpg/car_management_button.jpg"    	    //车库管理按钮
#define MONITOR_BUTTON_JPG  	     "../mypic_and_music/myinterface_jpg/monitor_button.jpg"    	        //监控按钮
#define EXIT_BUTTON_JPG  	     	 "../mypic_and_music/myinterface_jpg/exit_button.jpg"    	            //退出程序按钮


/****     监控界面按钮   *****/
#define   SCREENSHOT_BUTTON_JPG    	       "../mypic_and_music/myinterface_jpg/screenshot_button.jpg"   	    //截图按钮
#define   LOOK_SCREENSHOT_BUTTON_JPG       "../mypic_and_music/myinterface_jpg/look_screenshot_button.jpg"      //查看截图按钮
#define   GO_BACK_HOME_BUTTON_JPG    	   "../mypic_and_music/myinterface_jpg/go_back_home_button.jpg"    	    //返回用户主界面按钮



/*
*****                 指令 1.进入相册     2.进入音乐   3.上一张图片   4.下一张图片 
******                     5.播放         6.暂停歌     7.上一首歌     8.下一首歌
*******                    9.返回        10.退出程序
*/

#define GO_TO_PIC     1   // 1.进入相册
#define GO_TO_MP3     2   // 2.进入音乐 
#define LAST_PIC      3   // 3.上一张图片
#define NEXT_PIC      4   // 4.下一张图片
#define PLAY_MP3      5   // 5.播放 
#define STOP_MP3      6   // 6.暂停歌
#define LAST_MP3      7   // 7.上一首歌
#define NEXT_MP3      8   // 8.下一首歌
#define GO_BACK_HOME  9   // 9.返回

#define EIXT_KEY       10            // 10.退出程序

#define GO_TO_CAR      11            // 11.进入车库界面
#define GO_TO_MONITOR  12            // 12.监控

#define SCREENSHOT_KEY 13           // 13.截图
#define LOOK_SCREENSHOT_KEY 14      // 14.查看截图


int fd_ts;           //触摸屏fd 最后由main 关闭
int get_x, get_y;      //获取坐标
int flag_func;       //功能标志


#endif