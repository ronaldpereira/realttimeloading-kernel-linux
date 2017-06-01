// Real-Time Loading

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define SIZE 50

typedef struct
{
    char cmd[100];
    unsigned int count;
} Command;

Command *databasereader(FILE *database)
{
    int i;
    Command *cmd;

    cmd = (Command*) calloc(SIZE,sizeof(Command));

    for(i = 0; i < SIZE; i++)
    {
        fscanf(database, "%s %u ", cmd[i].cmd, &(cmd[i].count));
        if(!(strcmp(cmd[i].cmd, "Empty") == 0))
            printf("%s %d\n", cmd[i].cmd, cmd[i].count);
    }

    return cmd;
}

int commandsearch(Command *cmd, char *command)
{
    int i, firstfit = -1;

    for(i = 0; i < SIZE; i++)
    {
        if(strcmp(cmd[i].cmd, command) == 0)
        {
            printf("%s found at line %d\n", cmd[i].cmd, i);
            return i;
        }

        else if((strcmp(cmd[i].cmd, "Empty") == 0) && firstfit == -1)
        {
            printf("No command found at size %d\n", i);
            firstfit = i;
        }
    }

    if(firstfit == -1)
        printf("ERROR: Full buffer, can't insert more commands. Please, delete some commands.\n");

    return firstfit;
}

void databaseprinter(Command *cmd)
{
    FILE *database;
    int i;

    database = fopen("rtldatabase.txt", "w");

    for(i = 0; i < SIZE; i++)
        fprintf(database, "%s %u\n", cmd[i].cmd, cmd[i].count);

    fclose(database);
}

Command *deletecommand(Command *cmd, char *command)
{
    int i;

    for(i = 0; i < SIZE; i++)
    {
        if(strcmp(cmd[i].cmd, command) == 0)
        {
            strcpy(cmd[i].cmd, "Empty");
            cmd[i].count = 0;
            return cmd;
        }
    }

    return cmd;
}

Command *config()
{
    int i, position, qtd;
    Command *cmd;
    FILE *database;
    char *command, *startcommand, *killcommand, option;

    database = fopen("rtldatabase.txt", "r+w");

    command = (char*) calloc(100,sizeof(char));
    startcommand = (char*) calloc(100,sizeof(char));
    killcommand = (char*) calloc(100,sizeof(char));

    cmd = databasereader(database);

    printf("Insert the option: (i)nsert and execute a command / (d)elete a command\n> ");
    scanf("%c", &option);

    if(option == 'i')
    {
        printf("Insira quantos comandos deseja inserir:\n> ");
        scanf("%d", &qtd);

        for(i = 0; i < qtd; i++)
        {
            startcommand[0] = '\0';
            killcommand[0] = '\0';
            fseek(database, SEEK_SET, 0);
            printf("Insira o comando %d:\n> ", i+1);
            scanf(" %[^\n]", command);
            position = commandsearch(cmd, command);

            if(position == -1)
                return cmd;

            strcpy(cmd[position].cmd, command);
            cmd[position].count++;

            printf("'%s' is starting\n", cmd[position].cmd);
            strcat(startcommand, cmd[position].cmd);
            strcat(startcommand, " &");
            system(startcommand);

            sleep(0.5); // 0.5 seconds wait

            strcat(killcommand, "killall ");
            strcat(killcommand, cmd[position].cmd);

            printf("Killing process %s\n", killcommand);
            system(killcommand);
        }
    }

    else if(option == 'd')
    {
        printf("Insert the command you want to delete:\n> ");
        scanf("%s", command);
        cmd = deletecommand(cmd, command);
    }

    databaseprinter(cmd);

    fclose(database);

    return cmd;
}

void rtl(FILE *database, Command *cmd)
{

}

void init()
{
    int i;
    FILE *database;

    database = fopen("rtldatabase.txt", "w");

    for(i = 0; i < SIZE; i++)
        fprintf(database, "Empty 0\n");
}

int main(int argv, char *argc[])
{
    Command *cmd;
    FILE *database;

    system("ps -A > actualprocesses.txt");

    if(strcmp(argc[1], "init") == 0)
    {
        printf("Initializing Real-Time Loading database file\n");
        init();
        printf("Real-Time Loading ready\n");
    }

    else if(strcmp(argc[1], "config") == 0)
    {
        printf("Welcome to Real-Time Loading configuration!\n");
        cmd = config();
    }

    else
    {
        cmd = databasereader(database);
        rtl(database, cmd);
    }

	return 0;
}
