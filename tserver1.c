#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<error.h>
#include<errno.h>
#include<stdlib.h>
#include<signal.h>
#define MAX 512
int count1 =1000;
struct normal_account{
  char acc_name[MAX];
  char acc_pwd[MAX];
  int acc_num;
  float bal;
};
struct joint_account{
  char acc_name1[MAX];
  char acc_name2[MAX];
  char acc_pwd[MAX];
  int acc_num;
  float bal;
};
struct joint_account_d{
  char acc_pwd[MAX];
  int acc_num;
};
struct admin{
  int id;
  char pwd[MAX];
  char name[MAX];
};
int count=0;
int count2=0;
void myhandler(int s){
  if(s==SIGCHLD){
    printf("client exits\n");
    count--;
    printf("%d clients are live\n",count);
  }
}
int count_joint =100;
int sfd;
int cfd;
int nfd;
int jfd1;
int jfd2;
int afd;
int count_fd;//for count file
int acc_num;
char pwd[MAX];
int jacc_num;
char jpwd[MAX];
void adminLogin();
void adminManage();
void adminViewNormalAccountDetails();
void adminViewJointAccountDetails();
void adminDeleteNormalAccount();
void adminDeleteJointAccount();
void Login_Screen();
void normalCreate();
void normalLogin();
void Normal_Account_Options();
void Normal_Account_Details();
void Normal_Change_Password();
void Normal_Withdraw_Money();
void Transfer_money();
void Normal_Deposit_Money();
void Normal_View_Account_Balance();
void jointCreate();
void jointLogin();
void jointManage();
void jointAccountDetails();
void jointAccountChangePassword();
void jointAccountWithdrawMoney();
void jointAccountDepositMoney();


//*****************************************************NORMAL USER************************************************************************

//This function is used to display details of valid user
void Normal_Account_Details()
{      
  struct normal_account a1;
  printf("Your account details are-> \n");
  lseek(nfd,0,SEEK_SET);// set pointer position at starting of file
  while(read(nfd,&a1,sizeof(a1))>0)
  {
  if(acc_num==a1.acc_num && strcmp(pwd,a1.acc_pwd)==0)//validating account number and password from file
  {
  printf("credentials matched valid user\n");
  write(cfd,&a1,sizeof(a1));// write the details to the clientent if match found
  printf("details send to clientent\n");
  break;
  }
  }
  Normal_Account_Options();
}

//This function is used to change password for normal account 
void Normal_Change_Password()
{
  printf("normal change password\n");
  char npwd[MAX];
  read(cfd,npwd,MAX);//reading new password from client
  lseek(nfd,0,SEEK_SET);
  struct normal_account a1;
  while(read(nfd,&a1,sizeof(a1))>0)
  {
  if(acc_num==a1.acc_num)
  {
  printf("Account Found\n");
  strcpy(a1.acc_pwd,npwd);//copy new password in place old password
  break;
  }
  }
  lseek(nfd,-1*sizeof(a1),SEEK_CUR);// move pointer to starting to record
  write(nfd,&a1,sizeof(a1));// replace the old record with new one
  printf("password changed\n");
  Login_Screen();
}

// This function is used to withdraw money from account
void Normal_Withdraw_Money()
{
  printf("Withdraw Money\n");
  float amt;
  read(cfd,&amt,sizeof(amt));//reading amount to be withdrawn from clientent
  lseek(nfd,0,SEEK_SET);
  struct normal_account a1;
  while(read(nfd,&a1,sizeof(a1))>0)
  {
  if(acc_num==a1.acc_num)
  {
  printf("account found\n");
  if(amt > a1.bal)
  {
  printf("insufficient balance!!!\n");
  write(cfd,&a1,sizeof(a1));
  }
  else
  {
  a1.bal=a1.bal-amt;
  write(cfd,&a1,sizeof(a1));
  }
  break;
   }
  }
  if(amt < a1.bal)
  {
  lseek(nfd,-1*sizeof(a1),SEEK_CUR);
  write(nfd,&a1,sizeof(a1));// updating balance to file 
  printf("Amount withdrawn\n");
  }
  Normal_Account_Options();
}
// This function is used to deposit money to account
void Normal_Deposit_Money()
{
  printf("Deposit Money\n");
  float amt;
  read(cfd,&amt,sizeof(amt));//reading amount entered by clientent
  lseek(nfd,0,SEEK_SET);
  struct normal_account a1;
  while(read(nfd,&a1,sizeof(a1))>0){
  if(acc_num==a1.acc_num){
  printf("Account found\n");
  a1.bal=a1.bal+amt;
  break;
  }
  }
  lseek(nfd,-1*sizeof(a1),SEEK_CUR);
  write(nfd,&a1,sizeof(a1));
  write(cfd,&a1,sizeof(a1));
  printf("Amount Deposited\n");
  Normal_Account_Options();
}

