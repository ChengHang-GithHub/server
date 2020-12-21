#ifndef __ASYNCLOGGER_H__
#define __ASYNCLOGGER_H__
#include<iostream>
#include<stdlib.h>
#include<assert.h>//assert()
#include<mutex>
#include<condition_variable>
#include<thread>
#include<vector>
#include<memory>  //shared_ptr
#include"Logstream.h"
#include"Logfile.h"

class Asynclogger{
private:
	mutex mutex_;
	condition_variable cond_;
	thread thread_;
	
	char* basename;
	bool running;
	const int flushinterval;
	
	std::shared_ptr<fixed_buffer> currentbuffer;
	std::shared_ptr<fixed_buffer> nextbuffer;
	std::vector<std::shared_ptr<fixed_buffer>> buffers;
	
	void threadfunc();
public:
	Asynclogger(char* basename_,int flushinterval_=2);
	~Asynclogger();
	void stop(){
		running=false;
		cond_.notify_one();
		thread_.join();
	}
	void append(char* logline,int len);
};
#endif