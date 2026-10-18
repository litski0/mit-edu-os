#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/param.h"
#include "user.h"



char* getfilename(char * path){ 
    char *p= path+strlen(path);
    for(;p>=path && *p != '/'; p--);
    p++;
    return p;
}

void find(char * path,char* name, int argc,char * argv[]){
    // going with dfs approach with recurssion XD
    //getting ptr
    
    int fd;
    struct dirent de;
    struct stat st;
    if( (fd= open(path,O_RDONLY)) <0){
        fprintf(2,"find: cannot open %s\n",path);
        return;
    }

    if(fstat(fd,&st)<0){
        fprintf(2,"find: cannot stat %s\n",path);
        close(fd);
        return;
    }
   
    switch(st.type){
        case T_DEVICE:
        case T_FILE:
            char *filename = getfilename(path);
            if(strcmp(filename,name) == 0){
              
                if(argc==0)
                    printf("%s\n",path);

                else{
                    char * buff [MAXARG];
                    for(int i =0;i<argc;i++){
                        buff[i] = argv[i];
                    }
                    buff[argc]=path;
                    buff[argc+1]=0;
                    int pid=fork();
                    if(pid==0){
                        exec(buff[0],buff);
                        exit(0);
                    }else if (pid>0){
                        wait(0);
                    }else{
                        fprintf(2,"find: fork error\n");
                    }

                }
             
            }
            break;

        case T_DIR:
        while(read(fd,&de,sizeof(de)) == sizeof(de)){
            if(strcmp(de.name,".") ==0 || strcmp(de.name,"..")==0){
                continue;
            }
            if(de.inum == 0)
                continue;

            int n=strlen(path);
            int m=strlen(de.name);
            
            char buff [512];
            for(int i=0;i<n;i++){
                buff[i]=path[i];
            }
            buff[n]='/';
            for(int i=0;i<m;i++){
                buff[i+1+n]=de.name[i];
            }
            buff[n+m+1]='\0';
            find(buff,name,argc,argv);
        }
            
    }
    close(fd);



    

}

int main(int argc, char* argv[]){
    if(argc<2){
        fprintf(2,"usage find <dir> <filename> ...\n");
        exit(1);
    }
    else if(argc==2){
        find(".",argv[1],0,0);
        exit(0);
    }
    else if(argc>=3){
        
        if(argc==3)
            find(argv[1],argv[2],0,0);
        else if (strcmp(argv[3],"-exec")==0){
            find(argv[1],argv[2],argc-4,&argv[4]);
        }else{
            fprintf(2,"usage find <dir> <filename> '-exec' <comamnds...> \n");
            exit(1);
        }
        
    }

    
}
