#include"LogStream.h"
using namespace std;
int main()
{
	fixed_buffer test;
	char data_[]="i love you";
	test.append(data_,sizeof(data_));
	cout<<"sizeof(data_):"<<sizeof(data_)<<endl;
	cout<<"test.avail():"<<test.avail()<<endl;
	cout<<"test.length():"<<test.length()<<endl;
	return 0;
}