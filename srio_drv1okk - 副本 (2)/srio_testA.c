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
#define SRIO_DMA _IO('s',0)
#define SRIO_NWRITE _IO('s',1)

//申请DMA空间2MB
#define COPY_BUFF_SIZE (1024*8)*2
#define COPY_SIZE 512

int main(int argc, char **argv)
{
	unsigned long long fd;
	int i = 0;
	char ch = 0;
	unsigned long long w_val[256] = { 0 };
	unsigned long long r_val[256] = { 0 };
	//配置为nwrite
	unsigned int val_nwrite[3] = {0x05000000,512,0x85000000};


	//1.打开srio0设备，可读可写打开
	fd = open("/dev/srio", O_RDWR);
	if (fd < 0)
	{
		printf("fd_srio can't open!\n");
	}
	//初始化写入数据
	for(i=0; i<(COPY_SIZE/8); i++)
	{
		w_val[i] = 0x1111111122222222+i;
		printf("user-write-w_val[%d] = %llx\n",i,w_val[i]);		
	}

	printf("COPY_SIZE = %d\n",COPY_SIZE);	

	while(1){
		ch = getchar();
		getchar();

		if(ch=='q')
			break;

		switch(ch){
			case '1':
				//1.往memory里面写数据
				write(fd, &w_val, COPY_SIZE);
				break;
			case '2':
				//3.配置为NWRITE，启动dma
				ioctl(fd,SRIO_NWRITE,&val_nwrite);
				break;
			case '3':
				//3.测试nread读写
				//3.1 读
				read(fd, &r_val, COPY_SIZE);
				/*
				for(i=0; i<COPY_SIZE/8; i++)
				{
					printf("user-read-r_val[%d] = %llx\n",i,r_val[i]);		
				}
				*/
				break;
			default:
				printf("error input!\n");
				break;
		}
		sleep(1);	
	}
	//4.关闭设备
	close(fd);

    return 0;
}
