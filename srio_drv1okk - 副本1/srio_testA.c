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
#define SRIO_NWRITE _IO('s',0)
#define SRIO_NREAD _IO('s',1)
#define SRIO_DOORBELL _IO('s',2)
#define SRIO_NREAD_SIZE _IO('s',3)

int main(int argc, char **argv)
{
	unsigned long long fd;
	int i = 0;
	char ch = 0;
	unsigned long long w_val[256] = { 0 };
	unsigned long long r_val[256] = { 0 };
	//配置为nwrite
	unsigned int val_nwrite[4] = {0x00000005,0x5ff00000,0x111000ff,0x80000005};
	unsigned int nwrite_size = (val_nwrite[1]&0x0ff00000)/0x100000+1;
	//配置为nread
	unsigned int val_nread[4] = {0x00000002,0x4ff00000,0x111000ff,0x80000002};
	unsigned int nread_size = (val_nread[1]&0x0ff00000)/0x100000+1;
	//配置为nread
	unsigned int val_doorbell[4] = {0x000f0f0a,0x4ff00000,0x000200ff,0x8000000a};

	//1.打开srio0设备，可读可写打开
	fd = open("/dev/srio", O_RDWR);
	if (fd < 0)
	{
		printf("fd_srio can't open!\n");
	}
	//初始化写入数据
	for(i=0; i<(nread_size/8); i++)
	{
		w_val[i] = 0x1111111122222222+i;
		printf("user-write-w_val[%d] = %llx\n",i,w_val[i]);		
	}

	printf("nread_size = %d\n",nread_size);	

	while(1){
		ch = getchar();
		getchar();

		if(ch=='q')
			break;

		switch(ch){
			case '1':
				//1.配置为NWRITE
				ioctl(fd,SRIO_NWRITE,&val_nwrite);
				break;
			case '2':
				//2.配置为NREAD
				ioctl(fd,SRIO_NREAD,&val_nread);
				break;
			case '3':
				//3.配置为DOORBELL
				ioctl(fd,SRIO_DOORBELL,&val_doorbell);
				break;
			case '4':
				//4.测试nread读写
				//4.1 读
				read(fd, &r_val, nread_size);
				for(i=0; i<nread_size/8; i++)
				{
					printf("user-read-r_val[%d] = %llx\n",i,r_val[i]);		
				}
				break;
			case '5':
				//5.测试nwrite读写
				//5. 写
				write(fd, &w_val, nwrite_size);

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
