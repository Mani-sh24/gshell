#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *get_input();
char **parse_tokens(char *input, int *count);
int main(int argc, char *argv[])
{
  int count;
  char *input;
  do
  {
    printf("> ");
    input = get_input();
    if (strcmp(input, "exit") == 0)
    {
      break;
    }
    char **str = parse_tokens(input, &count);
    printf("%s:command not found\n", input);
    for (int i = 0; i < count; i++)
    {
      free(str[i]);
    }
    free(str);
    free(input);
  } while (1);

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
  int cap = 256, i = 0;
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
    tokens[i++] = token;
  }
  *count = i;
  return tokens;
}