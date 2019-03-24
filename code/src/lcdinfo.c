#include "lcdinfo.h"

static unsigned char g_color_buf[FB_SIZE]={0};

//初始化lcd
struct LcdInfo* lcd_init(const char *dev)
{
	struct LcdInfo* lcd = malloc(sizeof(struct LcdInfo));
	lcd->fd = open(dev, O_RDWR);
	if(lcd->fd < 0)
	{
		perror("open error");
		goto error;
	}
	
	//获取lcd信息
	struct fb_var_screeninfo info;
	int ret  = ioctl(lcd->fd, FBIOGET_VSCREENINFO, &info);
	if(ret < 0)
	{
		perror("get info error");
		goto error;
	}
	
	printf("\n------lcd--information---\n");
	printf("xres=%d yres=%d\n", info.xres, info.yres);
	printf("xres_virtual=%d yres_virtual=%d\n", info.xres_virtual, info.yres_virtual);
	printf("bits_per_pixel=%d\n", info.bits_per_pixel);        // 一个像素多少位   
	printf("------lcd--information---\n\n");
	
	//映射
	lcd->lcdW = info.xres_virtual;
	lcd->lcdH = info.yres_virtual;
	lcd->lcdsize = lcd->lcdW*lcd->lcdH*info.bits_per_pixel/8; //(32位/8 = 4字节)
	
	lcd->mmp = mmap(NULL, lcd->lcdsize, PROT_READ|PROT_WRITE, MAP_SHARED, lcd->fd, 0);
	if(lcd->mmp == (void*)-1)
	{
		perror("mmp error");
		goto error;
	}
	return lcd;
error:
	lcd_destroy(lcd);
	return NULL;
}


//销毁
bool lcd_destroy(struct LcdInfo* lcd)
{
	if(lcd != NULL)
	{
		if(lcd->mmp != (void*)-1)
		{
			munmap(lcd->mmp, lcd->lcdsize); /* 取消内存映射 */
		}
		if(lcd->fd > 0)
		{
			close(lcd->fd);
		}
		free(lcd);
		return true;
	}
	else
		return false;
}

//绘制点
bool lcd_draw_point(struct LcdInfo* lcd, int x, int y, unsigned int  color)
{
	if(lcd == NULL) return false;
	unsigned int *p = lcd->mmp+y*lcd->lcdW+x;
	p[0] = color;
	return true;
}


//绘区域颜色
bool lcd_draw_rect(struct LcdInfo* lcd, int x, int y, int w, int h, unsigned int  color)
{
	if(lcd == NULL) return false;
	unsigned int *p = lcd->mmp+y*lcd->lcdW+x;
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<w; j++)
		{
			*(p+j) = color;
		}
		p += lcd->lcdW;
	}
	return true;
}


//为rgbData->rgb分配(malloc)一段内存  不用时要free掉
bool jpeg_to_rgb(struct RgbData *rgbData, const char *file)
{
	FILE *infile = fopen(file,"rb");
	if(infile == NULL)
	{
		perror("fopen file error");
		return false;
	}
	//1.创建解码对象,错误处理
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	//初始化标准错误处理
	cinfo.err = jpeg_std_error(&jerr);
	//初始化解码对象
	jpeg_create_decompress(&cinfo);
	
	//2.绑定解码文件
	jpeg_stdio_src(&cinfo, infile);  // 不同点 没有jpg数据 从文件获取
	//3.获取文件头信息
	jpeg_read_header(&cinfo, true);
	
	//4.设置解码参数
	//5.开始解码
	jpeg_start_decompress(&cinfo);
	
	//printf("cinfo.output_components=%d\n", cinfo.output_components);
	
	//计算一行像素所占用的字节数  cinfo.output_components 3字节 RGB
	int row_stride = cinfo.output_width * cinfo.output_components;
	//分配一行像素所要占用的空间(用来存储RGB数据)
	unsigned char *buffer = (unsigned char *)malloc(row_stride);
	if(buffer==NULL)
	{
		perror("malloc buffer error\n");
		fclose(infile);

		//结束--释放对象
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		return false;
	}

	
	//初始化数据结构体
	rgbData->w = cinfo.output_width;
	rgbData->h = cinfo.output_height;
	rgbData->pixel_types = cinfo.output_components;
	
	//rgbData->rgb: malloc一段内存  用来存储图片rgb-》lcd上rgb数据
	rgbData->rgb = malloc(rgbData->w * rgbData->h * cinfo.output_components);
	if(rgbData->rgb==NULL)
	{
		perror("malloc rgbData->rgb error\n");
		fclose(infile);
		free(buffer);

		//结束--释放对象
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		return false;
	}

	//从文件解码一行一行数据
	while (cinfo.output_scanline < cinfo.output_height) 
	{
		//读取一行数据存储在buffer中
		jpeg_read_scanlines(&cinfo, &buffer, 1); 
		for(int i=0; i<rgbData->w; i++)
		{
			unsigned char buf[3];
			memcpy(buf, buffer+i*3, 3);
			unsigned char t = buf[0];
			buf[0] = buf[2];
			buf[2] = t;
			//memcpy(bmprowbuffer+i*3, buf, 3);
			memcpy(rgbData->rgb+(cinfo.output_scanline-1)*row_stride+i*3, buf, 3);
		}			              
	}
	free(buffer);
	fclose(infile);
	//结束--释放对象
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	return true;
}


