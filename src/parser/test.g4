grammar test;

prog: (statement)* ;

statement: simple_statement | compound_statement ;

simple_statement:
    COMMENT |
    doc |
    graph |
    inc;
// single-line comment
COMMENT: '%' [ \t]* .*? '\n' ;

doc: '%doc' '(' ('file' '=')? path ')' ;

graph: '%graph' paramlist;

inc: '%inc' paramlist ;

compound_statement:
    quiz |
    table ;

quiz:
    '%quiz' paramlist .*? '%quiz' ;

table:
    '%table' paramlist .*? '%table' ;

paramlist:
    '(' param? (',' param)* ')' ; 

param:
     IDENT ;

path:
    IDENT ('/' IDENT)* ;

IDENT:
    [a-zA-Z_] [a-zA-Z0-9_]* ;