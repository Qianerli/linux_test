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

#define DEV1553B_CFG _IO('j',3)


int main(int argc, char **argv)
{
	unsigned short fd;
	int i = 0;
	unsigned short w_val[1000] = { 0 };
	unsigned short r_val[1000] = { 0 };
	short val_bc[5] = {0x0416,0x0000,0xffff,0xf000,0x0000};
	
	short val_cfg[64] = {0x4200,0x0820, 0,0x1000,0,0,0,0,
						0x4200,0x0c24, 0,0x1020,0,0,0,0,
	                    0x4200,0x2860, 0,0x1020,0,0,0,0,
	                    0,0,0,0,0,0,0,0};
						
	//1.打开1553b设备，可读可写打开
	fd = open("/dev/dev1553b", O_RDWR);
	if (fd < 0)
	{
		printf("fd can't open!\n");
	}	
	
	
	//写入数据初始化
	for(i=0; i<255; i++)
	{
		w_val[i] = i+1;	
	}
	
	//配置为BC
	ioctl(fd,DEV1553B_BC,&val_bc[0]);	
	
	
	//写入命令字
	ioctl(fd,DEV1553B_CFG,&val_cfg[0]);

					
	//2.将w_val的数据写入到内核fd
	write(fd, &w_val[0], 512);
	

	
	//write(fd+16, &w_val[0], 16);
	//printf("user-w_val[%d] = %d\n",i,w_val[i]);
	//usleep(1);
	//3.将内核fd的数据读到r_val
	//read(fd, &r_val[0], 255);
	
	
	
	/*		
	for(i=0; i<0x10f; i++)
	{
		printf("user-r_val[%d] = %d\n",i,r_val[i]);		
	}
	*/
	



	//4.关闭设备
	close(fd);

    return 0;
}
