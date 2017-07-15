/*
 File: scheduler.C
 
 Author:
 Date  :
 
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

#ifndef NULL
#define NULL 0L
#endif


/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "scheduler.H"
#include "thread.H"
#include "console.H"
#include "utils.H"
#include "assert.H"
#include "simple_keyboard.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   S c h e d u l e r  */
/*--------------------------------------------------------------------------*/

void fun0() {
    Console::puts("Thread: "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");
    Console::puts("FUN 0 INVOKED!\n");

    whlie(size==0);
    yield();
}

Scheduler::Scheduler() {
  size=0;
  //ready_queue = new Queue(); Already construct
  Console::puts("Constructed Scheduler.\n");
}

void Scheduler::yield() {
  	if (size!=0){
            --size;
            Thread* curr= ready_queue.dequeue(); //get next queue waiting
            Thread::dispatch_to(curr);// run new thread
        }
    else{
      Console::puts("CREATING THREAD 0...\n");
      char * stack0 = new char[1024];
      thread0 = new Thread(fun0, stack0, 1024);
      Console::puts("DONE\n");
      Thread::dispatch_to(thread0);
    }
}

void Scheduler::resume(Thread * _thread) {
	ready_queue.enqueue(_thread);//add thread to queue
        ++size;
}

void Scheduler::add(Thread * _thread) {
  	ready_queue.enqueue(_thread);//add thread to queue
        ++size;
}

void Scheduler::terminate(Thread * _thread) {
  Console::puts("IN terminate of the schedler.C");
  	 bool found=false;
        for (int i=0;i<size;++i){
            Thread* temp=ready_queue.dequeue();
            if (temp->ThreadId()==_thread->ThreadId())
                found=true;
            else 
                ready_queue.enqueue(temp);
            d
        }
        if (found)
            --size;ss
}
