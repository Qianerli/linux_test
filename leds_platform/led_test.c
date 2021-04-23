#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

//ioctl命令
#define LED_PLATFORM _IO('a',0)


int main(int argc, char **argv)
{
    int i = 0;
	char ch = 0;
	unsigned int fd;
	unsigned int led[5] = {0x00020000,0x00010000,0x80000000,0x40000000,0};
    
	//1.打开led设备，可读可写打开
	fd = open("/dev/led", O_RDWR);
	if (fd < 0)
	{
		printf("fd can't open!\n");
	}
	
	//2.初始化要写入的数据内容
	//putdate = 0x00000000;
    
   while(1){
		ch = getchar();
		getchar();

		if(ch=='q')
			break;

		switch(ch){
			case '1':
					printf(">=====================<\n");
					ioctl(fd,LED_PLATFORM,led[3]);
					sleep(1);
				break;
			default:
				printf("error input!\n");
				break;
		}
		sleep(1);
	}			    
    close(fd);
    
    return 0;
}