// This function is used to view the account balance
  void Normal_View_Account_Balance()
  {
  float amt;
  printf("Your account balance details are as follows:\n");
  lseek(nfd,0,SEEK_SET);
  struct normal_account a1;
  while(read(nfd,&a1,sizeof(a1))>0)
  {
  printf("%d %d %s %s\n",acc_num,a1.acc_num,a1.acc_pwd,pwd);
  if(acc_num==a1.acc_num){
  if(strcmp(pwd,a1.acc_pwd)==0)
   {
  printf("pass match\n");
   }
   amt=a1.bal;
   write(cfd,&amt,sizeof(float));
   printf("details send\n");
   break;
    }
  }
  Normal_Account_Options();
}

void Transfer_money()// transfers the money from one account to another
{
int res =0;
float ax;
int accx;
float bala;
 read(cfd,&accx,sizeof(accx));
 read(cfd,&ax,sizeof(ax));
 struct normal_account a1;
  lseek(nfd,0,SEEK_SET);
  while(read(nfd,&a1,sizeof(a1))>0)//searching for account and password in file 
  {
    if(accx==a1.acc_num)// if found means user is valid
    {
      res=1;
      break;
    }
  }
  if(res==0)
  {
  printf("account not found");
  write(cfd,&res,sizeof(int));
  Normal_Account_Options();
  }
  else
  {
  write(cfd,&res,sizeof(int));
  lseek(nfd,0,SEEK_SET);
  while(read(nfd,&a1,sizeof(a1))>0)
  {
  if(acc_num==a1.acc_num)
  {
  printf("account found\n");
  if(ax > a1.bal)
  {
  bala = a1.bal;
  printf("insufficient balance!!!\n");
  write(cfd,&bala,sizeof(int));
  write(cfd,&a1,sizeof(a1));
  }
  else
  {
  bala = a1.bal;
  a1.bal=a1.bal-ax;
  write(cfd,&bala,sizeof(int));
  write(cfd,&a1,sizeof(a1));
  }
  break;
   }
   }
   if(ax < bala)
  {
  lseek(nfd,-1*sizeof(a1),SEEK_CUR);
  write(nfd,&a1,sizeof(a1));// updating balance to file 
  printf("Amount withdrawn\n");
  }
  lseek(nfd,0,SEEK_SET);
   if(ax<=bala)
   { 
  while(read(nfd,&a1,sizeof(a1))>0)//searching for account and password in file 
  {
    if(accx==a1.acc_num)// if found means user is valid
    {
      printf("account found");
      a1.bal=a1.bal+ax;
      break;
    }
    
  }
  }
  lseek(nfd,-1*sizeof(a1),SEEK_CUR);
  write(nfd,&a1,sizeof(a1));
  printf("Amount Deposited\n");
  Normal_Account_Options();
  }
  
}
// Normal Account management portal
void Normal_Account_Options()
{
  
  printf("normalManage\n");
  int res;
  read(cfd,&res,sizeof(int));
  switch(res){
    case 1:    
               Normal_Deposit_Money();
               break;
    case 2:
  		Normal_Withdraw_Money();
  		break;
    case 3:
                Normal_View_Account_Balance();
  		break;
    case 4:     
                Normal_Change_Password();
  		break;
    case 5:
              Normal_Account_Details();
               break;
    case 6:
              Transfer_money();
               break;
    default:
               Login_Screen();
  }
}

