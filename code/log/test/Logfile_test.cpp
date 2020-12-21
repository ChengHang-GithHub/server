#include"Logfile.h"
int main()
{
	char* filename="/mnt/hgfs/myshare/tranning/log_system/log.log";
	Logfile logfile(filename);
	char* str="chenghang\n";
	for(int i=1;i<=10000;++i)
		logfile.append(str,strlen(str));
	return 0;
}