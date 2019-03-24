#include "mypthread.h"



//服务器线程函数
void *server_thread_func(void *arg) 
{
    //服务器初始化 ip+port 第一参数NULL--本地地址
	int soc_fd=server_init(NULL, 6000);
	if(soc_fd<0)
	{
		printf("server_init failed\n");
	}
	
	int acc_fd=0;
	
	struct sockaddr_in cli_addr;
	socklen_t len=sizeof(cli_addr);
	bzero(&cli_addr, len);
	
//多路复用有关变量	
	int acc_fd_num=0;         //已连接客户端个数 0		
	int acc_fds[6];           //存储客户端的套接字
	 
    for(int i = 0; i < 6; i++)    //初始化acc_fds[6]
    {
        acc_fds[i] = -1;
    }

			
	char read_buf[100]={0};  //接收消息缓冲区
	int  read_size=0;        //实际接收信息长度
	
    int maxfd = soc_fd;      //初始化maxfd
		
	fd_set read_fdset;   	//定义一个文件描述符集合
	fd_set backup_set;  	//定义一个备份集合
	
    //清空集合
	FD_ZERO(&backup_set);
	
	//将soc_fd 加入backup_set集合
	FD_SET(soc_fd, &backup_set);
	
	printf("soc_fd =%d\n", soc_fd);
	while(1)
	{
		printf("maxfd = %d\n", maxfd);
		printf("client num = %d\n", acc_fd_num);
		//重置read_fdset集合
		read_fdset = backup_set;
		
		
		//多路复用
		int ret = select(maxfd +1, &read_fdset, NULL, NULL, NULL);
		if(ret < 0)
        {
           perror("select error!\n");
		   return (void*)-1;		   
        }
		else if(0 == ret) //这句判断可不写 没有设置超时控制 
		{
			continue;
		}
		
		//有新的客户端请求连接
		if(FD_ISSET(soc_fd, &read_fdset))
		{		
			acc_fd = accept(soc_fd, (struct sockaddr *)&cli_addr, &len);
			if(acc_fd <0)
			{
				perror("accept failed!");
				close(soc_fd);
				return (void*)-1;
				
			}
			printf("from:%s,port:%d--》online\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
			printf("acc_fd=%d\n", acc_fd);
			//判断是否达到连接上限
		   if(6 == acc_fd_num)
		   {
			   printf("too many client!\n");
			   continue;
		   }
				
			//已连接客户端加1
			acc_fd_num++;  
			
			//将客户端的套接字存入acc_fds[6]
           for(int i = 0; i < 6; i++)
           {
               if(acc_fds[i] < 0)
               {
                   acc_fds[i] = acc_fd;
                   break; //找到合适位置就没必要继续遍历
               }
			   
           }
						
		   //将新的acc_fd 加入backup_set集合
           FD_SET(acc_fd, &backup_set);
			
			//更新maxfd
           if(acc_fd > maxfd)
           {
               maxfd = acc_fd;
           }
		   
		   //判断是否已经处理完事件 ret存储事件个数 当前accept算一件
           if(0 == (--ret))
           {
               continue;
           }
										
		}
		
/*
******                指令 1.进入相册     2.进入音乐   3.上一张图片   4.下一张图片 
******                     5.播放         6.暂停歌     7.上一首歌     8.下一首歌
******                     9.返回
*/ 

	   //从客户端接收信息
		else
		{
			for(int i=0; i<6; i++)
			{
				if(acc_fds[i] < 0) //不是客户端套接字
				{
					continue;
				}
				
				//判断哪个客户端有变化
				if(FD_ISSET(acc_fds[i], &read_fdset))
				{
					bzero(read_buf, sizeof(read_buf));
					read_size = read(acc_fds[i], read_buf, sizeof(read_buf));
					if(read_size < 0)
					{
						perror("read error");
						shutdown(acc_fds[i], SHUT_RDWR);  //挂电话(关闭套接字)
						
						FD_CLR(acc_fds[i], &backup_set); //删除集合中的某个文件描述符
						acc_fds[i] = -1;
						acc_fd_num--;                   //客户端数量减1								
					}
					
					//ret=0表示 客户端下线 去掉相应acc_fd 客户端数量减1
					else if(0 == read_size) 
					{
						printf("one client offline\n");
						shutdown(acc_fds[i], SHUT_RDWR);  //挂电话(关闭套接字)
						
						FD_CLR(acc_fds[i], &backup_set); //删除集合中的某个文件描述符
						acc_fds[i] = -1;
						acc_fd_num--;                   //客户端数量减1
					}
					
					else  //有数据可读
					{
						read_buf[read_size] = '\0';
						if(atoi(read_buf)<=20 && atoi(read_buf)>=1) //1-20 atoi字符串转整形
						{
							printf("from %d receive command :%d\n", acc_fds[i], atoi(read_buf));
							flag_func = atoi(read_buf);
						}
						send(acc_fds[i], read_buf, strlen(read_buf),0);
					}
										
				}//判断哪个客户端有变化
			}//for
									
		}//从客户端接收信息
	}	
	close(soc_fd);
	return NULL;
}

int server_init(const char* ip, int port)//服务器初始化
{
	//1、买手机（创建套接字）
	int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(soc_fd < 0)
	{
		perror("create socket failed!");
		return -1;
	}
	
	//配置IPV4地址结构体	
	struct sockaddr_in ser_addr;
	int len = sizeof(ser_addr);
	bzero(&ser_addr, len);
	

	ser_addr.sin_family = AF_INET; //选择IPV4地址族
	ser_addr.sin_port = htons(port);  //设置端口号（必须转化为网络字节序）

	if(ip == NULL)
	{
		ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //需要进行网络序转换，INADDR_ANY：本地地址
	}
	else
	{
		ser_addr.sin_addr.s_addr = inet_addr(ip); //设置IP地址
	}
	
	//设置 允许重用本地地址和端口 SO_REUSEADDR (若无此设置，一个端口释放后会等待两分钟之后才能再被使用)
	int on=1;
	int ret = setsockopt(soc_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if(ret < 0)
	{
		perror("setsockopt failed!");
		close(soc_fd);
		return -1;
	}
	
	//2、绑定号码（绑定IP+端口号）
	ret = bind(soc_fd, (struct sockaddr *)&ser_addr, len);
	if(ret < 0)
	{
		perror("bind failed!");
		close(soc_fd);
		return -1;
	}
	
	//3、设置铃声（监听套接字）
	ret = listen(soc_fd, 5);
	if(ret < 0)
	{
		perror("listen failed!");
		close(soc_fd);
		return -1;
	}
	return soc_fd;
}




//打开触摸屏，获取触摸屏坐标 线程
void *get_xy_thread_func(void *arg)
{

	int x=0;  
	int y=0;
	
	//打开触摸屏	 
	 fd_ts = open("/dev/input/event0",O_RDWR);
	if(fd_ts < 0)
	{
		perror("open error");
		return (void*)-1;
	}
	
	while(1)
	{
		//在这个函数里面，x, y的值会一直改变，直到松开
		//所以不能放get_x，get_y进去，不然有bug ,get_x，get_y是全局变量
		get_xy(fd_ts, &x, &y); 
		get_x = x;
		get_y = y;
		printf("坐标 x=%d y=%d\n", get_x, get_y);
	}
}

//获取触摸屏坐标 
//x, y的值会一直改变，直到松开
int get_xy(int fd, int *x, int *y)
{
	//定义一个输入事件结构体存储数据
	struct input_event  inpute;

	while(1)
	{
		read(fd, &inpute, sizeof(inpute));
		//printf("type=%d   code=%d   value=%d\n", inpute.type, inpute.code, inpute.value);
		if(inpute.type == 3 && inpute.code == 0)
		{
			*x = inpute.value;
			continue;
		}
		if(inpute.type == 3 && inpute.code == 1)
		{
			*y = inpute.value;
			continue;
		}
		
		if(inpute.type==3 && inpute.code==24 && inpute.value==200)//按下
		{
			//printf("down\n");
		}
		if(inpute.type == 3&& inpute.code == 24 && inpute.value ==0)//松开
		{
			//printf("up\n");
			//printf("%d:%d\n", x, y);
			return 0;
		}
	}
}


void *get_keyboard_thread_func(void *arg) //获取 键盘 输入的数据  线程--
{
	while(1)
	{
		scanf("%d", &flag_func);
	}
}