// validating normal user login
  void normalLogin()
  {
  read(cfd,&acc_num,sizeof(acc_num));
  read(cfd,pwd,sizeof(pwd));
  printf("clientent details:%d %s\n",acc_num,pwd);
  int res=0;
  struct normal_account a1;
  lseek(nfd,0,SEEK_SET);
  while(read(nfd,&a1,sizeof(a1))>0)//searching for account and password in file 
  {
    if(acc_num==a1.acc_num && strcmp(pwd,a1.acc_pwd)==0)// if found means user is valid
    {
      res=1;
      break;
    }
  }
  write(cfd,&res,sizeof(int));// write result to clientent if res =1 login successful else not 
  if(res==0)
  {
    printf("Wrong username or password\n");
    Login_Screen();
  }
  else
  {
    printf("Login details matched\n");
    Normal_Account_Options();
  }
}

//  Create Normal Account
void normalCreate()
{
 printf("normalCreate\n");
  struct normal_account a1;
  int inc;
  lseek(count_fd,0,SEEK_SET);
  read(count_fd,&inc,sizeof(int));
  write(cfd,&inc,sizeof(inc));
  lseek(count_fd,0,SEEK_SET);
  inc++;
  write(count_fd,&inc,sizeof(inc));
  read(cfd,&a1,sizeof(a1));//read normal account details from clientent
  lseek(nfd,0,SEEK_END);// move file pointer to end of file
  write(nfd,&a1,sizeof(a1));//writing clientent data to file
  printf("Normal Account Created\n");
  Login_Screen();
  }

//*********************************************** joint account holder********************************************************************

// view Joint Account Details
void jointAccountDetails()
{
  printf("JointAccount Details");
  int offset=sizeof(struct joint_account)*count2;
  lseek(jfd1,offset,SEEK_SET);
  struct flock lock;
  lock.l_type=F_RDLCK;
  lock.l_whence=SEEK_SET;
  lock.l_start=offset;
  lock.l_len=sizeof(struct joint_account);
  printf("Trying to acquire read lock on data\n");
  int r=fcntl(jfd1,F_SETLKW,&lock);
  printf("Read lock acquired\n");
  struct joint_account j1;
  read(jfd1,&j1,sizeof(struct joint_account));
  printf("details are %s %s %s %d %f\n",j1.acc_name1,j1.acc_name2,j1.acc_pwd,j1.acc_num,j1.bal);
  write(cfd,&j1,sizeof(struct joint_account));
  char c;
  read(cfd,&c,1);
  lock.l_type=F_UNLCK;
  printf("releasing read lock\n");
  fcntl(jfd1,F_SETLK,&lock);
  printf("lock released\n");
  jointManage();
}

// change Password For Joint Account
void jointAccountChangePassword()
{
  printf("joint change password\n");
  char tpwd[MAX];
  read(cfd,tpwd,MAX);
  int offset=sizeof(struct joint_account)*count2;
  int offset1=sizeof(struct joint_account_d)*count2;
  lseek(jfd1,offset,SEEK_SET);
  lseek(jfd2,offset1,SEEK_SET);
  struct flock lock;
  lock.l_type=F_WRLCK;
  lock.l_whence=SEEK_SET;
  lock.l_start=offset;
  lock.l_len=sizeof(struct joint_account);
  printf("Trying to acquire write lock on data\n");
  int r=fcntl(jfd1,F_SETLKW,&lock);
  printf("write lock acquired\n");
  struct joint_account j1;
  struct joint_account_d j2;
  read(jfd1,&j1,sizeof(struct joint_account));
  read(jfd2,&j2,sizeof(struct joint_account_d));
  printf("details are %s %s %s %d %f\n",j1.acc_name1,j1.acc_name2,j1.acc_pwd,j1.acc_num,j1.bal);
  strcpy(j1.acc_pwd,tpwd);
  strcpy(j2.acc_pwd,tpwd);
  lseek(jfd1,-1*sizeof(struct joint_account),SEEK_CUR);
  write(jfd1,&j1,sizeof(struct joint_account));
  lseek(jfd2,-1*sizeof(struct joint_account_d),SEEK_CUR);
  write(jfd2,&j2,sizeof(struct joint_account_d));
  char m[MAX];
  strcpy(m,"Password Changed successfully\n");
  write(cfd,m,MAX);
  char c;
  read(cfd,&c,1);
  lock.l_type=F_UNLCK;
  printf("releasing write lock\n");
  fcntl(jfd1,F_SETLK,&lock);
  printf("lock released\n");
  jointManage();
}

