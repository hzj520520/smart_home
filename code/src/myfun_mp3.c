#include "myfun_mp3.h"

//函数功能描述： MP3界面功能
void mp3_func(struct LcdInfo* lcd)
{
			flag_func=0;
			printf("\n---------进入mp3界面----------\n");
			//显示音乐界面
			lcd_draw_picture(lcd , 0, 0, MUSIC_INTERFACE);
			int num_mp3=0;                    //jpg文件个数
			double_list head_mp3=init_list();//初始化头结点 无数据的 遍历目录时才加入数据
				//遍历相册目录
			search_dir(MYMUSIC_MP3_DIR, ".mp3", head_mp3, &num_mp3);
			printf("mp3 num=%d\n", num_mp3);
				
			double_list head_mp3_temp=head_mp3;//临时变量存储头结点
				
			char mp3_buf[1024]={0};
			sprintf(mp3_buf, "madplay %s &", head_mp3_temp->data);
			system(mp3_buf);                 //先播放第一首歌
			int tip_flag=1;                 //提示标志
			while(1)            //循环功能选择
			{
							
				//由坐标确定 对应功能区 判断进入相册 or 音乐	
				if(get_x>170 && get_x<270 && get_y>160 && get_y<220) //7上一首
				{		
						flag_func = LAST_MP3;
						printf("x=%d y=%d flag_func=%d\n", get_x, get_y, flag_func);
						get_x=get_y=0;						
				}
				if(get_x>440 && get_x<555 && get_y>160 && get_y<220) //8下一首
				{	

						flag_func = NEXT_MP3;
						printf("x=%d y=%d flag_func=%d\n", get_x, get_y, flag_func);
						get_x=get_y=0;							
				}
				if(get_x>300 && get_x<420 && get_y>160 && get_y<220) //5暂停
				{	
			
						flag_func = STOP_MP3;
						printf("x=%d y=%d flag_func=%d\n", get_x, get_y, flag_func);
						get_x=get_y=0;							
				}
				if(get_x>300 && get_x<420 && get_y>80 && get_y<140) //6播放
				{

						flag_func = PLAY_MP3;
						printf("x=%d y=%d flag_func=%d\n", get_x, get_y, flag_func);
						get_x=get_y=0;							
				}
				if(get_x>700 && get_x<780 && get_y>340 && get_y<390) //9返回用户主界面
				{	
			
						flag_func = GO_BACK_HOME;
						printf("x=%d y=%d flag_func=%d\n", get_x, get_y, flag_func);
						get_x=get_y=0;							
				}
				
				
				if(tip_flag==1)
				{
					tip_flag=0;
									//提示输入数字
					printf("\n---------in mp3_face, input %d---last mp3   %d---next mp3  %d---stop\n", LAST_MP3, NEXT_MP3, STOP_MP3);
					printf("--------------------------  %d---play       %d---quit\n", PLAY_MP3, GO_BACK_HOME);
					//scanf("%d", &flag_func);
					printf("flag_func=%d\n",flag_func);
				}
/*
              system("madplay -a -10 xxx.mp3 &");	//-a音量控制 范围(-175~+18)
			  system("killall -STOP madplay &");    //暂停播放
			  system("killall -CONT madplay &");    //恢复继续播放
			  
			  system("killall madplay");    //结束播放
			  
			  system("killall -9 madplay");    //终止
*/			
					if(flag_func == LAST_MP3) //7.上一首
					{
						printf("last one\n");						
						flag_func=0;  
						tip_flag=1;						
						head_mp3_temp=head_mp3_temp->prev;
						printf("------show %s\n", head_mp3_temp->data);
						
						system("killall -9 madplay");    //先终止 再播放
						sprintf(mp3_buf, "madplay %s &", head_mp3_temp->data);
						system(mp3_buf);
					}
					else if(flag_func == NEXT_MP3) //8.下一首
					{
						printf("next one\n");
						flag_func=0;  
						tip_flag=1;						
						head_mp3_temp=head_mp3_temp->next;
						printf("-----show %s\n", head_mp3_temp->data);
						
						system("killall -9 madplay");    //先终止 再播放
						sprintf(mp3_buf, "madplay %s &", head_mp3_temp->data);
						system(mp3_buf);
					}
					else if(flag_func == STOP_MP3) //6.暂停
					{
						printf("mp3 stop play\n");
						flag_func=0; 
						tip_flag=1;
						printf("-----stop %s\n", head_mp3_temp->data);
						system("killall -STOP madplay &");    //暂停播放
					}
					else if(flag_func == PLAY_MP3) //5.播放
					{
						printf("mp3  play\n");
						flag_func=0; 
						tip_flag=1;
						printf("-----play %s\n", head_mp3_temp->data);
						system("killall -CONT madplay &");    //恢复继续播放
					}
/* 					else if(flag_func==8)//8.关掉音乐，并返回用户主界面
					{
						printf("go back main face\n");
						flag_func=0;	
						system("killall madplay");    //结束播放
						break;
					} */
					else if(flag_func == GO_BACK_HOME)//9.返回用户主界面
					{
						printf("go back main face\n");
						flag_func=0;	
						system("killall madplay");    //结束播放
						destroy_double_list(head_mp3);
						break;
					}
			
			}//循环功能选择 while(1)
}
