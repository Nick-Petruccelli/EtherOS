#include "../inc/gdt.h"
#include "../inc/vga.h"
#include <stdbool.h>

// check if we are targeting the wrong operating system
#if defined(__linux__)
#error "You are not using a cross compiler"
#endif

// check we are on 32 bit x86
#if defined(__I386__)
#error "You need a i686 compiler"
#endif

void kernel_main(void) {
  init_gdt();
  terminal_init();
  terminal_write_str("Hello OS World!!!!!!!!!!!!\nNext Line.\nLine "
                     "3\nLine4\nLine 5\nLine 6\n Line \n "
                     "8\n9\n10n\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22"
                     "\n23\n24\n25\n26\n27\n28\n29\n30");
}
