/*
     File        : file_system.C

     Author      : Riccardo Bettati
     Modified    : 2017/05/01

     Description : Implementation of simple File System class.
                   Has support for numerical file identifiers.
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
#include "file_system.H"


/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR */
/*--------------------------------------------------------------------------*/

FileSystem::FileSystem() {
    disk=NULL;
    size=0;
    freeblocks=0;
    File::file_system=this;
}

/*--------------------------------------------------------------------------*/
/* FILE SYSTEM FUNCTIONS */
/*--------------------------------------------------------------------------*/

bool FileSystem::Mount(SimpleDisk * _disk) {
    
    disk=_disk;
    unsigned char buffer[512];
    disk->read(0, buffer);
    memcpy(&size, buffer, 4);
    memcpy(&freeblocks, buffer+4, 4);
    memcpy(&numberof_file, buffer+8, 4);
    
    return true;
}

bool FileSystem::Format(SimpleDisk * _disk, unsigned int _size) {
     unsigned char buffer[512];
    int end=-1;
    memcpy(buffer+508, &end, 4);
    _disk->write(1, buffer);
    
    return true;
}

File * FileSystem::LookupFile(int _file_id) {
      for (int i=0; i<512; ++i) {
        if (data[i][0]==_file_id) {
            File * re = new File(_file_id);
	    re->size = data[i][1];	
            return re;
        }
    }
    return NULL;
}

bool FileSystem::CreateFile(int _file_id) {
    unsigned int i=0;
    
    for(;i<512; i++){
        if(data[i][0] == 0) break;
    }
    if(i!=512){
        data[i][0] = _file_id;
        data[i][1] = 0;
        data[i][2] = 0;
        
        return true;
    }
    else
        return false;
}

bool FileSystem::DeleteFile(int _file_id) {
     unsigned int i = 0;
    for(;i<512;i++){
        if(data[i][0]==_file_id) break;
    }
    if(i==40) return false;
    unsigned char _buf[512];
    unsigned int block = data[i][2];
    unsigned int next_block = 0;
    for(;;){
        if((int)block==-1) break;
        disk->read(block,_buf);
        memcpy(&next_block,_buf+508,4);
        memcpy(_buf+508,&freeblocks,4);
        disk->write(block,_buf);
        freeblocks = block;
        block = next_block;
    }
    data[i][0] = 0;
    data[i][1] = 0;
    data[i][2] = 0;
    return true;
}

void FileSystem::Refresh(unsigned int block_no, unsigned char* _buf, unsigned int nextblock){
 while(1){
        if((int)block_no == -1){
            break;
        }
        File::file_system->disk->read(block_no,_buf);
        memcpy(&nextblock, _buf+508, 4);
        
        memcpy(_buf+508, &(File::file_system->freeblocks),4);
        File::file_system->disk->write(block_no,_buf);
        File::file_system->freeblocks = block_no;
        
        block_no = nextblock;
    }
}
