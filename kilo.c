#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disable_raw_mode()
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode()
{
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);

  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);

  tcgetattr(STDIN_FILENO, &raw);
  raw.c_lflag &= ~(ECHO);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main()
{
  char c;

  enable_raw_mode();

  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}
