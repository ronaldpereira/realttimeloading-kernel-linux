// Real-Time Loading

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct
{
    unsigned int pid;
    unsigned int count;
    char cmd[100];
    char startcmd[100];
    char killcmd[100];
} command;

int main(int argv, char *argc[]){
    command command[1000];
    FILE *input, *output;
    int i;

    system("ps -A > actualprocesses.txt");

    input = fopen("actualprocesses.txt", "r");
    output = fopen("rtldatabase.txt", "r+w");

    for(i = 0; i < 1000; i++)
    {
        printf("Insira um comando:\n> ");
        fflush(stdin);
        scanf(" %[^\n]", command[i].cmd);

        strcat(command[i].startcmd, command[i].cmd);
        strcat(command[i].startcmd, " &");
        printf("'%s' is starting\n", command[i].startcmd);
        system(command[i].startcmd);

        strcat(command[i].killcmd, "killall ");
        strcat(command[i].killcmd, command[i].cmd);
        printf("Killing process %s\n", command[i].killcmd);
        system(command[i].killcmd);
    }

	return 0;
}
