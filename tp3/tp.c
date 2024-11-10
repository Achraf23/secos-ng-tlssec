/* GPLv2 (c) Airbus */
#include <debug.h>
#include <segmem.h>

void userland() {
   asm volatile ("mov %eax, %cr0");
}

void tp() {
   
   seg_desc_t segments[5];

   segments[0].raw = 0ULL;

   segments[1].limit_1 = 0xffff;
   segments[1].base_1 = 0x000;
   segments[1].base_2 = 0x00;
   segments[1].type = 11; // code RX
   segments[1].p = 1;
   segments[1].s = 1;
   segments[1].dpl = 0; // ring 0
   segments[1].limit_2 = 0xf;
   segments[1].avl = 1;
   segments[1].d = 1;
   segments[1].g = 0;
   segments[1].base_3 = 0x00;
   
   segments[2].limit_1 = 0xffff;
   segments[2].base_1 = 0x000;
   segments[2].base_2 = 0x00;
   segments[2].type = 3; // data RW
   segments[2].s = 1;
   segments[2].p = 1;
   segments[2].dpl = 0;
   segments[2].limit_2 = 0xf;
   segments[2].avl = 1;
   segments[2].d = 1;
   segments[2].g = 1;
   segments[2].base_3 = 0x00;
   

   /*** Q8 ***/ 
   // debug("test %d",1<<3 | 0<<2 | 0);
   // set_ds(1<<3 | SEG_SEL_GDT<<2 | 0);

   /*** Q12 ***/
   segments[3].limit_1 = 0xffff;
   segments[3].base_1 = 0x000;
   segments[3].base_2 = 0x00;
   segments[3].type = 0xa; // code R/X
   segments[3].s = 1;
   segments[3].p = 1;
   segments[3].dpl = 3;  // ring 3
   segments[3].limit_2 = 0xf;
   segments[3].avl = 1;
   segments[3].d = 1;
   segments[3].g = 1;
   segments[3].base_3 = 0x00;

   segments[4].limit_1 = 0xffff;   //:16;     /* bits 00-15 of the segment limit */
   segments[4].base_1 = 0x0000;    //:16;     /* bits 00-15 of the base address */
   segments[4].base_2 = 0x00;      //:8;      /* bits 16-23 of the base address */
   segments[4].type = 3; //data,RW //:4;      /* segment type */
   segments[4].s = 1;              //:1;      /* descriptor type */
   segments[4].dpl = 3; //ring3    //:2;      /* descriptor privilege level */
   segments[4].p = 1;              //:1;      /* segment present flag */
   segments[4].limit_2 = 0xf;      //:4;      /* bits 16-19 of the segment limit */
   segments[4].avl = 1;            //:1;      /* available for fun and profit */
   segments[4].l = 0; // 32 bits   //:1;      /* longmode */
   segments[4].d = 1;              //:1;      /* default length, depend on seg type */
   segments[4].g = 1;              //:1;      /* granularity */
   segments[4].base_3 = 0x00;      //:8;      /* bits 24-31 of the base address */
    

   gdt_reg_t my_gdtr;
   my_gdtr.addr = (long unsigned int)segments;
   my_gdtr.limit = sizeof(segments) - 1;
   set_gdtr(my_gdtr);

   int cs = (0x1 << 3) | 3;

   uint16_t tr;
   get_tr(tr);
   printf("tr:%d\n",tr);

   asm volatile(
      "push %%ss\n"
      "push %%esp\n"
      "pushf\n"
      "push %%eax\n"
      "push %%ebx\n"
      "iret"
      ::"a"(cs), "b"(userland));

   debug("\nEnd executing tp3.\n");
   
}
