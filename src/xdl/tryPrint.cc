#include <stdio.h>
#include <stdarg.h>
#include <math.h>


int tryprint(const char* format,...){
  va_list arg;
  va_start(arg, format);
  
  int count = 0;
  
  while (*format)
    {
      char ret = *format;
      if (ret == '%')
	{
	  switch (*++format)
	    {
	      case 'd':{
	      int pd = va_arg(arg, int);
	      int temp = pd;
	      while(temp){
		count++;
		temp /= 10;
	      }
	      temp = pd;
	      while(count){
		int res = fmod(temp, pow(10, count-1));
		putchar(res + '0');
		count--;
	      }
	      break;
	    }
	    case 'c':{
	      int sc = va_arg(arg, int);
	      putchar(sc);
	      break;
	    }
	    case 's':{
	      char *pc = va_arg(arg, char *);
	      while (*pc)
		{
		  putchar(*pc);
		  pc++;
		}
	      break;
	    }
	    default: break;
	    }
	}
      else
	{
	  putchar(*format);
	}
      format++;
    }
  va_end(arg);
}


int main(){
  tryprint("testing\n");
  
  tryprint("%s\n", "test");
  
  tryprint("%c\n", "a");

  tryprint("%s %s  %c%c%c%c!\n", "testing", "try",  'T', 'e', 's', 't', 'a');

  tryprint("%d\n", 22);
  
  return 0;
}
