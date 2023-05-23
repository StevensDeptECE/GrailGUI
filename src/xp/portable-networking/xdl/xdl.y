%{
#include <stdio.h>
	//#include "y.tab.h"
#include <string.h>
	//#include "lex.yy.c"
int yylex();
/*int yyerror(FILE *f, const char *s)
{
 printf("\tYACC ERROR: %s\n", s);
 set_error(1);
 return 1;
 }*/

int yyerror(char* s){
 fprintf(stderr,"%s\n",s);
}
 
int yywarp();
main(){
	yyparse();
}

%}


%union{
	char* str;
	int num;
 }

%type<str> contact FIRSTNAME LASTNAME ADDRESS PHONE EMAIL USERID
 //%type<num> USERID

 //%token CONTACT_TAG USER_TAG
%token USERID FIRSTNAME LASTNAME ADDRESS PHONE EMAIL

%start file

%%

file : user
     | file user

user : USERID FIRSTNAME LASTNAME contact {
	// $1 = the user id
	// $2 = peter
	// $3 = ho
	// $4 = Returns 12 Mallory Drive123-456-7890b@a.com
	FILE *f = fopen("xdl.txt", "w");
	fprintf(f, "%s %s %s %s\n", $1, $2, $3, $4);
	fclose(f);
 }
     ;
contact : ADDRESS PHONE EMAIL {
	// Example: 12 Mallory Drive 123-456-7890 b@a.com
	char *buf = malloc(1024);
  strcat(buf, $1); 
  strcat(buf, $2); 
	strcat(buf, $3);
	//fprintf(buf, "hello");
	// Returns 12 Mallory Drive123-456-7890b@a.com
	 return buf; 
 }
        ;




%%

