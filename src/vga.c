#include "../inc/vga.h"
#include "../inc/stdlib/stdstr.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void terminal_scroll_down(void);

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, enum vga_color color) {
  return (uint16_t)uc | (uint16_t)color << 8;
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
