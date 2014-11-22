#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

char*  getPW(FILE* acc,char* username){
//Loop until eof, fgets each time. strtok()to get username,strcmp()
	char* userInfo=malloc(200);//records the line for a user: username,password 
	char* tempP=malloc(100);
	char* tempU=malloc(100);
	do{
		fgets(userInfo,200,acc);
		tempU=strtok(userInfo,",");
		if(strcmp(tempU,username)==0){
			tempP=strtok(NULL,",");
			return strtok(tempP,"\n");//need to remove \n from password
		}
	}	
	while(!feof(acc)&&strcmp(tempU,username)!=0);
	return tempP;
}
int Verify(char* username, char* password){
//This method takes in username and password and verify them according to the .csv file
//finding name should be done by binary search
	FILE* accounts=fopen("./admins.csv","rt");	
	char* pw;//Do not allow pw over 100 chars
	char* tempPW=malloc(100);
	tempPW=getPW(accounts,username);
	if(tempPW!=NULL){//-the case the username is registered
		pw=malloc(100);
		strcpy(pw,tempPW);
	}
	else {
		printf("Your username doesn't exist..");
		exit(EXIT_FAILURE);//still need error message-username not registered
	}
	return strcmp(pw,password);
}
void verifySuccess(int result){
//Method takes in rsult of verification and act appropriately
	if(result==0){
		printf("<p>The username and password matched!\n");
	}
	else printf("<p>Your identification did not match, please try again\n");
}
int main(void){
	//1-receive 2 strings from page
	//2-verify with verify method.
	//..
	/*-TESTS*/
/*
	char *un1="username";
	char *pw1="password";
	char *un2="arthur";
	char *pw2="zhao";
	int result=Verify(un2,pw2);//get result of verification
	verifySuccess(result);
	return 0;
*/
	char* un;
	char* pw;
	char* input;
	//int inLength=atoi(getenv("CONTENT_LENGTH"));//get from shell length of input
	un=malloc(1000);
	pw=malloc(1000);
	input=malloc(1000);
	printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);
	printf("<title>Login Details</title>\n");
	fgets(input,200,stdin);
	printf("<p> Info is \n <br> %s\n",input);
	//I now have the input as a string separated by &s and =s
	un=strtok(input,"=");
	un=strtok(NULL,"=");
	pw=strtok(NULL,"=");
	un=strtok(un,"&");
	//Now username & pw has been read.
	int result=Verify(un,pw);	
	if(result==0){//Match

	}
	else{//Did not match

	}
	return 0;
}

