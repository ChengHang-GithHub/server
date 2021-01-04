#include <iostream>
#include <string>
#include <stdio.h>
#include <fcntl.h> // open
#include <unistd.h> // close
#include <sys/stat.h> // stat
#include <sys/mman.h> // mmap, munmap
int main(){
struct stat sbuf;
char* path="./index.html";
stat(path, &sbuf);
int srcFd = ::open(path, O_RDONLY, 0);
void* mmapRet = ::mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, srcFd, 0);
void* mmapRet1 = ::mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, srcFd, 0);
void* mmapRet2 = ::mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, srcFd, 0);
char* srcAddr = static_cast<char*>(mmapRet);
char* srcAddr1 = static_cast<char*>(mmapRet1);
char* srcAddr2 = static_cast<char*>(mmapRet2);
std::cout<<mmapRet<<std::endl;
std::cout<<mmapRet1<<std::endl;
std::cout<<mmapRet2<<std::endl;
munmap(srcAddr, sbuf.st_size);
//std::cout<<srcAddr<<std::endl;
close(srcFd);
return 0;
}
