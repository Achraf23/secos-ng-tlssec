/* GPLv2 (c) Airbus */
#include <debug.h>
#include <segmem.h>

#define c0_sel  gdt_krn_seg_sel(1)
#define d0_sel  gdt_krn_seg_sel(2)
#define c3_sel  gdt_usr_seg_sel(3)
#define d3_sel  gdt_usr_seg_sel(4)

void userland() {
   asm volatile ("mov %eax, %cr0");
}

void print_gdt_content(gdt_reg_t gdtr_ptr) {
    seg_desc_t* gdt_ptr;
    gdt_ptr = (seg_desc_t*)(gdtr_ptr.addr);
    int i=0;
    while ((uint32_t)gdt_ptr < ((gdtr_ptr.addr) + gdtr_ptr.limit)) {
        debug("ptr:%d\n",(uint32_t)gdt_ptr);
        debug("ptr limit:%ld\n",(gdtr_ptr.addr) + gdtr_ptr.limit);
        
        uint32_t start = gdt_ptr->base_3<<24 | gdt_ptr->base_2<<16 | gdt_ptr->base_1;
        uint32_t end;
        if (gdt_ptr->g) {
            end = start + ( (gdt_ptr->limit_2<<16 | gdt_ptr->limit_1) <<12) + 4095;
        } else {
            end = start + (gdt_ptr->limit_2<<16 | gdt_ptr->limit_1);
        }
        debug("%d ", i);
        debug("[0x%x ", start);
        debug("- 0x%x] ", end);
        debug("seg_t: 0x%x ", gdt_ptr->type);
        debug("desc_t: %d ", gdt_ptr->s);
        debug("priv: %d ", gdt_ptr->dpl);
        debug("present: %d ", gdt_ptr->p);
        debug("avl: %d ", gdt_ptr->avl);
        debug("longmode: %d ", gdt_ptr->l);
        debug("default: %d ", gdt_ptr->d);
        debug("gran: %d ", gdt_ptr->g);
        debug("\n");
        gdt_ptr++;
        i++;
    }
    debug("ptr:%d\n",(uint32_t)gdt_ptr);
    debug("ptr limit:%ld\n",(gdtr_ptr.addr) + gdtr_ptr.limit);

}


void tp() {
	// TODO
    gdt_reg_t gdt;
    get_gdtr(gdt);
    // print_gdt_content(gdt);

    //Q5
    // GDTR containing 2 descriptors 
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
    // end Q12

    gdt_reg_t gdt_new = {.addr= (long unsigned int) segments , .limit=sizeof(segments)-1};
    set_gdtr(gdt_new);

    /*** Q13 ***/
    set_ds(d3_sel);
    set_es(d3_sel);
    set_fs(d3_sel);
    set_gs(d3_sel);

    // TSS handling

    uint16_t tr;
    get_tr(tr);
    printf("tr:%d\n",tr);
    //farjump
    // fptr32_t ptr = {.segment = c3_sel, .offset = (uint32_t)userland};
    // farjump(ptr);

}
