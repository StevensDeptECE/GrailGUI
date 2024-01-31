grammar grail;

prog: tag_block ;

tag_block: (tag)* ;

/* root definition of any tag in Grail */
tag:
    ('<' simple_tag '/>') |
    (org | text | link | media | form | table | tag2d | tag3d)
    ('class' '=' '"' CLASS_NAME '"')?
    '>'
    ;

CLASS_NAME: [a-zA-Z_] [a-zA-Z_0-9]* ;
org:
    DIV
    ;

DIV:
    'div'
    ;

/*
    all tags used to display text
 */
text:
    P | SPAN | H1 | H2 | H3 | H4 | H5 | H6 | txt
    ;

P:    'p';
SPAN: 'span';
H1:   'h1' ;
H2:   'h2' ;
H3:   'h3' ;
H4:   'h4' ;
H5:   'h5' ;
H6:   'h6' ;

/*
    text at location
 */
txt: 'txt' 'x' '=' COORD 'y' '=' COORD ;

COORD: ([0-9]+) ;
/*
    any kind of tag that refers to other external documents
    except media which are listed separately
 */
link:
    REF | A | CITE | LINK
    ;    

REF:
    'ref'
    ;

A:
    'a'
    ;

CITE:
    'cite'
    ;

LINK:
    'link'
    ;

media:
    img | video | audio
    ;

quoted_filename:
    '"' FILENAME '"' | '\'' FILENAME '\'' ; 

assign_filename:
    '=' quoted_filename
    ;

/*
 just arbitrarily allowing only names and digits in filenames, easier for now
 no spaces, no punctuation
 the optional specification after # is a region spec within the file
 TODO: grammar of this is not specified, perhaps it should be rigorously
 syntax checked?
*/
FILENAME: [A-Za-z0-9_]+ ('#' [a-Za-z0-9,]*);

img:
    'img' 'src' assign_filename ;
video:
    'video' 'src' assign_filename
    ;

audio:
    'audio' 'src' assign_filename
    ;

field_name:
    'name' '=' [a-zA-Z0-9_]+
    ;

form:
    button | field | select
    ;

TEXTVAL: 'text' '=' '"' [^"]* '"' ;

button:
    'button' field_name TEXTVAL;

field:
    'f' field_name TEXTVAL ;

select:
    'select' field_name TEXTVAL ;

table:
    'table' 'src' assign_filename ;

tag2d:
    canvas | line | rect | circle | ellipse | 
    polygon | polyline | bezier
    ;

canvas: ;
line: ;
rect: ;
circle: ;
ellipse: ;
polygon: ;
polyline: ;
bezier: ;

tag3d:
    cube | sphere | cylinder | polyhedron |
    text3d | group
    ;

cube: ;
sphere: ;
cylinder: ;
polyhedron: ;
text3d: ;
group: ;
