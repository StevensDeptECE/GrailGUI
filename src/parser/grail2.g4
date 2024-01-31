grammar simple_grail;

prog: tag_block ;

tag_block: (tag)*;

/* root definition of any tag in Grail */
tag:
    p | div | canvas | rect | button | txt | textlit;

p: '<' 'p' CLASS_NAME? '>' tag_block '<' '/' 'p' '>' ;
div: '<' 'div' CLASS_NAME? '>' tag_block '<' '/' 'div' '>' ;
canvas: '<' 'canvas' CLASS_NAME? '>' tag_block '<' '/' 'canvas' '>' ;
rect:
     '<' 'rect' CLASS_NAME?
     'x' ASSIGN_COORD 'y' ASSIGN_COORD 'w' ASSIGN_COORD 'h' ASSIGN_COORD
     '/>' ;
button:
    '<' 'button' CLASS_NAME? (NAME)? 'text' ASSIGN_TEXT '/>' ;
txt:
    '<' 'text' 'x' ASSIGN_COORD 'y' ASSIGN_COORD '/>' ;

textlit:
    TEXT
    ;

CLASS_NAME: [a-zA-Z_] [a-zA-Z_0-9]* ;

ASSIGN_COORD: '=' [0-9]+ ;

name: 'name' '=' NAME ;

NAME: [A-Za-z_][A-Za-z_0-9]* ;

ASSIGN_TEXT: '=' ('"' [^"]* '"' | '\'' [^']* '\'');

TEXT: ~[<]+ ;