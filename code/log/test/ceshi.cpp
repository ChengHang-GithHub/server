#include"Asynclogger.h"
#include"Logger.h"
#include"LogStream.h"
#include"Logfile.h"
using namespace std;
void myfunc1()
{
	for(int i=1;i<1001;++i){
		LOG<<"i love jerry!!";
		this_thread::sleep_for(chrono::milliseconds(10));
	}
}
void myfunc2()
{
	for(int i=1;i<1001;++i){
		LOG<<"i love jerry!!!!!!!!!!!";
		this_thread::sleep_for(chrono::milliseconds(50));
	}
}
void mufunc2(){
}
int main()
{
	thread thread_1(myfunc1);
	thread thread_2(myfunc2);
	thread_1.join();
	thread_2.join();
	return 0;
}