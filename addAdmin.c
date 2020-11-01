#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
struct admin{
  int id;
  char psd[50];
  char name[50];
};
int main(){
  int fd=open("a_acc_d",O_CREAT|O_RDWR,S_IRWXU);
  printf("Enter admin Details\n");
  struct admin a1;
  printf("Enter admin id:");
  scanf("%d",&a1.id);
  printf("\n");
  printf("Enter admin password:");
  scanf("%s",a1.psd);
  printf("\n");
  printf("Enter admin name:");
  scanf("%s",a1.name);
  printf("\n");
  lseek(fd,0,SEEK_END);
  write(fd,&a1,sizeof(a1));
  close(fd);
  return 0;
}
