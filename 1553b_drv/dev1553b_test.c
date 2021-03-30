#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

//ioctl命令
#define DEV1553B_BC _IO('j',0)
#define DEV1553B_RT _IO('j',1)
#define DEV1553B_BM _IO('j',2)
#define DEV1553B_CFG _IO('j',3)
#define DEV1553B_BM_CS _IO('j',4)

int main(int argc, char **argv)
{
	unsigned short fd;
	int i = 0;
	char ch = 0;
	unsigned short w_val[1024] = { 0 };//发送数据,写数据
	unsigned short r_val[1024] = { 0 };//接收数据,读数据
	//配置为bc
	short val_bc[5] = {0x0416,0x0000,0xffff,0xf000,0x0000};
	//配置为rt
	short val_rt[7] = {0x1d17,0x2d00,0xffff,0xf000,0x0000,0x2000,0x8000};//通道5
	//配置为bm
	short val_bm[10] = {0x0412,0x0200,0xffff,0xf000,0x0000,
						0x0000,0x1000,0x2710,0xffff,0xffff};
	//配置命令字
	short val_cfg[64] = {0x4200,0x0820, 0,0x1000,0,0,0,0,
						0x4200,0x0c24, 0,0x1020,0,0,0,0,
	                    0x4200,0x2860, 0,0x1040,0,0,0,0,
	                    0,0,0,0,0,0,0,0};
						
	//1.打开1553b设备，可读可写打开
	fd = open("/dev/dev1553b", O_RDWR);
	if (fd < 0)
	{
		printf("fd can't open!\n");
	}	
		
	//2.初始化要写入的数据内容
	for(i=0; i<255; i++)
	{
		w_val[i] = i+1;	
	}
	
	while(1){
		ch = getchar();
		getchar();

		if(ch=='q')
			break;

		switch(ch){
			case '1':
				//1.测试bc
				//1.1 配置为BC
				ioctl(fd,DEV1553B_BC,&val_bc);					
				//1.2 写入命令字
				ioctl(fd,DEV1553B_CFG,&val_cfg);							
				//1.3 发送数据,将w_val的数据写入到内核fd
				write(fd, &w_val, 256*2);
				break;
				
			case '2':
				//2.测试rt
				//2.1 配置为RT,并启动RT
				ioctl(fd,DEV1553B_RT,&val_rt);				
				break;
			case '3':				
				//2.2 接收数据,将内核fd的数据读到r_val
				read(fd, &r_val, 96*2);
				//打印接收到的数据				
				for(i=0; i<96; i++)
				{
					printf("user-r_val[%d] = %x\n",i,r_val[i]);		
				}				
				break;
				
			case '4':
				//3.测试bm
				//3.1 配置为BM,并且启动BM
				ioctl(fd,DEV1553B_BM,&val_bm);	
				break;
				
			case '5':
				//3.2 测试bm
				ioctl(fd,DEV1553B_BM_CS);					

				break;
			default:
				printf("error input!\n");
				break;
		}
		sleep(1);
	}
	//6.关闭设备
	close(fd);
    return 0;
}
