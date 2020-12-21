#ifndef __LOGSTREAM_H__
#define __LOGSTREAM_H__
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<string>  //string

using namespace std;
const int SMALL_BUFFER=4000;

class fixed_buffer{
private:
	char* data;
	char* cur;
public:
	fixed_buffer(){
		data=new char[SMALL_BUFFER];
		cur=data;
	}
	~fixed_buffer(){
		delete[] data;
		data=nullptr;
		cur=nullptr;
	}
	void append(const char* buf,size_t len){
		if(avail()>=len)
		{
			memcpy(cur,buf,len);
			cur+=len;
		}
	}
	int avail()const{
		char* A=data+SMALL_BUFFER;
		int def=A-cur;
		return def;
	}
	int length()const{
		return cur-data;
	}
	char* current()const{
		return cur;
	}
	/*void add(size_t len){
		cur+=len;
	}*/
		
	char* get_data()const{
		return data;
	}
	void reset(){
		cur=data;
	}
	void bzero(){
		memset(data,0,SMALL_BUFFER);
	}
};


class LogStream{
private:
	fixed_buffer buffer;
private:
	template <typename T>
	void transformat(T);
public:
    LogStream(){
	}
	~LogStream(){
	}
	void append(const char*,size_t);
	fixed_buffer& get_buffer();   //这里注意要加const前后都得加！！
public:
    LogStream& operator<<(bool);
	LogStream& operator<<(short);
	LogStream& operator<<(unsigned short);
	LogStream& operator<<(int);
	LogStream& operator<<(unsigned int);
	LogStream& operator<<(long);
	LogStream& operator<<(unsigned long);
	LogStream& operator<<(long long);
	LogStream& operator<<(unsigned long long);
	LogStream& operator<<(double);
	LogStream& operator<<(float);
	LogStream& operator<<(char);
	LogStream& operator<<(char*);
	LogStream& operator<<(unsigned char*);
	LogStream& operator<<(string&);
	LogStream& operator<<(void*);
};
#endif