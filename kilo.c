#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)

struct termios orig_termios;

void editor_refresh_screen();

/*** util ***/

void die(const char *s)
{
  editor_refresh_screen();
  perror(s);
  exit(1);
}

/*** terminal ***/

void disable_raw_mode()
{
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) die("tcsetattr");
}

void enable_raw_mode()
{
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(disable_raw_mode);

  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  raw.c_cc[VMIN] = 0;
  raw.c_cc[VMIN] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/*** output ***/

void editor_refresh_screen()
{
  // <esc>[2J - clear entire screen
  write(STDOUT_FILENO, "\x1b[2J", 4);
  // <esc>[H - cursor position (defaults to 1,1)
  write(STDOUT_FILENO, "\x1b[H", 3);
}

/*** input ***/

char editor_read_key()
{
  int nread;
  char c;

  while (read(nread = STDIN_FILENO, &c, 1) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  return c;
}

void editor_process_keypress()
{
  char c = editor_read_key();

  switch (c) {
    case CTRL_KEY('q'):
      editor_refresh_screen();
      exit(0);
      break;
  }
}

/*** init ***/

int main()
{
  enable_raw_mode();

  while (1) {
    editor_refresh_screen();
    editor_process_keypress();
  }

  return 0;
}
