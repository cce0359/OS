/*
     File        : file.C

     Author      : Riccardo Bettati
     Modified    : 2017/05/01

     Description : Implementation of simple File class, with support for
                   sequential read/write operations.
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "assert.H"
#include "console.H"
#include "file.H"
#include "file_system.H"

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR */
/*--------------------------------------------------------------------------*/

File::File(unsigned int _file_id) {
    /* We will need some arguments for the constructor, maybe pointer to disk
     block with file management and allocation data. */
    currentpos = 0;
    currentblock = 0;
    file_id = _file_id;
    
    file_system->CreateFile(file_id);
    size = 0;
    start = 0;
    
}

/*--------------------------------------------------------------------------*/
/* FILE FUNCTIONS */
/*--------------------------------------------------------------------------*/

int File::Read(unsigned int _n, char * _buf) {
  if(_n==0 || _buf==NULL || size==0 || EoF()) return 0;
    unsigned char tmp_buf[512];
    unsigned int number_data_read = 0;
    
    
    while(_n>0){
        file_system->disk->read(currentblock,tmp_buf);
        unsigned int data_to_read;
        
        
        if(_n>(508-(currentpos%508))) {
            data_to_read = 508-(currentpos&508);
        }else{
            data_to_read = _n;
        }
        if(data_to_read>size-currentpos) 
            data_to_read = size-currentpos;
        
        memcpy(_buf+number_data_read,tmp_buf+currentpos%508,data_to_read);
        
        number_data_read += data_to_read;
        _n -= data_to_read;
        currentpos += data_to_read;
    }
    return number_data_read;
}


void File::Write(unsigned int _n, const char * _buf) {
     unsigned char buffer[512];
    unsigned int data_written;
    unsigned int data_to_write;
    if (_n==0 || _buf==NULL) {
        return;
    }
    int flag=currentpos%508;
    while (_n>0) {
        if (flag==0) {
            if (!EoF()) {
                file_system->disk->read(currentblock,buffer);
                memcpy(&currentpos, buffer+508, 4);
            }
        }
        
        unsigned int offset=currentpos%508;
        file_system->disk->read(currentblock, buffer);
        if (_n>508-offset) {
            data_to_write=508-offset;
        }else{
            data_to_write=_n;
        }
        
        memcpy(buffer+offset, _buf+data_to_write, data_to_write);
        file_system->disk->write(currentblock, buffer);
        _n -= data_to_write;
    }
    
    
    return;
}

void File::Reset() {
    currentpos=0;
    currentblock=start;
}

void File::Rewrite() {
    currentblock=0;
    currentpos=0;
    
    unsigned char _buf[512];
    unsigned int block_no = start;
    unsigned int nextblock = 0;
    file_system->Refresh(block_no, _buf, nextblock);

}


bool File::EoF() {
    return size == currentpos;
}
