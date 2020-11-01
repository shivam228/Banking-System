#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include<error.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
int sd;
#define MAX 512
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
int acc_num;
char pwd[MAX];
int jacc_num;
char jpwd[MAX];
int count1=0;
void Admin_Login();
void adminManage();
void Admin_View_Normal_Account_Details();
void Admin_View_Joint_Account_Details();
void Admin_Delete_Normal_Account();
void Admin_Delete_Joint_Account();
void Login_Screen();
void Normal_Create_Account();
void Normal_Login();
void Normal_Account_Options();
void Normal_Account_Details();
void Normal_Change_Password();
void Normal_Withdraw_Money();
void Transfer_money();
void Normal_Deposit_Money();
void Normal_View_Account_Balance();
void Joint_Account_Create();
void Joint_Login();
void jointManage();
void joinaccountDetails();
void joinaccountChangePassword();
void joinaccountWithdrawMoney();
void joinaccountDepositMoney();
//*****************************************************NORMAL USER************************************************************************
// Deposit money to a normal account
void Normal_Deposit_Money()
      {
        struct normal_account a1;
        float amt;
	printf("Enter amount to Deposit:");
	scanf("%f",&amt);
	write(sd,&amt,sizeof(amt));//writing amount to be deposited to server 
	read(sd,&a1,sizeof(a1));
	printf("Money Deposited\n");
	printf("Balance Available : %f\n",a1.bal);
        printf("Press any key to go back to Portal\n");
        getchar();
        getchar();
        system("clear");
	Normal_Account_Options();
}

// withdraw money from account
void Normal_Withdraw_Money()
    {
        struct normal_account a1;
        float amt;
	printf("Enter Amount to Withdraw:");
	scanf("%f",&amt);
	write(sd,&amt,sizeof(amt));
	read(sd,&a1,sizeof(a1));
	if(a1.bal < amt)
	{
	printf("Oops Insufficient Balance!!!\n");
	}
	else
	printf("Amount Withdrawn...\n");
	printf("Balance Available : %f\n",a1.bal);
        printf("Press any key to go back to Portal\n");
        getchar();
        getchar();
        system("clear");
	Normal_Account_Options();
}

// view Balance of a normal account
void Normal_View_Account_Balance(){
	printf("Your Account balance details->\n");
	float amt;
       read(sd,&amt,sizeof(float));
       printf("Account Balance:%f\n\n",amt);
       printf("Press any key to go back to Portal\n");
       getchar();
       getchar();
       system("clear"); 
       Normal_Account_Options();
}

// Change password for normal account
void Normal_Change_Password(){
  label:
	printf("Enter new Password for your Account it should be greater than 6 chracters:\n");
	char npsd[MAX];
	scanf("%s",npsd);
	int len;
	len = strlen(npsd);
	if(len < 6)
	{
	printf("Password should be atleast 6 digit long...\n");
	goto label;
	}
	write(sd,npsd,MAX);// writing new password to sever
	printf("Password successfully changed\n\n");
        printf("Press any key to go back to login screen\n");
        getchar();
        getchar();
        system("clear");
        Login_Screen();
	
}

// Get Normal account details 
void Normal_Account_Details()
{
        struct normal_account a1;
	printf("Your Account details are->\n");
	read(sd,&a1,sizeof(a1));// reading details from server
	printf("Name:%s\n",a1.acc_name);
	printf("Password:%s\n",a1.acc_pwd);
	printf("Account_number:%d\n",a1.acc_num);
	printf("Account_balance:%f\n\n",a1.bal);
	printf("Press any key to to go back to  Portal\n");
        getchar();
        getchar();
        system("clear");
	Normal_Account_Options();
}

