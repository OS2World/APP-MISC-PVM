#include<os2.h>
#include<stdlib.h>
#include<stdio.h>

/* #define DEBUG */

/************** OS2 *******************/ 
/* interface for standard pvmd script */
/*------------------------------------*/
/* nonstandard os2/emx behaviours:    */ 
/* 1) can not exec *.cmd files        */
/* 2) rsh is blocked after pvmd3 fork */
/**************************************/
/* Tue May 18 19:53:23 CET 1999       */
/* B.Horeni     horeni@login.cz       */
/**************************************/

#define MSGSZ      512

int main(int argc, char *argv[])
/******************************/
{ HPIPE  hp;
  ULONG  ulBytes,ulAction;  
  BYTE   abBuf[MSGSZ]="";
  FILE  *F;
  int    rc,i;
  char  *pvm_root=getenv("PVM_ROOT"),*s,*d;

  if(!pvm_root) 
     { fprintf(stderr,"missing environment variable PVM_ROOT\n");
       return -1; 
     }
  if(argc==3&&!strcmp(argv[1],"$")) 
/********************/
/* RUN as SLAVE     */
/********************/
     { 
#ifdef DEBUG
       for(i=0;i<argc;i++)stdlog("slave : arg[%i] \"%s\"\n",i,argv[i]);
#endif
       rc=DosOpen(argv[2],&hp,&ulAction,0,FILE_NORMAL,FILE_OPEN,
                  OPEN_ACCESS_READWRITE|OPEN_SHARE_DENYNONE,
                 (PEAOP2)NULL);
       if(rc)return -1;
       DosRead(hp,abBuf,MSGSZ,&ulBytes);
#ifdef DEBUG
       stdlog("slave : popen(\"%s\")\n",abBuf);
#endif
       F=popen(abBuf,"r");
       if(!F)return -1;
       fgets(abBuf,MSGSZ,F); 
#ifdef DEBUG
       stdlog("slave : DosWrite(\"%s\")\n",abBuf);
#endif
       DosWrite(hp,abBuf,MSGSZ,&ulBytes);
#ifdef DEBUG
       stdlog("slave : return\n");
#endif
     } 
   else
/*****************/
/* RUN as MASTER */ 
/*****************/
     { char pipeName[256];
#ifdef DEBUG
       for(i=0;i<argc;i++)stdlog("master: arg[%i] \"%s\"\n",i,argv[i]);
#endif
    /***************/
    /* create pipe */
    /***************/ 
       sprintf(pipeName,"\\PIPE\\pvmd_%i",getpid());
       rc=DosCreateNPipe(pipeName,&hp,
                         NP_ACCESS_DUPLEX,
                         NP_WAIT|NP_TYPE_MESSAGE|
                         NP_READMODE_MESSAGE|NP_WMESG|NP_RMESG|0x01,
                         MSGSZ,MSGSZ,0L);
       if(rc)return -1;
    /**************************************/
    /* start slave, param: "$" "pipeName" */
    /**************************************/
       strcpy(abBuf,"start /c /min ");
       s=pvm_root; d=abBuf+strlen(abBuf);
       while(*s){*d++=*s=='/'?'\\':*s; s++;} *d=0;
       strcat(abBuf,"\\lib\\OS2\\pvmd $ ");
       strcat(abBuf,pipeName);
       /* sample command:                                            */        
       /* start /c /min c:\usr\local\pvm3\lib\pvmd $ \PIPE\pvmd_2336 */
#ifdef DEBUG
       stdlog("master: system(\"%s\")\n",abBuf);
#endif
       if(system(abBuf))return -1;   
    /*************************/
    /* connect pipe to slave */
    /*************************/ 
       rc=DosConnectNPipe(hp);
       if(rc)return -1;
    /*************************/
    /* send request to slave */
    /*************************/
       strcpy(abBuf,"sh "); 
       strcat(abBuf,pvm_root); 
       strcat(abBuf,"/lib/pvmd -exe");
       for(i=1;i<argc;i++)
             { strcat(abBuf," "); 
               strcat(abBuf,argv[i]); 
             }
#ifdef DEBUG
       stdlog("master: DosWrite(\"%s\")\n",abBuf);
#endif
       rc=DosWrite(hp,abBuf,MSGSZ,&ulBytes);
       if(rc)return -1;
    /*************************/
    /* get result from slave */
    /*************************/
#ifdef DEBUG
       stdlog("master: get result...\n");
#endif
       rc=DosRead(hp,abBuf,MSGSZ,&ulBytes);
#ifdef DEBUG
       stdlog("master: DosRead(\"%s\")\n",abBuf);
#endif
    /*************************/
    /* put result to stdout  */
    /*************************/
       if(rc==0)puts(abBuf);
    /*************************/
    /* disconnect from slave */
    /*************************/ 
       rc=DosDisConnectNPipe(hp); 
#ifdef DEBUG
       stdlog("master: return\n");
#endif
      }
   return 0;
}    