// Joint Account Withdraw Money
void jointAccountWithdrawMoney()
{
  printf("joint Withdraw Money\n");
  float kamt;
  read(cfd,&kamt,sizeof(float));
  int offset=sizeof(struct joint_account)*count2;
  lseek(jfd1,offset,SEEK_SET);
  struct flock lock;
  lock.l_type=F_WRLCK;
  lock.l_whence=SEEK_SET;
  lock.l_start=offset;
  lock.l_len=sizeof(struct joint_account);
  printf("Trying to acquire write lock on data\n");
  int r=fcntl(jfd1,F_SETLKW,&lock);
  printf("write lock acquired\n");
  struct joint_account j1;
  read(jfd1,&j1,sizeof(struct joint_account));
  printf("details are %s %s %s %d %f\n",j1.acc_name1,j1.acc_name2,j1.acc_pwd,j1.acc_num,j1.bal);
  if(j1.bal<kamt)
  {
  printf("insuffcient balance");
  char m1[MAX];
  strcpy(m1,"insufficient balance...\n");
  write(cfd,m1,MAX);
  }
  else
  {
  j1.bal=j1.bal-kamt;
  lseek(jfd1,-1*sizeof(struct joint_account),SEEK_CUR);
  write(jfd1,&j1,sizeof(struct joint_account));
  char m[MAX];
  strcpy(m,"Amount Withdrawn successfully\n");
  write(cfd,m,MAX);
  }
  int kg = j1.bal;
  write(cfd,&kg,sizeof(kg));
  char c;
  read(cfd,&c,1);
  lock.l_type=F_UNLCK;
  printf("releasing write lock\n");
  fcntl(jfd1,F_SETLK,&lock);
  printf("lock released\n");
  jointManage();
}

//Joint Account Deposit Money
void jointAccountDepositMoney()
{
  printf("joint Deposit Money\n");
  float kamt;
  read(cfd,&kamt,sizeof(float));
  int offset=sizeof(struct joint_account)*count2;
  lseek(jfd1,offset,SEEK_SET);
  struct flock lock;
  lock.l_type=F_WRLCK;
  lock.l_whence=SEEK_SET;
  lock.l_start=offset;
  lock.l_len=sizeof(struct joint_account);
  printf("Trying to acquire write lock on data\n");
  int r=fcntl(jfd1,F_SETLKW,&lock);
  printf("write lock acquired\n");
  struct joint_account j1;
  read(jfd1,&j1,sizeof(struct joint_account));
  printf("details are %s %s %s %d %f\n",j1.acc_name1,j1.acc_name2,j1.acc_pwd,j1.acc_num,j1.bal);
  j1.bal=j1.bal+kamt;
  lseek(jfd1,-1*sizeof(struct joint_account),SEEK_CUR);
  write(jfd1,&j1,sizeof(struct joint_account));
  char m[MAX];
  strcpy(m,"Amount Deposited Successfully\n");
  write(cfd,m,MAX);
  int kg = j1.bal;
  write(cfd,&kg,sizeof(kg));
  char c;
  read(cfd,&c,1);
  lock.l_type=F_UNLCK;
  printf("releasing write lock\n");
  fcntl(jfd1,F_SETLK,&lock);
  printf("lock released\n");
  jointManage();
}

