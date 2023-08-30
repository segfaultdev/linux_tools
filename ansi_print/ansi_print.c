#include <sys/ioctl.h>
#include <termios.h>
#include <locale.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

static struct termios old_termios;

void io_init(void) {
  tcgetattr(STDIN_FILENO, &old_termios);
  struct termios new_termios = old_termios;
  
  new_termios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  new_termios.c_oflag &= ~(OPOST);
  new_termios.c_cflag |= (CS8);
  new_termios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  new_termios.c_cc[VMIN] = 0;
  new_termios.c_cc[VTIME] = 1;
  
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_termios);
  
  setlocale(LC_ALL, "C.UTF-8");
  printf("\x1B[?1000;1002;1006;1015h");
  
  fflush(stdout);
}

void io_exit(void) {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_termios);
  printf("\x1B[?25h\x1B[?1000;1002;1006;1015l");
  
  fflush(stdout);
}

int main(void) {
  uint8_t chr;
  io_init();
  
  for (;;) {
    if (read(STDIN_FILENO, &chr, 1) > 0) {
      if (chr == '\r' || chr == '\n') {
        printf("\r\n");
      } else if (chr < 32) {
        printf("\\x%02X", chr);
      } else {
        putchar(chr);
      }
      
      fflush(stdout);
      
      if (chr == ((uint8_t)('Q') & 31)) {
        break;
      }
    }
  }
  
  io_exit();
  return 0;
}
