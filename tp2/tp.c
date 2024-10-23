/* GPLv2 (c) Airbus */
#include <debug.h>
#include <intr.h>

void bp_handler() {
   // TODO
   debug("bp_handler start:\n");
   uint32_t val;
   asm volatile("pusha\n");
   asm volatile ("mov 4(%%ebp), %0":"=r"(val));
   debug("val:0x%x\n",val);
   asm volatile("popa\n");
   asm volatile("leave\n");
   asm volatile("iret\n");
}

void bp_trigger() {
	// TODO
	asm volatile("int3\n");
}

void tp() {
	// TODO print idtr
	idt_reg_t idtr[IDT_NR_DESC];

	get_idtr(idtr);
	printf("idtr address: %p\n",idtr);

	build_int_desc(&idtr->desc[3], gdt_krn_seg_sel(1), (offset_t)bp_handler);

	// TODO call bp_trigger
	bp_trigger();
}
