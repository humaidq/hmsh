#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

const char ROOT_PROMPT = '#';
const char REG_PROMPT = '$';
int quit = 0;

void printPrompt();
char ** getInput();
int isBuiltin(char **command);

char **
getInput()
{
  char input[4096];
  fgets(input, sizeof(input), stdin);
  /* remove \n from fgets */
  char *p;
  if ((p=strchr(input, '\n')) != NULL)
    *p = '\0';

  char **commands = malloc(sizeof(char)*32);

  char *token = strtok(input, " ");
  int i = 0;
  while (token) {
    commands[i++] = token;
    token = strtok(NULL, " ");
  }
  return commands;
}

void
printPrompt()
{
  char work_dir[1024];
  char *dir_ptr = getcwd(work_dir, sizeof(work_dir)); /* TODO replace home dir with ~ */
  if (dir_ptr == NULL) printf("current working dir is NULL!\n");

  char prompt = (getuid() == 0) ? ROOT_PROMPT : REG_PROMPT;
  printf("%s %c ", work_dir, prompt);
}



int
isBuiltin(char **command)
{
  if(command[0] == NULL) {
    return 1;
  } else if(strcmp(command[0], "exit") == 0) {
    quit = 1;
    return 1;
  } else if(strcmp(command[0], "cd") == 0) {
    
  }
  return 0;  
}

int
main(/*int argc, char ** argv*/)
{
  for(;;) {
    printPrompt();
    char **input = getInput();

    /* TODO check if builtin command */
    if(!isBuiltin(input)) {
      int pid;
      int status;
      if((pid = fork()) < 0) {
        /* TODO Error */
        free(input);
        exit(-1); 
      } else if (pid == 0) {
        if(execvp(input[0], input) < 0)
          printf("hmsh: command not found: %s\n", input[0]);
        else exit(0);
      } else {
        while(wait(&status) != pid);
      }
    }
    
    free(input);
    if(quit) exit(0);
  }

}

