/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp() {
   debug("kernel mem [0x%p - 0x%p]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);
   // debug("test 0x%x\n", info->mbi->mmap_length);

   char *tab[] = {
      "MULTIBOOT_MEMORY_AVAILABLE",
      "MULTIBOOT_MEMORY_RESERVED",
      "MULTIBOOT_MEMORY_ACPI_RECLAIMABLE",
      "MULTIBOOT_MEMORY_NVS"
   };

   multiboot_memory_map_t* entry = (multiboot_memory_map_t*)info->mbi->mmap_addr;
   while((uint32_t)entry < (info->mbi->mmap_addr + info->mbi->mmap_length)) {
      // TODO print "[start - end] type" for each entry
      // if(entry->type == 1){
      // debug("[0x%x - ",(unsigned int)entry->addr);
      // debug("0x%x] ",(unsigned int)(entry->len + entry-> addr - 1));
      // debug("%s\n",(tab[entry->type-1]));

      // }

      entry++;
   }

   int *ptr_in_available_mem;
   ptr_in_available_mem = (int*)0x0;
   debug("Available mem (0x0): before: 0x%x ", *ptr_in_available_mem); // read
   *ptr_in_available_mem = 0xaaaaaaaa;                           // write
   debug("after: 0x%x\n", *ptr_in_available_mem);                // check

   int *ptr_in_reserved_mem;
   ptr_in_reserved_mem = (int*)0xf0000;
   debug("Reserved mem (at: 0xf0000):  before: 0x%x ", *ptr_in_reserved_mem); // read
   *ptr_in_reserved_mem = 0xaaaaaaaa;                           // write
   debug("after: 0x%x\n", *ptr_in_reserved_mem);                // check

}
