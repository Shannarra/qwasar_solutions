#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/// Implementation of std's strlen()
/// Returns the length of the given `str`
size_t len(char *str)
{
  size_t len = 0;

  if (*str == '\0')
    return 0;

  while (str[len++] != '\0');

  return len - 1;
}

/// Implementation of std's strcmp()
/// Returns if the given `src` is the
/// same as the candidate `cand`.
int cmp(char *src, char *cand)
{
  int i = 0;

  if (len(src) != len(cand))
    return 0;

  while (src[i] != '\0')
  {
    if (src[i] != cand[i])
      return 0;
    i++;
  }

  return 1;
}

/// Returns if the given string
/// is a valid code for the game.
int code_is_num(char *arg)
{
  if (atoi(arg))
    return 1;
  else if (cmp(arg, "0000"))
    return 1;

  return 0;
}

/// Implementation of std's strcpy()
/// Copies the `src` contents into the
/// provided `dst`.
///
/// # !WARNING! THIS FUNCTION IS UNSAFE! 
/// If the `dst` is smaller than `src`
/// it WILL segfault intentionally.
void cpy(char *dst, char *src)
{
  int i = 0;

  while (src[i] != '\0')
  {
    dst[i] = src[i];
    i++;
  }
}

/// Kills the program with a given `msg`
int die(char *msg)
{
  printf("%s", msg);
  return -1;
}

/// Parses the provided CLI arguments
int parse_setup(int argc, char **argv, char *code, int *tries)
{
  for (int i = 0; i < argc; i++)
  {
    if (cmp(argv[i], "-c"))
    {
      if (code_is_num(argv[i + 1]))
      {
        if (len(argv[i + 1]) == 4)
          cpy(code, argv[i + 1]);
        else
          return die("Code must be EXACTLY 4 characters!\n");
      }
      else
        return die("The code must be numbers only!\n");
    }
    if (cmp(argv[i], "-t"))
      *tries = atoi(argv[i + 1]);
  }

  return 0;
}

/// Checks if provided `code` is
/// legal, e.g. in range 0-7
int code_illegal(char *code)
{
  for (size_t i = 0; i < 4; i++)
    if (48 > (int)code[i] || (int)code[i] > 55)
      return 1; // illegal number

  return 0;
}

/// Implementation of std's strchr()
/// Returns if the given `src` contains
/// the requested character `c`
int contained(char *src, char c)
{
  for (size_t i = 0; i < 4; i++)
    if (src[i] == c)
      return 1;

  return 0;
}

/// Main loop, runs the game
int play(char *code, int tries)
{
  char in[5] = "";
  int round = 0;

  while (cmp(in, code) != 1 && round <= tries)
  {
    printf("Round %i\n>", round);
    round++;

    scanf("%4s", in);

    while (code_illegal(in))
    {
      printf("Wrong input!\n>");
      scanf("%4s", in);
    }

    // well/mis placed
    int well = 0, mis = 0;

    for (size_t i = 0; i < 4; i++)
    {
      if (contained(code, in[i]))
      {
        if (code[i] == in[i])
          well++;
        else
          mis++;
      }
    }

    printf("Well placed pieces: %i \nMisplaced pieces: %i\n---\n", well, mis);

    if (round == tries && well < 4)
    {
      printf("Number of tries exceeded! The correct code was \"%s\"\n", code);
      return -1;
    }
  }

  printf("Congratz! You did it!\n");

  return 0;
}

int main(int argc, char **argv)
{
  srand(time((0))); // a seed for the randgen

  char code[5] = "";
  int tries = 0;

  if (-1 == parse_setup(argc, argv, code, &tries))
    return -1;

  if (code_illegal(code) == 1 || len(code) == 0)
  { // generate code in the range 0000-7777
    for (int i = 0; i < 4; i++)
      // n + 48 = the number as a character, therefore:
      code[i] = ((char)(rand() % (7 - i) + 48));
  }

  if (tries < 1)
    tries = 10;

  printf("Will you find the secret code?\n---\n");

  return play(code, tries);
}
