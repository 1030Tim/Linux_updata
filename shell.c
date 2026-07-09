#include <unistd.h> // conturl os, add the fork 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h> // wait fork
//#include <wait.h>

#define MYSH_TOK_DELIM "\t\r\n "
// 巨集 shell 遇到 "\t\r\n " 要分隔
#define MYSH_TOK_BUFFER_SIZE 64
// 準備64格儲存Token

int mysh_cd(char **args);
int mysh_help(char **args);
int mysh_exit(char **args);

char * builtin_cmd[] = 
{
    "cd",
    "help",
    "exit"
};// 紀錄有什麼內建功能
// table-driven design
int (*builtin_func[])(char **) =
{ // 執行對應的指令
    &mysh_cd,
    &mysh_help,
    &mysh_exit
};

int mysh_cd(char **args) // 檢查cd 指令
{// 假設cd Desktop , args[0] = cd, args[1] = Desktop
    if (args[1] == NULL) // 確保後面有接東西
    {
        perror("Mysh error at cd, lack of args\n");
    }
    // cd
    else
    {
        if (chdir(args[1]) != 0)  // 符合條件直接切換
            perror("Mysh error at chdir\n"); // 如果沒有這個資料夾
    }
    return 1;
}

int mysh_builtin_num()
{
    return sizeof(builtin_cmd) / sizeof(builtin_cmd[0]);
} // 知道有多少個指令方便搜尋

int mysh_help(char **args)
{// list出有什麼功能
    puts("This is Mt's shell");
    puts("Here are some built in cmd:");
    for (int i = 0; i < mysh_builtin_num(); i++)
        printf("%s\n", builtin_cmd[i]); 
    return 1;
}

int mysh_exit(char **args)
{
    return 0;
}




char * mysh_read_line()
{
    char * line = NULL; // 建立空指標
    ssize_t bufsize = 0; // 儲存 getline 配置的 buffer 大小
    getline(&line, &bufsize, stdin); // 從 stdin 讀取使用者輸入，並自動配置line 所需的記憶體
    return line;
}

// parsing

char ** mysh_split_line(char * line) // 目的為了切割token出來
{
    int buffer_size = MYSH_TOK_BUFFER_SIZE, position = 0; // position目前位置
    char **tokens = malloc(buffer_size * sizeof(char *)); // 配置記憶體存token **字元陣列
    char *token; // 暫存目前切出來的
    // "cd /desk/main"
    token = strtok(line, MYSH_TOK_DELIM); // 根據設定做分割
    while (token != NULL)
    {
        tokens[position++] = token; 
        token = strtok(NULL, MYSH_TOK_DELIM); // 繼續切下一個
    }
    tokens[position] = NULL; // 結尾標記
    return tokens;
}

int mysh_launch(char **args)
{
    // execvp
    pid_t pid, wpid;  
    int status; // 存放子程序的結束狀態

    pid = fork(); // 建立子序列
    if (pid == 0) // 代表現在在子程序
    {
        if (execvp(args[0], args) == -1) // 載入對應的程式
            perror("Mysh error at execvp\n"); // 若找不到
        exit(EXIT_FAILURE);
    }
    else // 現在在父程式
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int mysh_execute(char **args)
{
    if (args[0] == NULL) return 1;
    for (int i = 0; i < mysh_builtin_num(); i++)
        if (strcmp(args[0], builtin_cmd[i]) == 0)
            return (*builtin_func[i])(args);
    return mysh_launch(args);
    
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
        args = mysh_split_line(line);
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