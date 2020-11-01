#include<stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{ 
int n=1000;
int fd1 = open("count.txt",O_RDWR|O_CREAT,0666);
write(fd1,&n,sizeof(n));
return 0;
}