// Joint Management portal
void jointManage()
{
  printf("jointManage\n");
  int res;
  read(cfd,&res,sizeof(res));
  switch(res){
    case 1:
      printf("Joint Account Details\n");
      jointAccountDetails();
    case 2:
      printf("Change Joint Account Password\n");
      jointAccountChangePassword();
      break;
    case 3:
      printf("Withdraw Money\n");
      jointAccountWithdrawMoney();
      break;
    case 4:
      printf("Deposit Money\n");
      jointAccountDepositMoney();
      break;
    default:
      Login_Screen();
  }
}
// Login For Joint User
void jointLogin(){
  printf("joint Login menu\n");
  read(cfd,&jacc_num,sizeof(int));
  read(cfd,jpwd,MAX);
  printf("Joint details are:%d %s\n",jacc_num,jpwd);
  lseek(jfd2,0,SEEK_SET);
  int res=0;
  struct joint_account_d j1;
  lseek(jfd2,0,SEEK_SET);
  count2=0;
  while(read(jfd2,&j1,sizeof(j1))>0){
    if(j1.acc_num==jacc_num && strcmp(j1.acc_pwd,jpwd)==0){
      printf("Account  verified\n");
      res=1;
      break;
    }
    count2++;
  }
  write(cfd,&res,sizeof(int));
  if(res==0){
    printf("Invalid details\n");
    Login_Screen();
  }else{
    printf("login successfull\n");
    printf("record number:%d\n",count2);
    jointManage();
  }
}

// Create Joint Account
void jointCreate(){
  printf("joint create\n");
  struct joint_account t1;
  struct joint_account_d t2;
  int inc;//
  lseek(count_fd,0,SEEK_SET);
  read(count_fd,&inc,sizeof(int));
  write(cfd,&inc,sizeof(inc));
  inc++;
  lseek(count_fd,0,SEEK_SET);
  write(count_fd,&inc,sizeof(inc));
  read(cfd,&t1,sizeof(t1));
  read(cfd,&t2,sizeof(t2));
  lseek(jfd1,0,SEEK_END);
  write(jfd1,&t1,sizeof(t1));
  lseek(jfd2,0,SEEK_END);
  write(jfd2,&t2,sizeof(t2));
  printf("account created\n");
  Login_Screen();
}

//******************************************************** Admin User********************************************************************

// Admin Views Normal Account Details
void adminViewNormalAccountDetails(){
  printf("adminViewNormalAccountDetails\n");
  int anum;
  read(cfd,&anum,sizeof(int));
  struct normal_account a1;
  lseek(nfd,0,SEEK_SET);
  int res=0;
  while(read(nfd,&a1,sizeof(a1))>0){
    if(a1.acc_num==anum){
      printf("account found\n");
      res=1;
      break;
    }
  }
  write(cfd,&res,sizeof(int));
  if(res==1){
      write(cfd,&a1,sizeof(a1));
  }
  adminManage();
}
//Admin views joint account details

void adminViewJointAccountDetails()
{
  printf("JointAccount Details\n");
  int id;
  read(cfd,&id,sizeof(int));
  printf("Account details:%d\n",id);
  int res=0;
  struct joint_account_d t1;
  lseek(jfd2,0,SEEK_SET);
  int counter=0;
  while(read(jfd2,&t1,sizeof(struct joint_account_d))>0){
    //printf("%d %d %s %s\n",j1.acc_num,jacc_num,j1.acc_pwd,jpwd);
    if(t1.acc_num==id){
      printf("Account  verified\n");
      res=1;
      break;
    }
    counter++;
  }
  write(cfd,&res,sizeof(int));
  if(res==0){
    printf("account not found\n");
  }else{
    int offset=sizeof(struct joint_account)*counter;
    lseek(jfd1,offset,SEEK_SET);
    struct flock lock;
    lock.l_type=F_RDLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=offset;
    lock.l_len=sizeof(struct joint_account);
    printf("Trying to acquire read lock on data\n");
    int r=fcntl(jfd1,F_SETLKW,&lock);
    printf("Read lock acquired\n");
    struct joint_account t2;
    read(jfd1,&t2,sizeof(struct joint_account));
    printf("details are %s %s %s %d %f\n",t2.acc_name1,t2.acc_name2,t2.acc_pwd,t2.acc_num,t2.bal);
    write(cfd,&t2,sizeof(struct joint_account));
    char c;
    read(cfd,&c,1);
    lock.l_type=F_UNLCK;
    printf("releasing read lock\n");
    fcntl(jfd1,F_SETLK,&lock);
    printf("lock released\n");
  }
  adminManage();
}