//lcd 显示jpeg图片
bool lcd_draw_picture(struct LcdInfo* lcd , int x, int y, const char *picture)
{
	struct RgbData rgbData;
	jpeg_to_rgb(&rgbData, picture);//
	
	if(lcd == NULL) return false;
	unsigned int *p = lcd->mmp+y*lcd->lcdW+x;
	
	int rowpicsize = rgbData.w*rgbData.pixel_types;
	
	int minx = rgbData.w > lcd->lcdW?lcd->lcdW:rgbData.w;
	int miny = rgbData.h > lcd->lcdH?lcd->lcdH:rgbData.h;
	for(int i=0; i<miny; i++)
	{
		for(int j=0;j<minx; j++)
		{
			memcpy(p+j, rgbData.rgb+i*rowpicsize+j*3, 3);
		}
		p += lcd->lcdW;
	}
	return true;
}


//lcd通过数据显示
bool lcd_draw_data(struct LcdInfo* lcd, int x, int y, int w, int h, unsigned char* rgb_data)
{
    if(lcd == NULL) return false;
	unsigned int *p = lcd->mmp+y*lcd->lcdW+x;
	
	int rowpicsize = w*3;
	
	int minx = w > lcd->lcdW?lcd->lcdW:w;
	int miny = h > lcd->lcdH?lcd->lcdH:h;
	for(int i=0; i<miny; i++)
	{
		for(int j=0;j<minx; j++)
		{
			memcpy(p+j, rgb_data+i*rowpicsize+j*3, 3);
		}
		p += lcd->lcdW;
	}
	return true;
}


 //把图片放缩为w*h
 //300、500都有bug
 //400、 600 可以
bool pictrue_zoom_wh(struct RgbData* rgbdata, int w, int h)
{
	if(rgbdata == NULL) return false;

	int zoom_w = 100*w/rgbdata->w;  
	int zoom_h = 100*h/rgbdata->h;  
	
	//申请空间存储--放缩后的图像数据
	unsigned char *timage = (unsigned char *)malloc(w*h*3);
	if(timage == NULL) return false;
	
	int x,y;
	for(y=0; y<h; y++)
	{
		for(x=0; x<w; x++)
		{
			timage[3*(y*w+x)]   = rgbdata->rgb[3*((y*100/zoom_h)*rgbdata->w+(x*100/zoom_w))];
			timage[3*(y*w+x)+1] = rgbdata->rgb[3*((y*100/zoom_h)*rgbdata->w+(x*100/zoom_w))+1];
			timage[3*(y*w+x)+2] = rgbdata->rgb[3*((y*100/zoom_h)*rgbdata->w+(x*100/zoom_w))+2];
			
		}
	}
	free(rgbdata->rgb);    //free掉旧的空间
	rgbdata->rgb = timage; //赋值新的空间
	rgbdata->w = w;
	rgbdata->h = h;
	return true;
}



//显示摄像头捕捉 pjpg_buf----》jpg数据 解码 转换成rgb
int show_video_data(struct LcdInfo* lcd, unsigned int x,unsigned int y, unsigned char *pjpg_buf, unsigned int jpg_buf_size)  
{
	/*定义解码对象，错误处理对象*/
	struct 	jpeg_decompress_struct 	cinfo;
	struct 	jpeg_error_mgr 			jerr;	
	
	unsigned char 	*pcolor_buf = g_color_buf;
	unsigned char 	*pjpg;
	
	unsigned int 	i=0;
	unsigned int	color =0;

	
	unsigned int 	x_s = x;
	//unsigned int 	x_e ;	
	//unsigned int 	y_e ;
			
	pjpg = pjpg_buf;

	/*注册出错处理*/
	cinfo.err = jpeg_std_error(&jerr);

	/*创建解码*/
	jpeg_create_decompress(&cinfo);

	/*直接解码内存数据*/		
	jpeg_mem_src(&cinfo, pjpg, jpg_buf_size);
	
	/*读文件头*/
	jpeg_read_header(&cinfo, TRUE);

	/*开始解码*/
	jpeg_start_decompress(&cinfo);	
	
	//x_e	= x_s+cinfo.output_width;
	//y_e	= y  +cinfo.output_height;	

	/*读解码数据*/
	while(cinfo.output_scanline < cinfo.output_height )
	{		
		pcolor_buf = g_color_buf;
			
		/* 读取jpg一行的rgb值 */
		jpeg_read_scanlines(&cinfo,&pcolor_buf,1);
		
		for(i=0; i<cinfo.output_width; i++)
		{
			/* 获取rgb值 */
			color = 		*(pcolor_buf+2);
			color = color | *(pcolor_buf+1)<<8;
			color = color | *(pcolor_buf)<<16;
			
			/* 显示像素点 */
			lcd_draw_point(lcd, x, y, color);
			
			pcolor_buf +=3;
			
			x++;
		}
		
		/* 换行 */
		y++;			
		
		x = x_s;
		
	}		
			
	/*解码完成*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	return 0;
}

