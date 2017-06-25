#include "assert.H"
#include "exceptions.H"
#include "console.H"
#include "paging_low.H"
#include "page_table.H"
PageTable * PageTable::current_page_table = NULL;
unsigned int PageTable::paging_enabled = 0;
ContFramePool * PageTable::kernel_mem_pool = NULL;
ContFramePool * PageTable::process_mem_pool = NULL;
unsigned long PageTable::shared_size = 0;

void PageTable::init_paging(ContFramePool * _kernel_mem_pool,
                            ContFramePool * _process_mem_pool,
                            const unsigned long _shared_size)
{
    PageTable::kernel_mem_pool = _kernel_mem_pool; //pointers
    PageTable::process_mem_pool = _process_mem_pool;
    PageTable::shared_size = _shared_size;//size of shared kernel memory

   Console::puts("Initialized Paging System\n");
}
unsigned long PageTable::get_page_directory(){
	return (unsigned long) page_directory;
}

PageTable::PageTable()
{
   page_directory = ( unsigned long*)(kernel_mem_pool->get_frames(1)*PAGE_SIZE);
   unsigned long* page_table= (unsigned long*)(kernel_mem_pool->get_frames(1)*PAGE_SIZE);
   unsigned long address=0; // holds the physical address of where a page is

   // map the first 4MB of memory for page table
    for(unsigned int i=0; i<1024; i++){
      page_table[i] = address | 3; // attribute set to: supervisor level, read/write, present(011 in binary)
      address = address + PAGE_SIZE ; // 4096 = 4kb
    }

    page_directory[0] = (unsigned long) page_table;
    page_directory[0] |= 3;

    for(unsigned int i=1; i<1024; i++){
      page_directory[i] = 0 | 2; // attribute set to: supervisor level, read/write, not present(010 in binary)
    };

   Console::puts("Constructed Page Table object\n");
}


void PageTable::load()
{
   current_page_table  = this;
   Console::puts("Loaded page table\n");
}

void PageTable::enable_paging()
{
    write_cr3(current_page_table->get_page_directory());
    paging_enabled = 1;
    write_cr0(read_cr0() | 0x80000000);

       Console::puts("Enabled paging\n");
}

void PageTable::handle_fault(REGS * _r)
{
  Console::puts("handled page fault\n");
}

