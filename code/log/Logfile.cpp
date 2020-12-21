#include"Logfile.h"

void Logfile::append(char* log_line,int len){
	unique_lock<mutex>lock(mymutex);
	int index=0;
	while(len>0){
	int n=::fwrite_unlocked(log_line+index,1,len,fp);
	if(n==0){
	int x=ferror(fp);
	if(x)
		fprintf(stderr,"Logfile::append() failed!!!");
	break;
	}
	count++;
	len-=n;
	index+=n;
	}
	if(count>=flush_N)
		flush();
	return;
}
void Logfile::flush(){
	unique_lock<mutex> lock(mymutex);
	fflush(fp);
	return;
}