// Delete Normal Account
void adminDeleteNormalAccount(){
  printf("Delete Normal Account\n");
  int acc;
  int res =0;
  read(cfd,&acc,sizeof(int));
  lseek(nfd,0,SEEK_SET);
  struct normal_account a1;
  while(read(nfd,&a1,sizeof(a1))>0){
    if(a1.acc_num==acc)
    {
      printf("account found\n");
      res =1;
      a1.acc_num=-1;
      break;
    }
  }
   write(cfd,&res,sizeof(res));
  if(res==0)
  printf("account number not allocated\n");
  else
  {
  lseek(nfd,-1*sizeof(a1),SEEK_CUR);
  write(nfd,&a1,sizeof(a1));
  printf("normal account deleted\n");
  }
  adminManage();
}
// Admin deletes joint account

void adminDeleteJointAccount(){
  printf("admin delete joint account\n");
  int tc;
  read(cfd,&tc,sizeof(int));

  struct joint_account_d t1;
  lseek(jfd2,0,SEEK_SET);
  int counter=0;
  while(read(jfd2,&t1,sizeof(struct joint_account_d))>0){
    if(t1.acc_num==tc){
      printf("Account verified\n");
      break;
    }
    counter++;//find the record number
  }
  int offset=sizeof(struct joint_account)*counter;
  int offset1=sizeof(struct joint_account_d)*counter;
  lseek(jfd1,offset,SEEK_SET);
  lseek(jfd2,offset1,SEEK_SET);
  struct flock lock;
  lock.l_type=F_WRLCK;
  lock.l_whence=SEEK_SET;
  lock.l_start=offset;
  lock.l_len=sizeof(struct joint_account);
  printf("Trying to acquire write lock on data\n");
  int r=fcntl(jfd1,F_SETLKW,&lock);
  printf("write lock acquired\n");
  struct joint_account t2;
  read(jfd1,&t2,sizeof(struct joint_account));
  read(jfd2,&t1,sizeof(struct joint_account_d));
  printf("details are %s %s %s %d %f\n",t2.acc_name1,t2.acc_name2,t2.acc_pwd,t2.acc_num,t2.bal);
  printf("log details :%d %s\n",t1.acc_num,t1.acc_pwd);
  t2.acc_num=-1;
  t1.acc_num=-1;
  lseek(jfd1,-1*sizeof(struct joint_account),SEEK_CUR);
  lseek(jfd2,-1*sizeof(struct joint_account_d),SEEK_CUR);
  write(jfd1,&t2,sizeof(struct joint_account));
  write(jfd2,&t1,sizeof(struct joint_account_d));
  char *tarr="Account deleted";
  char tarr1[MAX];
  strcpy(tarr1,tarr);
  write(cfd,tarr1,MAX);
  char c;
  read(cfd,&c,1);
  lock.l_type=F_UNLCK;
  printf("releasing lock\n");
  fcntl(jfd1,F_SETLK,&lock);
  printf("lock released\n");
  adminManage();

}

