**************************************************************BANKING SYSTEM***************************************************************
Banking Management System project simulates banking management allowing functionalities like creating and managing joint and single accounts and admin interface in C programming language.Purpose of this project is learning socket programming,process management,file management,record locking,system calls,shared memory,signals,etc concepts of LINUX or c.

Files In The Project------
1. add_admin.c - we have to run this file to add a admin user who have admin privileges.

2. count_initial.c - we have to run this file to intialize Account Number which will be incremented as user create new Account.

3. tserver.c -  we have to run this file to start the server which will wait for some client to connect to it.

4. tclient.c - we have to run this file to connect client user to server.

5. viewAdmin.c - This file is used to view the details of a admin user


HOW TO RUN  PROJECT-------

                               Run count_intial.c -> run tserver.c -> run tclient.c

1. First WE Have Compile count_initial.c to intialize the Account Number in my case i have intialized it to value 1000 and run it.

2. Now compile tserver.c and run it.

3. Now compile tclient.c and run it.

4. For adding Admin user compile and run addAdmin.c



FUNCTIONALITIES PROVIDED ------

1. The system simulates a basic online banking system Functionalities like
	- Creating  an new account (Normal account,Joint account,Admin account)
	- Deposit Money to account
	- Withdraw money from account
	- View details of a Account
	- Change password of Account
	- Tranfer money from one account to another
	- Admin Account User account has add/delete/view any account permissions
	

2. 3 Types of accounts user can have --
	
        - Normal account 
        - Joint account  
        - Admin account 
        
        
3. Normal account(Single User) -
                           create account->login using credential->perform opertaions
 - A user can create a new account for which he have to fill imformation like username ,password,balance,and account number will be        assigned by sever automatically as user click on create account.
 
 - After creating Account user can login to his Account using account number and password.
 
 - After login user will be directed to portal where he have the following options->
 - View Details -- all detalis of the user
 - Deposit money -- to deposit money to his account
 - Withdraw money -- to withdraw money from his account(only valid amount)
 - Change passwd of account(atleast 6 letters)
 - View Balance -- to view Account balance
 - Tranfer Money - to transfer money from one account to another
 - Exit -- go back to login page.	
		
		
4. Joint Account(2 User's with same account number) -
                        create account->login->perform operations	
- A user can create a new account in which two users can have same account allocated for which he have to fill imformation like first username , second username ,password,balance,and account number will be assigned by sever automatically as user click on create account.
 
 - After creating Account user can login to his Account using account number and password.
 
 - After login user will be directed to portal where he have the following options->
 - View Details -- all detalis of the user(two users can view details simuntaneously)
 - Deposit money -- to deposit money to his account(if one user is depositing money other user has to wait for which locking is used
 - Withdraw money -- to withdraw money from his account(if one user is withdrawing money other user has to wait for which locking is used
 - Change passwd of account(atleast 6 letters)
 - Exit -- go back to login page.	
		
5. Admin Account(root user) - 
                          create account using addAdmin.c->login using credentilas->perform operations
   create a admin account by running addAdmin.c
 - Admin user  have the special permissions a user with admin account can->
 - view any Account Details by using Account Number
 - Delete a normal Account -- erases the account details
 - Delete a joint Account -- erases the account details
 - Create a Account(a admin can allocate account number from (1 - 999)
 - Exit
								
		         
