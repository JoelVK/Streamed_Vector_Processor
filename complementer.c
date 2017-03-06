#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define WRITE 1
#define READ 0

int startProcesses(char * fileA,char * fileB, int size);
int complementer(char * subName, int size);
int incrementer(int size);
int adder(char * minunend, int size);

int pipeFD1[2], pipeFD2[2];   

int main(int argc, char **argv){    
    int size;

    if(argc != 4){
        printf("useage: ./complementer FILE_A FILE_B BIT_LENGTH\n");
        exit(0);
    }
    /* apparently the '/0' and '/n' add two chars at the end, need to be accounted for */
    size = atoi(argv[3]) + 2; 
    
    /* begin the other two processes */
    startProcesses(argv[1],argv[2],size);
    return 0;
}

int startProcesses(char * fileA, char * fileB, int size){
    
    /* instantiate arguments */
    int pid,pid2; 

    /* make complementer to incrementer pipe */
    pipe(pipeFD1);

    if((pid = fork()) < 0) {
        perror ("fork failed");
        exit(1);
    }
    /* begin other two processes */
    if(pid == 0) {
        
        dup2(pipeFD1[READ], STDIN_FILENO);
        close(pipeFD1[WRITE]);
        close(pipeFD1[READ]);
        pipe(pipeFD2);

        if((pid2 = fork()) < READ) {
            perror ("fork failed");
            exit(0);
        }
        if(pid2 == READ){
            dup2(pipeFD2[READ], STDIN_FILENO);
            close(pipeFD2[READ]);
            adder(fileA,size);
        }
        else{
            dup2(pipeFD2[WRITE],STDOUT_FILENO);
            close(pipeFD1[READ]);
            close(pipeFD2[WRITE]);
            close(pipeFD2[WRITE]);
            close(pipeFD2[READ]);
            incrementer(size);
        }                                                                               
    }
    else{
        dup2(pipeFD1[WRITE],STDOUT_FILENO);
        close(pipeFD1[READ]);
        close(pipeFD1[WRITE]);
        complementer(fileB,size);
    }
    return 0;
}

int complementer(char * subName,int size){
	/* parse and complement data */
    int i = 0;
    char subtrahend[size];
    FILE * inputFD;
    if ((inputFD = fopen(subName, "r")) == NULL){
        fprintf(stderr,"file not found\n");
        exit(1);
    }  

    while (fgets(subtrahend,size,inputFD) != NULL){
        while(i < size -2){
           if(subtrahend[i] == '1'){
               subtrahend[i] = '0';
            } 
           else if(subtrahend[i] == '0'){
               subtrahend[i] = '1';
            } 
            i++;
        }
        fprintf(stderr,"subtrahend: %s \n", subtrahend);
        write(STDOUT_FILENO,subtrahend,size);
        i = 0;
    }
    close(pipeFD1[READ]);
    close(pipeFD1[WRITE]);
    fclose(inputFD);
    return 0;
}

int incrementer(int size){
      
    char complement[size]; 
    int i = size-3;

    while(1){
        read(STDIN_FILENO,complement,sizeof(complement));

        while(i >= 0){
            if(complement[i] == '1'){
                complement[i] = '0';
            }
            else{
                complement[i] = '1';
                break;
            }
            i--;
        }
        write(STDOUT_FILENO,complement,size);
    }
    close(pipeFD2[READ]);
    close(pipeFD2[WRITE]);
	exit(0);
}

int adder(char * minunend, int size){
    
    char complement[size], minund[size];   
    FILE * inputFD;

    if ((inputFD = fopen(minunend, "r")) == NULL){
        printf("file not found\n");
        exit(1);
    }  

    int i = 0;
    char carry = '0';

    while(fgets(minund,size,inputFD) != NULL){
     
        read(STDIN_FILENO,complement,sizeof(complement));   
        
        /* start at the least significant bit in number */
        i = size-3;

        while(i >= 0){
            if(complement[i] =='1'&& minund[i] == '1'){
                if(carry =='1'){
                    complement[i] ='1';
                    carry ='1';
                }
                else{
                    complement[i] ='0';
                    carry ='1';
                }
            }
            else if(complement[i] =='1'&& minund[i] == '0'){
                if(carry =='1'){
                    complement[i] ='0';
                    carry ='1';
                }
                else{
                    complement[i] ='1';
                    carry ='0';
                }
            }
            else if(complement[i] =='0'&& minund[i] == '1'){
                if(carry =='1'){
                    complement[i] ='0';
                    carry ='1';
                }
                else{
                    complement[i] ='1';
                    carry ='0';
                }
            }
            else if(complement[i] =='0'&& minund[i] == '0'){
                if(carry =='1'){
                    complement[i] ='1';
                    carry ='0';
                }
                else{
                    complement[i] ='0';
                    carry ='0';
                }
            }
			i--;
        }
        write(STDOUT_FILENO,complement,size);
    }
    fclose(inputFD);
	exit(0);
}
