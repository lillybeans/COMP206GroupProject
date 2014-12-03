/*
	-Arthur Zhao
	November 22 2014
*/
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
		tempU=strtok(NULL,",");
		if(strcmp(tempU,username)==0){
			tempP=strtok(NULL,",");
			return strtok(tempP,"\n");//need to remove \n from password
		}
	}	
	while(!feof(acc)&&strcmp(tempU,username)!=0);
	//return NULL;
	return tempP;
}
int userExist(FILE* acc,char* username){

}


int Verify(char* username, char* password){
	FILE* accounts=fopen("./Members.csv","rt");	
	char* pw;//Do not allow pw over 100 chars
	char* tempPW=malloc(100);
	tempPW=getPW(accounts,username);
	if(tempPW!=NULL){//-the case the username is registered
		pw=malloc(100);
		strcpy(pw,tempPW);
	}
	else {
		return 1;
		//printf("Your username doesn't exist..");
		//exit(EXIT_FAILURE);//still need error message-username not registered
	}
	return strcmp(pw,password);
}
void verifySuccess(char* username){
	FILE* loggedin=fopen("./source/loggedin.csv","a");
	fprintf(loggedin,"%s\n",username);
	fclose(loggedin);
	FILE* catalogue=fopen("./catalogue.html","r+wt");
	char* line=malloc(1000);
	while(!feof(catalogue)){
		fgets(line,1000,catalogue);
		if(strcmp(line,"<html>\n")==0){
			printf("%s<h3>You are now logged in as %s",line,username);
		}
		else if(strcmp(line,"\t\t<input type=\"hidden\" name=\"username\" value=''>\n")==0){//COMPARE to the hidden form part
			char *temp=strtok(line,"'");
			strcat(temp,"'");
			strcat(temp,username);
			strcat(temp,"'>\n");
			printf("%s",temp);//update with hiddenbox
		}
		else{
			printf("%s",line);
		}	
	}
	fclose(catalogue);
}
void verifyFailure(){
	//Redirect or write to screen
	printf("<meta http-equiv=\"refresh\" content=\"0; url=./error.html\" />");
}
int main(void){
	char* un;
	char* pw;
	char* input;
	un=malloc(1000);
	pw=malloc(1000);
	input=malloc(1000);
	printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);
	fgets(input,200,stdin);
//	printf("%s",input);
	un=strtok(input,"=");
	un=strtok(NULL,"=");
	if(strcmp(un,"&pw")==0){
		verifyFailure();
		return 1;
	}
	pw=strtok(NULL,"=");
	un=strtok(un,"&");
	if(pw==NULL){
		verifyFailure();
		return 1;
	}
	int result=Verify(un,pw);	
	if(result==0) verifySuccess(un);
	else verifyFailure();
	return 0;
}

