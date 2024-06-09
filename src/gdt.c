#include "../inc/gdt.h"

extern void gdt_flush(addr_t);

struct gdt_segment_struct gdt_segments[5];
struct gdt_ptr_struct gdt_ptr;

void init_gdt() {
  gdt_ptr.base = (&gdt_segments);
  gdt_ptr.limit = sizeof(struct gdt_segment_struct) * 5 - 1;

  set_gdt_segment(0, 0, 0, 0, 0);                // Null descriptor
  set_gdt_segment(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment
  set_gdt_segment(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment
  set_gdt_segment(3, 0, 0xFFFFFFFF, 0xFA, 0xC);  // User code segment
  set_gdt_segment(4, 0, 0xFFFFFFFF, 0xF2, 0xC);  // User data segment

  gdt_flush(&gdt_ptr);
}

void set_gdt_segment(uint32_t num, uint32_t base, uint32_t limit,
                     uint8_t access, uint8_t gran) {
  gdt_segments[num].base_low = base & 0x0000FFFF;
  gdt_segments[num].base_mid = base & 0x00FF0000;
  gdt_segments[num].base_high = base & 0xFF000000;

  gdt_segments[num].limit = limit & 0x0000FFFF;
  gdt_segments[num].flags = limit & 0x000F0000;

  gdt_segments[num].access_byte = access;

  gdt_segments[num].flags |= gran & 0xF0;
}
