/*
	-Arthur Zhao
	November 22 2014
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

void verifySuccess(char* username);
void verifyFailure();


char*  getPW(FILE* acc,char* username){
//Loop until eof, fgets each time. strtok()to get username,strcmp()
	char* userInfo=malloc(200);//records the line for a user: username,password 
	char* tempP=malloc(100);
	char* tempU=malloc(100);
	while(!feof(acc)){
		fgets(userInfo,200,acc);//retrieve 1 line
		if(!feof(acc)){
			tempU=strtok(userInfo,",");
			tempU=strtok(NULL,",");	//these 2 lines will get the username from the line
			if(strcmp(tempU,username)==0){
				tempP=strtok(NULL,",");
				return strtok(tempP,"\n");//need to remove \n from password
			}
		}
       	}	
	return NULL;
}

int Verify(char* username, char* password){
//at this point, username and password should both have values
	FILE* accounts=fopen("./Members.csv","rt");	
	char* tempPW=malloc(100);
	tempPW=getPW(accounts,username);
	fclose(accounts);
	if(tempPW==NULL){//-the case the username is registered
		return 1;
	}
	else {
		return strcmp(tempPW,password);
	}
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

