namespace std; // this file is all in namespace std::

window {
  width:  1280;   // should the default be screen size?
  height: 1024;
}	  

font {
      Times: fonts/TimesRoman.otf;
	     Helvetica: fonts/Helvetica.otf;
	     Courier: fonts/Courier.otf;
	     
}

text {	
  font: Times, serif;   // request Times, if cannot be found then a serif font
      bg: 	#d0d0d0	// bright gray background by default
	fg:	#000000 // default black letters
