#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>

static char *path=0;

static char *log_path()
//---------------------
{ if(!path) 
   { char buf[1024],*root=getenv("PVM_ROOT");
     if(root)sprintf(buf,"%s/lib/OS2/pvm.log",root);
     else strcpy(buf,"pvm.log");
     path=strdup(buf);
   }
  return path;
}

void stdlog(char *fmt,...)
/*======================*/
{  FILE *F;
   char buf[4086]="";
   va_list argptr;
   va_start(argptr, fmt);
   vsprintf(buf, fmt, argptr);
   va_end(argptr);
   F=fopen(log_path(),"at");
   if(F){ fprintf(F,buf);
          fclose(F);
        }
}
