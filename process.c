#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int pid;
    int count;
	char cmd[100];
} commandremainder;

int main(int argv, char *argc[]){
    commandremainder commd;
    int i;
    int num = atoi(argc[1]) + 1;

    for(i = 2; i <= num; i++)
        system(argc[i]);

    // while(1){
    //     printf("Insira um comando:\n>");
    //     fflush(stdin);
    //     fgets(commd.cmd, 100, stdin);
    //     system(commd.cmd);
    // }

	return 0;
}
