#include "myrfid.h"



/*******************************************************************************
 * 函数名    :  tty_setattr
 * 功能描述  :  配置串口属性
 * 返回值说明:  成功返回0，失败返回-1
*******************************************************************************/
int tty_setattr(int tty_fd) 
{
	//声明设置串口的结构体
	struct termios old_cfg, new_cfg;
	
	//先清空结构体
	bzero(&old_cfg, sizeof(old_cfg));
	bzero(&new_cfg, sizeof(new_cfg));
	
	//1、保存串口原先属性
	if( tcgetattr(tty_fd, &old_cfg) < 0)
	{
		perror("tcgetattr failed");
		return -1;
	}
	
	//cfmakeraw()设置终端属性，就是设置termios结构中的各个参数。
	//cfmakeraw(&new_cfg);
		
	//2、激活本地连接和接收使能			
	 new_cfg.c_cflag  |=  CLOCAL | CREAD; 
	
	//3、设置波特率 io口
	cfsetispeed(&new_cfg, B9600);  //B9600
	cfsetospeed(&new_cfg, B9600);
	
	//4．设置数据位大小
	new_cfg.c_cflag &= ~CSIZE; /* 用数据位掩码清空数据位设置 */
	new_cfg.c_cflag |= CS8;  //CS5,CS6  CS8--->8位
	
	//5、设置无奇偶检验
	new_cfg.c_cflag &= ~PARENB;
	
	//6、设置停止位
	new_cfg.c_cflag &= ~CSTOPB; /* 将停止位设置为1位 */
	//new_cfg.c_cflag |= CSTOPB; /* 将停止位设置为两个比特 */

	//7、设置最少字符和等待时间
	new_cfg.c_cc[VTIME] = 0;
	new_cfg.c_cc[VMIN]  = 1;
	
	//8、清空串口缓冲区
	tcflush(tty_fd, TCIOFLUSH);
	
	//9、让配置立即生效
	if( (tcsetattr(tty_fd, TCSANOW, &new_cfg)) <0)
	{
		perror("tcsetattr failed");
		return -1;
	}
	
	return 0;
}


/*******************************************************************************
 * 函数名    :   BCC_VALUE
 * 功能描述  :   计算校验和
 * 返回值说明:   返回校验和BCC
*******************************************************************************/
unsigned char BCC_VALUE(unsigned char *buf)
{
	unsigned char BCC=0;
	int i;
	for(i=0; i<buf[0]-2; i++) //buf[0]--->帧长
	{			
		BCC ^= buf[i];
	}
	
	return (~BCC); 	
}


/*******************************************************************************
 * 函数名    :  send_A
 * 功能描述：:  发送A命令 判断附近是否有卡
 * 返回值说明:  成功返回0，失败返回-1
*******************************************************************************/
int send_A(int tty_fd)
{

	int ret=0;
	unsigned char buf[8];
	
	bzero(buf, 8);	
	buf[0] = 0x07;//帧长 7字节
	buf[1] = 0x02;//命令类型 ---> ISO14443A 类命令
	buf[2] = 0x41;//选择'A'命令
	buf[3] = 0x01;//信息长度
	buf[4] = 0x52;//信息内容 请求模式---> ALL
	buf[5] = BCC_VALUE(buf); //获取校验和
	buf[6] = 0x03; //结束符
		
	unsigned char rbuf[10];//接收信息缓冲区
	bzero(rbuf, 10);
	
	fd_set readfd_set; //多路复用 读就绪集合
	FD_ZERO(&readfd_set);
	FD_SET(tty_fd, &readfd_set);
	
	//发送A命令给RFID卡中
	write(tty_fd, buf, 7);
	
	//定义超时时间设置的结构体
	struct timeval v;
	
	v.tv_sec = 1; //1秒
	v.tv_usec = 0;
	
	 //多路复用 不阻塞，1s内检测文件描述符是否有读就绪，把没有读就绪的文件描述符踢出集合
	ret = select(tty_fd+1, &readfd_set, NULL ,NULL, &v);
	switch( ret )
	{
		case -1:
				perror("select tty_fd error");
				break;
		case 0:
				printf("select time out\n");
				break;
				
		default:
				ret = read(tty_fd, rbuf, 10);
		
	}

	if(ret < 0 )
	{
		perror("read failed!");
	}
	else if(ret > 0)//有数据可读
	{
		//状态位rbuf[2]为0 --> 表示请求成功
		if(rbuf[2] == 0)
		{
			printf("exist card\n");
			return 0;
		}
	}
				
	return -1;
	
}


/*******************************************************************************
 * 函数名    send_B
 * 功能描述：发送B命令（注意: 得配合A命令，即先发A命令,再发B命令）
 * 返回值说明:  成功返回卡号ID，失败返回0
*******************************************************************************/
unsigned int send_B(int tty_fd)
{
	unsigned char buf[9];
	bzero(buf, 9);
	
	buf[0] = 0x08;//帧长
	buf[1] = 0x02;//命令类型ISO14443A 类命令
	buf[2] = 0x42;//选择'B'命令
	buf[3] = 0x02;//信息长度
	buf[4] = 0x93;//信息内容
	buf[5] = 0X00;//位计数
	buf[6] = BCC_VALUE(buf); //获取校验和
	buf[7] = 0x03; //结束符
		
	unsigned char rbuf[11];
	bzero(rbuf, 11);
	
	int ret;
	unsigned int card_id=0;

	//发送B命令给RFID卡中
	write(tty_fd, buf, 8);
	
	//读取RFID发送的数据
	ret = read(tty_fd, rbuf, 11);
	if(ret < 0 )
	{
		perror("read failed!");
	}
	else if(ret > 0)
	{
		
		if(rbuf[2] == 0)
		{
			//printf("get cardid\n");
			
			card_id = (rbuf[7]<<24) | (rbuf[6] <<16) | (rbuf[5]<<8) | rbuf[4];
			
			return card_id;
		}	
	}	
	return 0;
	
}

/*******************************************************************************
 * 函数名    get_card_id
 * 功能描述：获取卡号ID, 失败返回0
*******************************************************************************/

unsigned int get_card_id(int tty_fd)
{
		if(0 == send_A(tty_fd))
		{
			unsigned int card_id = send_B(tty_fd);
			if(card_id > 0)
			{
				//printf("card_id:0x%x\n",card_id);
				return card_id;
			}		
		}
		return 0;
}
