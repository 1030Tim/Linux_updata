#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char * mysh_read_line()
{
    char * line = NULL;
    ssize_t bufsize;
    getline(&line, &bufsize, stdin);
    return line;
}

// parsing

char ** mysh_split_line(char * line)
{
    
}

void mysh_loop()
{
    char *line; // save user's input
    // "cd /desk/main"
    // "cd" "/desk/main"
    char **args;
    int status;

    do
    {
        char path[100];
        getcwd(path,100);
        // 取得目前工作目錄
        char now[200] = "[mysh ";
        strcat(now, path);
        strcat(now, " ]$");
        printf("%s",now);

        line = mysh_read_line();
        args = mysh_split_line();
        status = mysh_execute(args);

        free(line);
        free(args);
    }while (status);

    
}

int main(int argc, char *argv[])
{
    // shell loop
    mysh_loop();
    return 0;
}