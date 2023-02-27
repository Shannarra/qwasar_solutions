#include <string.h> // strcpy, strlen
#include <stdio.h>  // putchar, EOF
#include <stdarg.h> // All the variadics (e.g. va_arg, va_list, va_start)
#include <stdlib.h> // malloc, abs
#include <stdint.h> // uintptr_t
#include <assert.h> // assert

#define READ_INT(list_p) va_arg(list_p, int)

/// Wraps an integer `val` into a string of the
/// given `radix` and outputs in the 
/// `out` string. 
/// (this is done like so because of the "no static" rules)
/// Additional inspiration:
/// https://groups.google.com/g/comp.lang.c++.moderated/c/ihafayWmWU8?pli=1#00c4c8fcb24dd1cc
void itoa(int val, int radix, char *out)
{
  char buf[32] = {0};

  int i = 30;

  for (; val && i; --i, val /= radix)
    buf[i] = "0123456789abcdef"[val % radix];

  //static vars forbidden so I have to do acrobatics...
  strcpy(out, &buf[i + 1]);
}

/// Prints the string `s` one character at a time
/// until it reaches EOF or faults.
int putstr(char const *s)
{
  for (size_t i = 0; s[i]; ++i)
    if (putchar(s[i]) == EOF)
      return EOF;

  return 0;
}

/// Reads a pointer `p` and converts its address
/// to a HEX string representation, outputed in the
/// given `buf`. MAXLEN 20!
void ptr_to_s(void *p, char buf[20]) /**2 + sizeof(uintptr_t) * 2*/
{
  uintptr_t x = (uintptr_t)p;
  //char buf[2 + sizeof(x) * 2];
  size_t i;

  for (i = 2; i < sizeof(x) * 2; i++)
    buf[i - 2] = "0123456789abcdef"[(x >> ((sizeof(x) * 2 - 1 - i) * 4)) & 0xf];

  buf[1] = 'x';
}

/// Outputs a string to stdout and increments the
/// size counter of the `my_printf` function.
/// It also handles nullptrs/args provided.
void handle_string(va_list va_l, int *size)
{
  char *arg = va_arg(va_l, char *);

  if (arg != NULL && *arg != '\0')
  {
    putstr(arg);
    *size += strlen(arg);
  }
  else
  {
    putstr("(null)");
    *size += 6;
  }
}

/// Outputs a SIGNED number to stdout and increments the
/// size counter of the `my_printf` function.
void handle_signum(va_list va_l, int *size)
{
  int num = READ_INT(va_l);

  char arg[33];

  if (num > 0)
    itoa(num, 10, arg);
  else
  {
    if (num == 0)
      arg[0] = '0';
    else // num < 0
    {
      putchar('-');
      itoa(abs(num), 10, arg);
    }
  }

  putstr(arg);
  *size += strlen(arg);
}

/// Outputs an UNSIGNED number to stdout and increments the
/// size counter of the `my_printf` function ~DEPENDING~
/// on the provided `radix` argument!
void handle_unsignum(va_list va_l, int* size, int radix) 
{
  char arg[33] = "";
  itoa(abs(READ_INT(va_l)), radix, arg);
  putstr(arg);
  *size += strlen(arg);
}

/// Outputs a pointer address to stdout and increments the
/// size counter of the `my_printf` function.
void handle_ptr(va_list va_l, int* size) 
{
  void *pt = va_arg(va_l, void *);
  char arg[20];
  ptr_to_s(pt, arg);
  putstr(arg);
  *size += strlen(arg);
}

/// Implements the functionality of 
/// std's printf and returns the size printed.
int my_printf(char *format, ...)
{
  int size = 0;

  va_list va_l;
  va_start(va_l, format);

  char *ptr = format;

  while (*ptr)
  {
    if (*ptr == '%')
    {
      ptr++;
      switch (*ptr++)
      {
        case 's':
          handle_string(va_l, &size);
          break;
        case 'i':
        case 'd':
          handle_signum(va_l, &size);
          break;
        case 'u':
          handle_unsignum(va_l, &size, 10);
          break;
        case 'o':
          handle_unsignum(va_l, &size, 8);
          break;
        case 'x':
          handle_unsignum(va_l, &size, 16);
          break;
        case 'c':
        {
          size++;
          putchar(va_arg(va_l, int));
          break;
        }
        case 'p':
          handle_ptr(va_l, &size);
          break;
      }
    }
    else
    {
      putchar(*ptr++);
      size++;
    }
  }

  va_end(va_l);

  return size;
}

int main()
{
  // Some tests ran with "assert"
  assert(13 == my_printf("Hello world!\n"));
  my_printf("OK 1!\n");

  assert(3 == my_printf("%c!\n", 'H'));
  my_printf("OK 2!\n");

  assert(7 == my_printf("%c%c%c%c%c!\n", 'H', 'e', 'l', 'l', 'o'));
  my_printf("OK! 3\n");

  assert(17 == my_printf("Hello %s!\n", "QWASAR.IO"));
  my_printf("OK 4!\n");

  assert(17 == my_printf("Hello %s %s!\n", "Mr", "Gaetan"));
  my_printf("OK 5!\n");

  assert(20 == my_printf("NULL STRING %s!\n", (char *)NULL));
  my_printf("NULL OK!\n");

  assert(6 == my_printf("%d!\n", 1337));
  my_printf("NUM OK!\n");

  assert(18 == my_printf("%d - %d-%d!\n", 2048, 0, -1337));
  my_printf("NUMS OK!\n"); // q 16/23

  assert(5 == my_printf("%o!\n", 100));
  my_printf("Octal ok!\n"); // q 17/23

  assert(6 == my_printf("%u\n!", -1337));
  my_printf("UDEC ok!\n"); // q 19/23

  assert(3 == my_printf("%x!\n", 14));
  my_printf("HEX ok!\n"); //21/23

  int var = 69;
  my_printf("%p\n", &var);
  assert(16 == my_printf("%p!\n", &var));
  my_printf("PTR OK!\n");

  my_printf("ALL TESTS SUCCEEDED!\n");
}
