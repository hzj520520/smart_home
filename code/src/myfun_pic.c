#include "myfun_pic.h"


//函数功能描述:   相册功能 
//         lcd:   在哪个lcd显示
//  jpgdirpath:   jpg目录路径
//  	  w、h:   放缩为 w * h
void picture_func(struct LcdInfo* lcd, const char *jpgdirpath, int w, int h)
{
	flag_func=0;
	printf("\n---------进入picture界面----------\n");
	
	//画相册界面 画最左边按钮而已
	lcd_draw_picture(lcd , 600, 0, PIC_INTERFACE); 
	
	int num_jpg=0;                    //jpg文件个数
	double_list head_jpg=init_list();//初始化头结点 无数据的 遍历目录时才加入数据
	//遍历相册目录
	search_dir(jpgdirpath, ".jpg", head_jpg, &num_jpg);
	printf("jpg num=%d\n", num_jpg);
	
	double_list head_jpg_temp=head_jpg;//临时变量存储头结点
	
	int flag_draw=1;                   //可以画标志 1.可以画 0.不用重复画
	struct RgbData rgbData;
	bzero(&rgbData, sizeof(rgbData)); //定义结构体后，先清0 
	
	
	while(1)//获取坐标 1.上一张 2.下一张 3.返回
	{

		get_flag_func_in_pic_interface(); //在pic界面 得到flag_func的值 

		//在pic界面, 通过flag_func的值, 实现对应功能			
		if(flag_func == LAST_PIC) //3.上一张
		{
			printf("last one\n");
			flag_func=0;
			flag_draw=1;                         //可以画
			head_jpg_temp=head_jpg_temp->prev;
		}
		else if(flag_func == NEXT_PIC) //4.下一张
		{
			printf("next one\n");
			flag_func=0;
			flag_draw=1;                        //可以画
			head_jpg_temp=head_jpg_temp->next;
		}
		else if(flag_func == GO_BACK_HOME)//9.返回
		{
			printf("go back main face\n");
			flag_func=0;
			destroy_double_list(head_jpg); //释放malloc的内存
			break;
		}	
		
		
		//避免重复画同一张
		if(flag_draw == 1)
		{
			flag_draw=0;//不用重复画
			
			if(rgbData.rgb)//若不为NULL
			{
				free(rgbData.rgb);
				rgbData.rgb=NULL;
			}   
			
			printf("------show %s\n", head_jpg_temp->data);
			
			//显示第一张图片
			jpeg_to_rgb(&(rgbData), head_jpg_temp->data);
			pictrue_zoom_wh(&rgbData, w, h);        //把图片放缩为w*h 600*480
			lcd_draw_data(lcd, 0, 0, rgbData.w, rgbData.h, rgbData.rgb);   
			
				//提示输入数字
			printf("\n---------in jpg_face, input <3>---last one  <4>---next one  9---quit\n");
			//scanf("%d", &flag_func);
			printf("flag_func=%d\n",flag_func);
			
		}  //避免重复画同一张
									
	}
}

 // 函数功能描述：在pic界面 得到flag_func的值   
void get_flag_func_in_pic_interface()
{
	//由坐标确定 对应功能区	
	if(get_x>600 && get_x<800 ) 
	{			
			if(get_y>0 && get_y<60)  //3上一张
			{											
				flag_func = LAST_PIC;
				printf("x=%d y=%d flag_func=%d\n", get_x, get_y, flag_func);
				get_x=get_y=0;
			}
			
			else if(get_y>90 && get_y<150) //4下一张
			{
				flag_func = NEXT_PIC;
				printf("x=%d y=%d flag_func=%d\n", get_x, get_y, flag_func);
				get_x=get_y=0;
			}
			
			else if(get_y>350 && get_y<400) //9返回
			{

				flag_func = GO_BACK_HOME;
				printf("x=%d y=%d flag_func=%d\n", get_x, get_y, flag_func);
				get_x=get_y=0;								
			}								
	}

}

