#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

void terminal_init(void);
void terminal_put_entry_at(char c, enum vga_color color, size_t row,
                           size_t col);
void terminal_put_entry(char c, enum vga_color color);
void terminal_write(const char *data, size_t len);
void terminal_write_str(const char *data);
char terminal_get_char_at(size_t row, size_t col);
uint8_t terminal_get_color_at(size_t row, size_t col);
void terminal_scroll_down(void);