void Transfer_money()
{
        struct normal_account a1;
        float amt;
        int accx;
        int res;
        int bala;
        printf("Enter Account Number in which you want to deposit Money:");
	scanf("%d",&accx);
	printf("Enter Amount to Deposit:");
	scanf("%f",&amt);
	write(sd,&accx,sizeof(int));
	write(sd,&amt,sizeof(amt));	
        read(sd,&res,sizeof(res));
        if(res==0)
        {
        printf("Account Not Present\n");
        printf("Press any key to go back to Portal\n");
        getchar();
        getchar();
        system("clear");
        Normal_Account_Options();
        }
        else
        {
        read(sd,&bala,sizeof(int));
        read(sd,&a1,sizeof(a1));
        if(bala < amt)
	{
	printf("Oops Insufficient Balance!!!\n");
	}
	else
	printf("Amount Deposited...\n");
	printf("Balance Available : %f\n",a1.bal);
        printf("Press any key to go back to Portal\n");
        getchar();
        getchar();
        system("clear");
	Normal_Account_Options();
	}
}
// options provided by bank to normal client
void Normal_Account_Options()
     {
	printf("***************Welcome to our portal**************\n\n");
	printf("Choose one of the following options:\n");
	printf("1.Deposit Money\n");
	printf("2.Withdraw Money\n");
	printf("3.View Balance\n");
	printf("4.Change Password\n");
	printf("5.View Account Holder Details\n");
	printf("6.Transfer Money\n");
	printf("7.EXIT\n\n");
        printf("Choose an option form above:\n");
	int select;
	scanf("%d",&select);
	write(sd,&select,sizeof(select));
        system("clear"); 
	switch(select){
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

   // Login for the normal user
        void Normal_Login()
        {
        char pp[MAX];     
        char *ptr;    
	printf("Enter Your Account Number:");
	scanf("%d",&acc_num);
	printf("Enter your Password:");
	ptr = getpass(pp);            
	sscanf(ptr,"%s",pwd);
	//scanf("%s",pwd);
	write(sd,&acc_num,sizeof(acc_num));
	write(sd,pwd,sizeof(pwd));
	int res;
	read(sd,&res,sizeof(int));// verifying login details from server
	if(res==0)// means wrong username or password
	{
       printf("your login details didnot match \nPlease any key to go back to login menu :\n");
       getchar();
       getchar();
       system("clear");
       Login_Screen();
	}
	else// valid login credentilas
	{
      printf("Login successfull\n");
      system("clear");
      Normal_Account_Options();// facilities provided by bank to client
	}
}

// Create account for normal user 
void Normal_Create_Account()
        {
        int count;
	printf("Enter Your Details->\n");
	printf("Your Account number is:");
	read(sd,&count,sizeof(count));
	printf("%d\n",count);
	printf("Enter Account name:");
	char name1[MAX];
	scanf("%s",name1);
   label:
	printf("Enter password:");
	char pwd1[MAX];
	scanf("%s",pwd1);
	int len;
	len = strlen(pwd1);
	if(len < 6)
	{
	printf("Password should be atleast 6 digit long...\n");
	goto label;
	}
	printf("Enter Opening  Balance:");
	float b;
	scanf("%f",&b);
	struct normal_account a1;
	a1.bal=b;
	a1.acc_num=count;
	strcpy(a1.acc_name,name1);
	strcpy(a1.acc_pwd,pwd1);
	write(sd,&a1,sizeof(a1));//write the data to server
	printf("Account created successfully\n\n");
        printf("Press any key to go back to login menu to login to your account:\n");
        getchar();
        getchar();
        system("clear");
        Login_Screen();//call to function
 
}
//*********************************************** joint account holder********************************************************************

//View joint account details
void joinaccountDetails()
{
  printf("Your Account details are:\n");
  struct joint_account j1;
  read(sd,&j1,sizeof(struct joint_account));
  printf("First Account Holder Name:%s\nSecond Account Holder Name:%s\nAccount password:%s\nAccount number:%d\nAccount balance:%f\n\n",j1.acc_name1,j1.acc_name2,j1.acc_pwd,j1.acc_num,j1.bal);
  char k;
  printf("Press any key to go back to Management portal\n");
  read(0,&k,1);
  write(sd,&k,1);
  system("clear");
  jointManage();
}
// Change password
void joinaccountChangePassword()
{
  printf("Joint Account Change Password..\n");
  label:
  printf("Enter new Password\n");
  char tarr[MAX];
  scanf("%s",tarr);
  int len;
  len = strlen(tarr);
  if(len < 6)
  {
  printf("Password should be atleast 6 digit long...\n");
  goto label;
  }
  write(sd,tarr,MAX);
  char t[MAX];
  read(sd,t,MAX);
  printf("%s\n\n",t);
  char c;
  printf("Press any key to go back to Management portal\n");
  read(0,&c,1);
  write(sd,&c,1);
  system("clear");
  jointManage();
}

//Withdraw money for joint account
void joinaccountWithdrawMoney()
{
  printf("Joint Account Withdraw Money->\n");
  printf("Enter amount to withdrawn\n");
  float kamt;
  scanf("%f",&kamt);
  write(sd,&kamt,sizeof(float));
  char t[MAX];
  read(sd,t,MAX);
  printf("%s\n",t);
  int kg;
  read(sd,&kg,sizeof(kg));
  printf("Balance Available :%d\n",kg);
  char c;
  printf("Press any key to go back to Management portal\n");
  read(0,&c,1);
  write(sd,&c,1);
  system("clear");
  jointManage();
}

// deposit money for joint account
void joinaccountDepositMoney()
{
  printf("Joint Account Deposit Money\n");
  printf("Enter amount to deposit:\n");
  float kamt;
  scanf("%f",&kamt);
  write(sd,&kamt,sizeof(float));
  char t[MAX];
  read(sd,t,MAX);
  printf("%s\n",t);
  int kg;
  read(sd,&kg,sizeof(kg));
  printf("Balance Available :%d\n",kg);
  char c;
  printf("Press any key to go back to Management portal\n");
  read(0,&c,1);
  write(sd,&c,1);
  system("clear");
  jointManage();
}

// Joint account management portal
void jointManage()
{
  printf("**************************Welcome to Joint Management Portal***********************************\n\n");
  printf("1.View Account Holder Details\n");
  printf("2.Change Password\n");
  printf("3.Withdraw Money\n");
  printf("4.Deposit Money\n");
  printf("5.Logout\n\n");
  printf("Choose one of the options:\n");
  int res;
  scanf("%d",&res);
  write(sd,&res,sizeof(int));
  system("clear");
  switch(res){
    case 1:
      joinaccountDetails();
    case 2:
      joinaccountChangePassword();
      break;
    case 3:
      joinaccountWithdrawMoney();
      break;
    case 4:
      joinaccountDepositMoney();
      break;
    default:
      Login_Screen();
  }
}
// login for joint account
  void Joint_Login(){
  char pp[MAX];     
  char *ptr;
  printf("Enter your account number\n");
  scanf("%d",&jacc_num);
  printf("Enter your account password\n");
  ptr = getpass(pp);            
  sscanf(ptr,"%s",jpwd);
  //scanf("%s",jpwd);
  write(sd,&jacc_num,sizeof(int));
  write(sd,jpwd,MAX);
  int res;
  read(sd,&res,sizeof(int));
  if(res==1){
    printf("\n\nLogin successfull\n");
    system("clear");
    jointManage();
  }else{
    printf("\n\nInvalid Login details\n");
    printf("Press any key to go back\n");
    getchar();
    getchar();
    system("clear");
    Login_Screen();
  }
}
// create joint account
void Joint_Account_Create(){
  int count_joint;
  printf("Create new joint Account\n");
  printf("Enter first account holder name\n");
  char a1[MAX];
  scanf("%s",a1);
  printf("Enter second account holder name\n");
  char n2[MAX];
  scanf("%s",n2);
  label:
  printf("Enter password\n");
  char n3[MAX];
  scanf("%s",n3);
  int len;
	len = strlen(n3);
	if(len < 6)
	{
	printf("password should be atleast 6 digit long...\n");
	goto label;
	}
  printf("Enter amount to deposit\n");
  float kamt;
  scanf("%f",&kamt);
  printf("your account number is\n");
  read(sd,&count_joint,sizeof(int));
  printf("%d\n",count_joint);
  struct joint_account j1;
  struct joint_account_d j2;
  strcpy(j1.acc_name1,a1);
  strcpy(j1.acc_name2,n2);
  strcpy(j1.acc_pwd,n3);
  j1.acc_num=count_joint;
  j1.bal=kamt;
  j2.acc_num=count_joint;
  strcpy(j2.acc_pwd,n3);
  write(sd,&j1,sizeof(struct joint_account));
  write(sd,&j2,sizeof(struct joint_account_d));
  printf("Account Created Successfully\n");
  printf("\n\nPress any key to go back to portal\n");
  getchar();
  getchar();
  system("clear");
  Login_Screen();
}
//***********************************************************ADMIN USER*****************************************************************
// Admin views normal account details 
void Admin_View_Normal_Account_Details(){
  printf("Enter Account number:\n");
  int acc;
  scanf("%d",&acc);
  write(sd,&acc,sizeof(acc));
  struct normal_account a1;
  int res;
  read(sd,&res,sizeof(int));
  if(res==0)
  {
    printf("Account not found\n\n");
    printf("Press any key to go back to admin portal\n");
    getchar();
    getchar();
    system("clear");
    adminManage();
  }
  read(sd,&a1,sizeof(a1));
  printf("Account details are->\n");
  printf("Accound number:%d\n",a1.acc_num);
  printf("User Name:%s\n",a1.acc_name);
  printf("Password:%s\n",a1.acc_pwd);
  printf("Account Balance:%f\n\n",a1.bal);
  printf("Press any key to go back to admin portal\n");
  getchar();
  getchar();
  system("clear");
  adminManage();
}

//Admin views joint acccount details
void Admin_View_Joint_Account_Details(){
  printf("Enter account id:\n");
  int id;
  scanf("%d",&id);
  write(sd,&id,sizeof(id));
  int res=0;
  read(sd,&res,sizeof(id));
  if(res==0){
    printf("Account doesn't exist\n\n");
    printf("Press any key to go back\n");
    getchar();
    getchar();
    system("clear");
    adminManage();
  }
  struct joint_account j1;
  read(sd,&j1,sizeof(j1));
  printf("\n\nFirst Account Holder Name:%s\nJoint Account Holder Name:%s\nPassword:%s\nAccount Number:%d\nBalance:%f\n\n",j1.acc_name1,j1.acc_name2,j1.acc_pwd,j1.acc_num,j1.bal);
  char k;
  printf("Press any key to go back\n");
  read(0,&k,sizeof(k));
  write(sd,&k,1);
  system("clear");
  adminManage();
}
// Admin delete normal account
void Admin_Delete_Normal_Account(){
  printf("Enter account number to delete:\n");
  int acc;
  int res;
  scanf("%d",&acc);
  write(sd,&acc,sizeof(acc));
  read(sd,&res,sizeof(res));
  if(res==0)
  printf("Account number not Allocated\n");
  else
  printf("Normal Account Deleted\n");
  printf("Press any key to go back\n");
  getchar();
  getchar();
  system("clear");
  adminManage();

}
// Admin delete joint account
void Admin_Delete_Joint_Account(){
  printf("Enter account number to delete\n");
  int tc;
  scanf("%d",&tc);
  write(sd,&tc,sizeof(tc));
  char msg[MAX];
  read(sd,msg,MAX);
  printf("%s\n\n",msg);
  char k;
  printf("Press any key to go back\n");
  read(0,&k,sizeof(k));
  write(sd,&k,1);
  system("clear");
  adminManage();
}
// Normal account created by Admin 
void Admin_Normal_Create_Account()
        {
	printf("Enter Your Details->\n");
	printf("Enter Account number:");
	int n;
	scanf("%d",&n);
	if(n>1000 || n<0)
	{
	printf("cannot be allocated admin can allocate account from 1 to 999");
	getchar();
	getchar();
        system("clear");
	Admin_Normal_Create_Account();
	}
	write(sd,&n,sizeof(n));
	int res;
	read(sd,&res,sizeof(int));
	if(res ==1)//account number already present
	{
	printf("Account number already allocated try with different account number\n\n"); 
	printf("Please any key to go back to create page:\n");
        getchar();
        getchar();
       system("clear");
       Admin_Normal_Create_Account();
	}
	else// account number not present and can be allocated to client 
	{
	printf("Enter Account name:");
	char name1[MAX];
	scanf("%s",name1);
   label:
	printf("Enter password:");
	char pwd1[MAX];
	scanf("%s",pwd1);
	int len;
	len = strlen(pwd1);
	if(len < 6)
	{
	printf("password should be atleast 6 digit long...\n");
	goto label;
	}
	printf("Enter opening  balance:");
	float b;
	scanf("%f",&b);
	struct normal_account a1;
	a1.bal=b;
	a1.acc_num=n;
	strcpy(a1.acc_name,name1);
	strcpy(a1.acc_pwd,pwd1);
	write(sd,&a1,sizeof(a1));//write the data to server
	printf("Account created successfully\n\n");
        printf("Press any key to go back to go back to admin portal\n");
        getchar();
        getchar();
        system("clear");
       adminManage();//call to function
  }
}

// Admin management portal
void adminManage(){
  printf("Admin Management Portal\n");
  printf("1.View Normal Account Details\n");
  printf("2.View Joint Account Details\n");
  printf("3.Delete Normal Account\n");
  printf("4.Delete Joint Account\n");
  printf("5.Add Normal Account\n");
  printf("6.Exit()\n\n");
  printf("Choose an option\n");
  int option;
  scanf("%d",&option);
  write(sd,&option,sizeof(int));
  system("clear");
  switch(option){
    case 1:
             Admin_View_Normal_Account_Details();
             break;
    case 2:
             Admin_View_Joint_Account_Details();
             break;
    case 3:
            Admin_Delete_Normal_Account();
            break;
    case 4:
            Admin_Delete_Joint_Account();
            break;
    case 5:
            Admin_Normal_Create_Account();
            break;
    default:
            Login_Screen();
  }

}
// verify login for Admin
void Admin_Login(){
        int id;
        char psd[MAX];
        printf("Enter your id:");
	scanf("%d",&id);
	printf("Enter your password:");
	scanf("%s",psd);
	write(sd,&id,sizeof(id));
	write(sd,psd,sizeof(psd));//send id and password to server to check whether it is correct or not
	int res;
	read(sd,&res,sizeof(res));// read response fro server
	if(res==0)// invalid user
	{
	printf("Invalid Usename or password\nPlease any key to go back to login screen...\n");
        getchar();
        getchar();
        system("clear");
	Login_Screen();
	}
	else
	{
	printf("Login successfull\n");
        system("clear");
	adminManage();//function call to admin management portal
	}
}
// list of options available at login time
void Login_Screen()
        {
        printf("********** WELCOME TO BANKING SYSTEM******************\n\n");
	printf("Choose one of the following options:\n");
	printf("1.Login(Single Accout)\n");
	printf("2.Login(Joint Account)\n");
	printf("3.Admin Login\n");
	printf("4.Create Account(Single)\n");
	printf("5.Create Account(Joint)\n");
	printf("6.Exit\n\n");
	int option;
	printf("Enter your choice:");
	scanf("%d",&option);
        fflush(stdin);
	write(sd,&option,sizeof(int));// send client choice to server
        system("clear");
	switch(option)
	{
    case 1:
              Normal_Login();
              break;
    case 2:
              Joint_Login();
    case 3:
              Admin_Login();
    case 4:
         
              Normal_Create_Account();
              break;
    case 5:
              Joint_Account_Create();
              break;
    default:
              close(sd);
              exit(1);
	}
}
// create a connection between client and server
int main()
{
	struct sockaddr_in server;
	char buf[80];
	sd=socket(AF_UNIX,SOCK_STREAM,0);//creating socket
	if(sd==-1)
  {
        printf("Error in creating socket:%d\n",sd);
        printf("%s",strerror(errno));
        return 0;
  }
	printf("socket created successfull\n\n");
	server.sin_family= AF_UNIX;
	server.sin_port=htons(8051);
	if(connect(sd, (struct sockaddr *)(&server), sizeof(server))==-1)//connecting client to server
	{
	printf("Error in connect\n%s\n",strerror(errno));
	return 0;
	}
	printf("Connected to server sucessfully\n");
        system("clear");//clear screen
	Login_Screen();// call to function
  close(sd);
  return 0;
}
