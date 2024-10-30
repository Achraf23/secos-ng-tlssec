/* GPLv2 (c) Airbus */
#include <debug.h>
#include <segmem.h>

void userland() {
   asm volatile ("mov %eax, %cr0");
}

void tp() {
   
   seg_desc_t my_gdt[3];

   my_gdt[0].raw = 0ULL;
   
   my_gdt[1].limit_1 = 0xffff;   //:16;     /* bits 00-15 of the segment limit */
   my_gdt[1].base_1 = 0x0000;    //:16;     /* bits 00-15 of the base address */
   my_gdt[1].base_2 = 0x00;      //:8;      /* bits 16-23 of the base address */
   my_gdt[1].type = 11;//Code,RX //:4;      /* segment type */
   my_gdt[1].s = 1;              //:1;      /* descriptor type */
   my_gdt[1].dpl = 3; //ring3    //:2;      /* descriptor privilege level */
   my_gdt[1].p = 1;              //:1;      /* segment present flag */
   my_gdt[1].limit_2 = 0xf;      //:4;      /* bits 16-19 of the segment limit */
   my_gdt[1].avl = 1;            //:1;      /* available for fun and profit */
   my_gdt[1].l = 0; //32bits     //:1;      /* longmode */
   my_gdt[1].d = 1;              //:1;      /* default length, depend on seg type */
   my_gdt[1].g = 1;              //:1;      /* granularity */
   my_gdt[1].base_3 = 0x00;      //:8;      /* bits 24-31 of the base address */


   my_gdt[2].limit_1 = 0xffff;   //:16;     /* bits 00-15 of the segment limit */
   my_gdt[2].base_1 = 0x0000;    //:16;     /* bits 00-15 of the base address */
   my_gdt[2].base_2 = 0x00;      //:8;      /* bits 16-23 of the base address */
   my_gdt[2].type = 3; //data,RW //:4;      /* segment type */
   my_gdt[2].s = 1;              //:1;      /* descriptor type */
   my_gdt[2].dpl = 3; //ring3    //:2;      /* descriptor privilege level */
   my_gdt[2].p = 1;              //:1;      /* segment present flag */
   my_gdt[2].limit_2 = 0xf;      //:4;      /* bits 16-19 of the segment limit */
   my_gdt[2].avl = 1;            //:1;      /* available for fun and profit */
   my_gdt[2].l = 0; // 32 bits   //:1;      /* longmode */
   my_gdt[2].d = 1;              //:1;      /* default length, depend on seg type */
   my_gdt[2].g = 1;              //:1;      /* granularity */
   my_gdt[2].base_3 = 0x00;      //:8;      /* bits 24-31 of the base address */

   gdt_reg_t my_gdtr;
   my_gdtr.addr = (long unsigned int)my_gdt;
   my_gdtr.limit = sizeof(my_gdt) - 1;
   set_gdtr(my_gdtr);

   int cs = (0x1 << 3) | 3;
   // int d0_sel = gdt_krn_seg_sel(2);
   // set_ss(d0_sel);

   tss_t TSS;
   TSS.s0.esp = get_ebp();
   TSS.s0.ss  = gdt_krn_seg_sel(2);
   tss_dsc(&my_gdt[6], (offset_t)&TSS);
   set_tr(gdt_krn_seg_sel(6));

   uint16_t tr;
   get_tr(tr);
   printf("tr:%d\n",tr);

   asm volatile(
      "push %%ss\n"
      "push %%esp\n"
      "pushf\n"
      "push %%eax\n"
      "push %%ebx\n"
      // "iret"
      ::"a"(cs), "b"(userland));

   debug("\nEnd executing tp3.\n");
   
}
