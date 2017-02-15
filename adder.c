#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv){
    
	int pipeFD;
    char * myfifo2 = "/tmp/myfifo2";
    char complement[1024], minunend[1024], result[1024];

    /* read input from pipe */
    pipeFD = open(myfifo2, O_RDONLY);
    read(pipeFD, complement, sizeof(complement));

    int i = 0;
    int j = 0;
    int carry = 0;

    while(i < (int)strlen(complement)){
        
        /* start at the least significant bit in nuber */
        i += atoi(argv[2]);

        while(j <= atoi(argv[2])){
            if(complement[i] == 1 && minunend[i] == 1){
                if(carry == 1){
                    result[i] = 1;
                    carry = 1;
                }
                else{
                    result[i] = 0;
                    carry = 1;
                }
            }
            else if(complement[i] == 1 && minunend[i] == 0){
                if(carry == 1){
                    result[i] = 0;
                    carry = 1;
                }
                else{
                    result[i] = 1;
                    carry = 0;
                }
            }
            else if(complement[i] == 0 && minunend[i] == 1){
                if(carry == 1){
                    result[i] = 0;
                    carry = 1;
                }
                else{
                    result[i] = 1;
                    carry = 0;
                }
            }
            else if(complement[i] == 0 && minunend[i] == 0){
                if(carry == 1){
                    result[i] = 1;
                    carry = 0;
                }
                else{
                    result[i] = 0;
                    carry = 0;
                }
            }
			i--;
			j++;
			
        }
		i += atoi(argv[2]);
		j = 0;
    }
    close(pipeFD);
    unlink(myfifo2);
    
    printf("%s", result);

	return 0;
}

