/*
     File        : blocking_disk.H

     Author      : 

     Date        : 
     Description : 

*/

#ifndef _BLOCKING_DISK_H_
#define _BLOCKING_DISK_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "simple_disk.H"
#include "scheduler.H"
#include "thread.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* B l o c k i n g D i s k  */
/*--------------------------------------------------------------------------*/
class Scheduler;
extern Scheduler* SYSTEM_SCHEDULER;
class BlockingDisk : public SimpleDisk {
protected:
  Scheduler *scheduler;
     void wait_until_ready();
    int size;
public:
   BlockingDisk(DISK_ID _disk_id, unsigned int _size);
   /* Creates a BlockingDisk device with the given size connected to the 
      MASTER or SLAVE slot of the primary ATA controller.
      NOTE: We are passing the _size argument out of laziness. 
      In a real system, we would infer this information from the 
      disk controller. */
     


};
/*
class MirroredDisk:public BlockingDisk{
public:
    MirroredDisk(DISK_ID _disk_id, unsigned int _size): SimpleDisk(_disk_id, _size){};
    void read(unsigned long _block_no,unsigned char* _buf);
    void write(unsigned long _block_no, unsigned char* _buf);
    Thread* get_thread();
private:
    queue* blockq;
};*/
#endif
