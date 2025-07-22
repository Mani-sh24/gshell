#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
char **parse_tokens(char *input, int *count);
char *get_input();
int ghs_exec(char **args);
int create_process(char **args);
int gsh_cd(char **args);
int gsh_exit(char **args);
int gsh_pwd(char **args);
int main(int argc, char *argv[])
{
  int count;
  char *input;
  char **args;
  int status;
  do
  {
    printf("> ");
    input = get_input();
    args = parse_tokens(input, &count);
    status = ghs_exec(args);
    if (status == -1)
    {
      printf("%s:command not found\n", input);
    }

    for (int i = 0; i < count; i++)
    {
      free(args[i]);
    }

    free(input);
    free(args);
  } while (status);

  return 0;
}
char *get_input()
{
  size_t bufsize = 1024;
  size_t pos = 0;
  int c;
  char *str1 = (char *)malloc(sizeof(char) * bufsize);
  if (str1 == NULL)
  {
    perror("Error!");
    return NULL;
  }

  while (1)
  {
    c = getchar();

    if (c == EOF || c == '\n')
    {
      str1[pos] = '\0';
      return str1;
    }
    else
    {
      str1[pos] = c;
    }
    pos++;
    if (pos >= bufsize)
    {
      bufsize += bufsize;
      char *temp = realloc(str1, bufsize);
      if (temp == NULL)
      {
        free(str1);
        perror("Error reallocating memory");
        return NULL;
      }
      str1 = temp;
    }
  }
}

char **parse_tokens(char *input, int *count)
{
  int cap = 1024, i = 0;
  char *str = strdup(input);
  char **tokens = malloc(sizeof(char *) * cap);
  char *token;
  while ((token = strsep(&str, " ")) != NULL)
  {
    if (*token == '\0')
      continue;
    if (i >= cap)
    {
      cap *= 2;
      tokens = realloc(tokens, cap * sizeof(char *));
    }
    tokens[i++] = strdup(token);
  }
  tokens[i] = NULL;
  *count = i;
  free(str);
  return tokens;
}

int create_process(char **args)
{
  pid_t pid, wpid;
  int status;
  pid = fork();
  if (pid == 0)
  {
    // child
    if (execvp(args[0], args) == -1)
    {
      return -1;
    }
    exit(EXIT_FAILURE);
  }
  else if (pid < 0)
  {
    perror("gsh");
  }
  else
  {
    do
    {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}
int gsh_cd(char **args)
{
  if (args[1] == NULL)
  {
    fprintf(stderr, "gsh: expected argument to \"cd\"\n");
  }
  else
  {
    if (chdir(args[1]) != 0)
    {
      perror("gsh");
    }
  }
  return 1;
}
int gsh_exit(char **args)
{
  return 0;
}
int gsh_pwd(char **args)
{
  printf("%s", get_current_dir_name());
  return 1;
}

char *builtins[] = {
    "cd", "exit", "pwd"};
// builtins and builtin_func should be in same order!
int (*builtin_func[])(char **) = {
    &gsh_cd,
    &gsh_exit,
    &gsh_pwd};

int gsh_num_builtins()
{

  return sizeof(builtins) / sizeof(char *);
}

int ghs_exec(char **args)
{
  if (args[0] == NULL)
  {
    return 1;
  }
  int i = 0;
  for (i = 0; i < gsh_num_builtins(); i++)
  {
    if (strcmp(args[0], builtins[i]) == 0)
    {
      return (*builtin_func[i])(args);
    }
  }
  return create_process(args);
}