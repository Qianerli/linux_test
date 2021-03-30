#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	unsigned char fd;
	int i = 0;
	unsigned char w_val[1000] = { 0 };
	unsigned char r_val[1000] = { 0 };

	//1.打开1553b设备，可读可写打开
	fd = open("/dev/dev1553b", O_RDWR);
	if (fd < 0)
	{
		printf("fd can't open!\n");
	}
	
	
	for(i=1; i<0x100; i++)
	{
		w_val[i] = i;	
	}
	

					
	//2.将w_val的数据写入到内核fd
	write(fd, &w_val[0], 255);
	//printf("user-w_val[%d] = %d\n",i,w_val[i]);
	//usleep(1);
	//3.将内核fd的数据读到r_val
	read(fd, &r_val[0], 255);
	
	
	
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
