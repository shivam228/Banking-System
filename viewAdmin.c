#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
struct admin{
  int id;
  char pwd[50];
  char name[50];
};
int main(){
  int fd=open("a_acc_d",O_CREAT|O_RDWR,S_IRWXU);
  struct admin a1;
  printf("Admin Users Details\n");
  while(read(fd,&a1,sizeof(a1))>0){
    printf("%d\n",a1.id);
    printf("%s\n",a1.pwd);
    printf("%s\n"a1.name);
  }
  return 0;
}
