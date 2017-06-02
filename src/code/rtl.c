// Real-Time Loading

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct
{
    char cmd[100000];
    unsigned int count;
} Command;

int SIZE = 50;

Command *databaseReader()
{
    int i;
    Command *cmd;
    FILE *database;

    database = fopen(".rtldatabase.db", "r");

    fscanf(database, "%d", &SIZE);
    cmd = (Command*) calloc(SIZE,sizeof(Command));

    for(i = 0; i < SIZE; i++)
    {
        fscanf(database, "%s %u ", cmd[i].cmd, &(cmd[i].count));

        if(strcmp(cmd[i].cmd, "") == 0)
            strcpy(cmd[i].cmd, "Empty");
    }

    fclose(database);

    return cmd;
}

void showDatabase(Command *cmd)
{
    int i;
    FILE *database;

    database = fopen(".rtldatabase.db", "r");

    printf("\n\n---------- Database commands ----------\n");
    printf("Events\t\t\t\tCommands\n\n");

    for(i = 0; i < SIZE; i++)
    {
        if(!(strcmp(cmd[i].cmd, "Empty") == 0))
            printf("%d\t\t\t\t%s\n\n", cmd[i].count, cmd[i].cmd);
    }

    fclose(database);
}

int commandSearch(Command *cmd, char *command)
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
        printf("ERROR: Full database, can't insert more commands. Please, delete some commands or change the size of it.\n");

    return firstfit;
}

void databasePrinter(Command *cmd)
{
    FILE *database;
    int i;

    database = fopen(".rtldatabase.db", "w");

    fprintf(database, "%d\n", SIZE);

    for(i = 0; i < SIZE; i++)
        fprintf(database, "%s %u\n", cmd[i].cmd, cmd[i].count);

    fclose(database);
}

Command *deleteCommand(Command *cmd, char *command)
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
    int i, position, qtd, option = 1;
    Command *cmd;
    FILE *database;
    char *command, *startcommand, *killcommand;

    command = (char*) calloc(100000,sizeof(char));
    startcommand = (char*) calloc(100000,sizeof(char));
    killcommand = (char*) calloc(100000,sizeof(char));

    while(option != 0)
    {
        cmd = databaseReader();

        printf("\n\n---------- Configuration menu ----------\n\nInsert the option:\n1 - Show the database saved commands\n2 - Insert and execute a command\n3 - Delete a command\n4 - Clear the database and change the maximum size (actual size = %d processes)\n0 - Exit the configuration program\n\n> ", SIZE);
        scanf(" %d", &option);

        if(option == 1)
            showDatabase(cmd);

        else if(option == 2)
        {
            database = fopen(".rtldatabase.db", "r+w");

            printf("Insira quantos comandos deseja inserir:\n> ");
            scanf("%d", &qtd);

            for(i = 0; i < qtd; i++)
            {
                startcommand[0] = '\0';
                killcommand[0] = '\0';
                fseek(database, SEEK_SET, 0);
                printf("Insira o comando %d:\n> ", i+1);
                scanf(" %[^\n]", command);
                position = commandSearch(cmd, command);

                if(position == -1)
                    goto _out;

                strcpy(cmd[position].cmd, command);
                cmd[position].count++;

                printf("'%s' is starting\n", cmd[position].cmd);
                strcat(startcommand, cmd[position].cmd);
                strcat(startcommand, " &");
                system(startcommand);

                sleep(0.75); // 0.75 seconds wait

                strcat(killcommand, "killall ");
                strcat(killcommand, cmd[position].cmd);

                printf("Killing process %s\n", killcommand);
                system(killcommand);
            }

            fclose(database);
        }

        else if(option == 3)
        {
            printf("Insert the command you want to delete:\n> ");
            scanf("%s", command);
            cmd = deleteCommand(cmd, command);
        }

        else if(option == 4)
        {
            database = fopen(".rtldatabase.db", "w");

            printf("Insert the new size of the database:\n> ");
            scanf(" %d", &SIZE);

            while(SIZE < 1)
            {
                printf("Invalid size.\nInsert the new size of the database:\n> ");
                scanf(" %d", &SIZE);
            }

            fprintf(database,"%d\n", SIZE);

            for(i = 0; i < SIZE; i++)
                fprintf(database, "Empty 0\n");

            cmd = databaseReader();

            fclose(database);
        }

        _out: databasePrinter(cmd);
    }

    return cmd;
}

void init()
{
    int i;
    FILE *database;

    database = fopen(".rtldatabase.db", "w");

    fprintf(database, "%d\n", SIZE);

    for(i = 0; i < SIZE; i++)
    fprintf(database, "Empty 0\n");
}

void rtl(FILE *database, Command *cmd)
{

}

int main(int argv, char *argc[])
{
    Command *cmd;
    FILE *database;

    system("ps -A > .actualprocesses.txt");

    printf("\n\n");
    printf("*********************\n");
    printf("*                   *\n");
    printf("* Real-Time Loading *\n");
    printf("*                   *\n");
    printf("*********************\n");


    if(strcmp(argc[1], "init") == 0)
    {
        printf("\nInitializing Real-Time Loading database file...\n");
        init();
        printf("Real-Time Loading ready!\n");
    }

    else if(strcmp(argc[1], "config") == 0)
    {
        printf("\nWelcome to Real-Time Loading configuration.\n");
        cmd = config();
    }

    else
    {
        cmd = databaseReader();
        rtl(database, cmd);
    }

	return 0;
}
