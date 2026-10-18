//syscall needed are file open and read , 

#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"



void sixfive(int fd){
    int i;
    char n[1];
    printf("entering sixfive\n");
    while( (i = read(fd,n,sizeof(n))) > 0){
        
        int tmp=0;
        while(i>0 && (n[0]!='\t' && n[0]!='\r' && n[0]!= '\n' && n[0] != '-' && n[0] != '.' && n[0] != '/' && n[0] != ',')){
            
            tmp = tmp*10 + (n[0]-'0');
            
            i=read(fd,n,sizeof(n));


        }
        if(tmp%5==0 || tmp%6 ==0){

            printf("%d\n",tmp);
        }

        
               
                
    
      
    }
    
}

int main(int argc, char *argv[]){
    int fd;

    if(argc < 2){
        fprintf(2,"usage: sixfive <file1> <file2> ...  ");
        exit(1);
    }

    for(int i=1; i< argc;i++){
        if((fd = open(argv[i], O_RDONLY)) <0){
            printf("sixfive: cannot open %s",argv[i]);
            exit(1);
        }
        sixfive(fd);
    }
    close(fd);
    exit(0);
}