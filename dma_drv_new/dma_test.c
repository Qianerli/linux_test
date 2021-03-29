#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>

#define COPY_NO_DMA _IO('d',0)
#define COPY_MTOM _IO('d',1)



int main(int argc, char **argv)
{
    int fd;
    int n, cmd;
    
    fd = open("/dev/dma_drv", O_RDWR);
    if(fd < 0)
    {
        printf("can't open\n");
        return -1;
    }




    
    
    ioctl(fd, COPY_NO_DMA);
    

    ioctl(fd, COPY_MTOM);

    close(fd);


    
    return 0;
}