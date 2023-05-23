%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
#include "function.h"

 int yylex();
%}

%x structElement
%x attribute
%x end

TypeDefTag    "typedef strict"|"typedef"
StructTag     "struct"
TypeTag       //todo: load from datatype.hh
Regex         "regex"
Comment       "/*"([^\*]|(\*)*[^\*/])*(\*)*"*/"
Obrace        \{
Ebrace        \}
Semi          \;
Address       [0-9]+(\s+\w+)+
Phone         \(?[0-9]+[ \-\.\)][0-9]+[ \-\.][0-9]+
Email         [\w\.-]+@[\w\.-]+\.\w+
UserId        [0-9]+
FirstName     [A-Z][a-z]*
LastName      [A-Z][a-z]*
Name          [A-Za-z][/w]*

%%

<attribute>{
    {TypeDefTag}{
    yylval.str = strdup(yytext);
    return Data_Name;
    // todo: Data_Type
    }


}

{TypeDefTag} {
    yy_push_state(attribute);
    yylval.str = strdup(yytext);
    addTypedef();
    return TYPE;
}
}
{StructTag}   {
    yy_push_state(structElement);
    yylval.str = strdup(yytext);
    addStruct();
    return STRUCT;
}
{Regex}       {}
{Obrace}      {}
{Ebrace}      {}
{Semi}        {}
[\n\r\t]       ;
.              ;
 
%%
"{" {
    yy_push_state()
}


int yywarp(){return 1;
}
/*



"}"{
return END_TAG;
}
*/
