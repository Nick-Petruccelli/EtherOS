#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// check if we are targeting the wrong operating system
#if defined(__linux__)
#error "You are not using a cross compiler"
#endif

// check we are on 32 bit x86
#if defined(__I386__)
#error "You need a i686 compiler"
#endif

void terminal_scroll_down(void);

// VGA color constants
enum vga_color {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, enum vga_color color) {
  return (uint16_t)uc | (uint16_t)color << 8;
}

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len] != '\0') {
    len++;
  }
  return len;
}

static const size_t vga_width = 80;
static const size_t vga_hight = 25;

size_t terminal_row;
size_t terminal_col;
uint8_t terminal_color;
uint16_t *terminal_buffer;

void terminal_init(void) {
  terminal_row = 0;
  terminal_col = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  terminal_buffer = (uint16_t *)0xB8000;
  for (size_t row = 0; row < vga_hight; row++) {
    for (size_t col = 0; col < vga_width; col++) {
      const size_t index = row * vga_width + col;
      terminal_buffer[index] = vga_entry(' ', VGA_COLOR_CYAN);
    }
  }
}

void terminal_set_color(enum vga_color color) { terminal_color = color; }

bool is_special_char(char c) {
  switch (c) {
  case '\n':
    return true;
  default:
    return false;
  }
}

void terminal_put_entry_at(char c, enum vga_color color, size_t row,
                           size_t col) {
  if (is_special_char(c)) {
    c = ' ';
  }
  const size_t index = row * vga_width + col;
  terminal_buffer[index] = vga_entry(c, color);
}

void terminal_put_entry(char c, enum vga_color color) {
  terminal_put_entry_at(c, color, terminal_row, terminal_col);
  if (c == '\n') {
    while (terminal_col++ <= vga_width) {
      terminal_put_entry_at(' ', color, terminal_row, terminal_col);
    }
  }
  terminal_col++;
  if (terminal_col > vga_width) {
    terminal_col = 0;
    terminal_row++;
    if (terminal_row > vga_hight) {
      terminal_row--;
      terminal_scroll_down();
    }
  }
}

void terminal_write(const char *data, size_t len) {
  for (size_t i = 0; i < len; i++) {
    terminal_put_entry(data[i], VGA_COLOR_CYAN);
  }
}

void terminal_write_str(const char *data) {
  terminal_write(data, strlen(data));
}

char terminal_get_char_at(size_t row, size_t col) {
  const size_t index = row * vga_width + col;
  uint16_t entry = terminal_buffer[index];
  return (char)(entry & 0x00FF);
}

uint8_t terminal_get_color_at(size_t row, size_t col) {
  const size_t index = row * vga_width + col;
  uint16_t entry = terminal_buffer[index];
  return (uint8_t)((entry & 0x0F00) >> 8);
}

void terminal_scroll_down(void) {
  for (size_t row = 0; row < vga_hight; row++) {
    for (size_t col = 0; col < vga_width; col++) {
      terminal_put_entry_at(terminal_get_char_at(row + 1, col),
                            terminal_get_color_at(row + 1, col), row, col);
    }
  }
}

void kernel_main(void) {
  terminal_init();
  terminal_write_str("Hello OS World!!!!!!!!!!!!\nNext Line.\nLine "
                     "3\nLine4\nLine 5\nLine 6\n Line \n "
                     "8\n9\n10n\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22"
                     "\n23\n24\n25\n26\n27\n28\n29\n30");
}
