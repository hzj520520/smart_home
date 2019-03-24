#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <pthread.h>  

int server_close_flag = 0;     //服务器是否关闭标志 0--没关闭 1--关闭 
void *read_thread_func(void *arg); //读线程 函数

int main()
{
	char serverIp[16];
    char port[6];
 
//1.输入IP
    fprintf(stderr, "Connect to IP:");
	fgets(serverIp, sizeof(serverIp), stdin );
	//把serverIp字符串里的换行符去掉 
	char *pserverIp=serverIp;
	while(*pserverIp)
	{
		if(*pserverIp == '\n')
			*pserverIp = '\0';
		pserverIp++;
	}
	
//2.输入口号
	fprintf(stderr, "port:");
	fgets(port, sizeof(port), stdin );
	
//3.连接服务器	
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
	ser_addr.sin_port = htons(atoi(port));  //设置端口号（必须转化为网络字节序）
	ser_addr.sin_addr.s_addr = inet_addr(serverIp); //设置IP地址
	
	
	//2、打电话（连接服务器端）
	int ret = connect(soc_fd, (struct sockaddr *)&ser_addr, len);
	if(ret < 0)
	{
		perror("connect failed!");
		close(soc_fd);
		return -1;
		
	}
	
	//创建读 线程
	pthread_t read_pth1_id;
	pthread_create(&read_pth1_id, NULL, read_thread_func, (void *)&soc_fd);

/*
*****连接服务器后 发送指令 1.进入相册     2.进入音乐   3.上一张图片   4.下一张图片 
******                     5.播放         6.暂停歌     7.上一首歌     8.下一首歌
*******                    9.返回
*/ 
    char funcbuf[100]={0};
	
	while(1)
	{
		fprintf(stderr, "输入： <1>进入相册     <2>进入音乐   <3>上一张图片   <4>下一张图片\n") ;
		fprintf(stderr, "       <5>播放         <6>暂停歌     <7>上一首歌     <8>下一首歌\n");
        fprintf(stderr, "       <9>返回         <quit>退出程序\n");
		fgets(funcbuf, 100, stdin);    
		
		//把buf字符串里的换行符去掉 buf存储路径 不需要换行符
		char *pfuncbuf=funcbuf;
		while(*pfuncbuf)
		{
			if(*pfuncbuf == '\n')
				*pfuncbuf = '\0';
			pfuncbuf++;
		}
		if(0 == strcmp(funcbuf, "quit"))
		{
			break;
		}
		int ret=write(soc_fd, funcbuf, strlen(funcbuf));
		if(ret < 0)
		{
			perror("write error");
			break;
		}
		
		if(1 == server_close_flag)
		{
			printf("server close\n");
			break;
		}
	
    } 
	
	//6、挂电话（关闭套接字）
	close(soc_fd);
	return 0;
}


void *read_thread_func(void *arg)
{
	//获取自己的线程TID号，并把自己分离
	pthread_detach(pthread_self());
	
	int soc_fd = *((int *)arg);
	char buf[100]={0};
	int ret=0;

	while(1)
	{
		bzero(buf, 100);
		ret = read(soc_fd, buf, 100);
		//当客户端退出，就删除节点，显示客户端下线，退出线程
		if(ret == 0 )
		{
			printf("read exit\n");
			server_close_flag=1; //服务器关闭标志
			pthread_exit(NULL); //退出线程
		}
		printf("read from server :%s\n", buf);
	}
}