// Account Created By Admin
void Admin_normalCreate()
{
  printf("normalCreate\n");
  struct normal_account a1;
  int res =0;
  int acc1;
  read(cfd,&acc1,sizeof(acc1));
  lseek(nfd,0,SEEK_SET);
  while(read(nfd,&a1,sizeof(a1))>0)//searching for account and password in file 
  {
    if(acc1==a1.acc_num)// account number already exist
    {
      res=1;
      break;
    }
  }
  write(cfd,&res,sizeof(int));
  if(res==1)
  {
  printf("account already present\n");
  Admin_normalCreate();
  //adminManage();
  }
  else
  {
  read(cfd,&a1,sizeof(a1));//read normal account details from client
  lseek(nfd,0,SEEK_END);// move file pointer to end of file
  write(nfd,&a1,sizeof(a1));//writing client data to file
  printf("Normal Account Created\n");
  adminManage();
  }
}
// Admin management portal
void adminManage(){
  printf("Admin Portal\n");
  int option;
  read(cfd,&option,sizeof(int));
  switch(option){
    case 1:
      adminViewNormalAccountDetails();
      break;
    case 2:
      adminViewJointAccountDetails();
      break;
    case 3:
      adminDeleteNormalAccount();
      break;
    case 4:
      adminDeleteJointAccount();
      break;
    case 5:
    Admin_normalCreate();
    default:
      Login_Screen();
  }
  Login_Screen();
}
// verify login for admin
void adminLogin(){
  int admin_acc;
  char admin_pwd[MAX];
  read(cfd,&admin_acc,sizeof(int));
  read(cfd,admin_pwd,MAX);
  int res=0;
  struct admin a1;
  lseek(afd,0,SEEK_SET);
  while(read(afd,&a1,sizeof(struct admin))>0){
    if(admin_acc==a1.id && strcmp(admin_pwd,a1.pwd)==0){
      res=1;
      printf("Credentials verified\n");
      break;
    }
  }
  write(cfd,&res,sizeof(int));
  if(res==0){
    printf("Invalid Details\n");
    Login_Screen();
  }else{
    printf("Login verified\n");
    adminManage();
  }
}
// login screen
void Login_Screen(){
  printf("Login Page->\n");
  int option;
 
  read(cfd,&option,sizeof(int));
  switch (option) {
    case 1:
      normalLogin();
      break;
    case 2:
      jointLogin();
    case 3:
      adminLogin();
    case 4:
      normalCreate();
      break;
    case 5:
      jointCreate();
      break;
    default:
      close(cfd);
      exit(1);
  }
}
int main(int argc,char *argv[]){
  signal(SIGCHLD,myhandler);
  nfd=open("n_acc_d",O_CREAT|O_RDWR,S_IRWXU);//normal user data
  jfd1=open("j_acc_d1",O_CREAT|O_RDWR,S_IRWXU);//joint user data
  jfd2=open("j_acc_d2",O_CREAT|O_RDWR,S_IRWXU);//joint user login data
  afd=open("a_acc_d",O_CREAT|O_RDWR,S_IRWXU);//admin data
  count_fd=open("count.txt",O_CREAT|O_RDWR,S_IRWXU);
  struct sockaddr_in server,client;
  socklen_t client_size=sizeof(struct sockaddr_in);
  sfd=socket(AF_UNIX,SOCK_STREAM,0);//creating socket
  if(sfd==-1)
  {
    printf("Error in creating socket\n");
    printf("%s",strerror(errno));
    return 0;
  }
  printf("socket created successfull\n");
  server.sin_family=AF_UNIX;
  server.sin_addr.s_addr=htonl(INADDR_ANY);
  server.sin_port=htons(8051);
  if(bind(sfd,(struct sockaddr*)&server,sizeof(server))==-1)// binding socket with ip and port
  {
    printf("Error in bind\n%s\n",strerror(errno));return 0;
  }
  printf("successfully bind\n");

  if(listen(sfd,5)==-1)// listen to client
  {
    printf("Error in listen call\n%s",strerror(errno));
  }
  printf("socket created successfully\n");
  while(1){
    if((cfd=accept(sfd,(struct sockaddr *)&client,&client_size))==-1)// accepting connection betwwen serverer and client
    {
      printf("Error in accepting conection from client\n%s\n",strerror(errno));
    }
    printf("connected clients-> %d\n",++count);
    if(fork()==0)
    {
      close(sfd);
      Login_Screen();//call to function
    }
    else
    {
      close(cfd);
    }
  }
  return 0;
}
