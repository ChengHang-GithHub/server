#ifndef __LOGFILE_H__
#define __LOGFILE_H__
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>//strlen()
#include<mutex>   //mutex
#include<fcntl.h>//FILE
#include <thread>
using namespace std;
class Logfile{
private:
	char fp_cache[1024*64];
	FILE* fp;
	char* filename;
	int count;
	mutex mymutex;
	const int flush_N;
public:
	Logfile(char* filename_):fp(fopen(filename_, "w+")),filename(filename_),count(0),flush_N(1024){
		setbuffer(fp,fp_cache,sizeof(fp_cache));
	}
	~Logfile(){
		fclose(fp);  //析构函数中要关闭文件
	}
	void append(char* logline,int len);
	void flush();
};
#endif