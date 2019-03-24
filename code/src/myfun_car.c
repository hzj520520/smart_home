#include "myfun_car.h"

//函数功能描述：车库管理
int car_func(struct LcdInfo* lcd)
{
	flag_func=0;
	printf("\n---------进入CAR界面----------\n");
	//显示CAR界面
	lcd_draw_picture(lcd , 0, 0, CAR_INTERFACE);

	
	//1、打开串口文件 con2---->ttySAC1
	int tty_fd = open("/dev/ttySAC1", O_RDWR | O_NOCTTY);
	if(tty_fd < 0)
	{		
		perror("open ttySAC1 failed1\n");
		return -1;		
	}
	
	//1.打开蜂鸣器
	int beep_fd = open("/dev/beep",O_RDWR);
	if(beep_fd < 0)
	{
		perror("open beep error");
		return -1;
	}

	int card_num = 0;      //卡的数量
	
	unsigned int card_ids[MAX_CAR_NUM] = {0}; //卡号 可对应停车位
	         
	unsigned int card_id = 0;
	//1.1配置串口属性
	tty_setattr(tty_fd);
	printf("配置串口属性 ok\n");
	
	while(1)
	{
			//printf("---------等待获取卡号----------\n");
			card_id = get_card_id(tty_fd);//获取卡号
			if(card_id > 0 ) //大于0---》有卡号
			{
				printf("\n-------获取到卡号------\n");
				
				int card_num_temp = card_num;
				for(int i=0; i<MAX_CAR_NUM; i++) //循环判断是否重复 是就删除
				{
					
					if(card_id == card_ids[i]) 
					{
						card_num--;
						card_ids[i] = 0;
						
						printf("pos_car=%d\n", i);
						printf("delete pos_car = %d  ,card_id:0x%x\n", i, card_id);
						printf("card_num = %d\n", card_num);
						beep_on_and_off(beep_fd); //蜂鸣器响一下

						break;
					}
					
				}
				
				if(MAX_CAR_NUM == card_num) 			//若达到最大值,则不添加新的, 提示车位已满
				{
					printf("车位已满\n");
				}
				else if(card_num_temp == card_num  ) //如果不重复就 添加
				{
						for(int i=0; i<MAX_CAR_NUM; i++)
						{
							if(0 == card_ids[i])
							{
								card_ids[i] = card_id; 			 //先添加卡号，
								break;
							}
						}
						card_num++;                 			 //数量再加1
						printf("add card_id:0x%x\n", card_id);
						printf("card_num = %d\n", card_num);
						beep_on_and_off(beep_fd);                     //蜂鸣器响一下
						
				}
				
				printf("还剩%d个空车位\n", MAX_CAR_NUM - card_num); //提示剩多少个空车位
				
				//printf("\n-------------重新画----------------\n");
				lcd_draw_picture(lcd , 0, 0, CAR_INTERFACE); //画车库背景
				for(int i=0; i<MAX_CAR_NUM; i++)             //画红车
				{
					if(card_ids[i] !=0 ) //卡号不为0就 画
					{
						lcd_draw_picture(lcd, 10+i*170, 10, RED_CAR_SHOW);
					}
				}
				
				if(MAX_CAR_NUM == card_num)
				{
					lcd_draw_picture(lcd , 0, 170, CAR_IS_FULL_TIPS); //画车库背景
				}
				
				for(int i=0; i<MAX_CAR_NUM; i++)
				{
					printf("第%d车位的卡号 : 0x%x\n", i, card_ids[i]);
				}
				
											
			}// if(card_id > 0 )

								
				
			//触摸屏获取的坐标 得到flag_func的值
			if(get_x>660 && get_x<780 && get_y>300 && get_y<370) //9返回用户主界面
			{	
					get_x=get_y=0;
					flag_func = GO_BACK_HOME;
			}

			if(flag_func == GO_BACK_HOME)
			{						
					
					close(tty_fd);  //关闭ttySAC1串口文件

					printf("go back main face\n");
					flag_func=0;
					return 0; 		//成功返回
			}
			
	}//while(1)
		
}