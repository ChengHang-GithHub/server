#ifndef __LOGGER_H__
#define __LOGGER_H__
#include<iostream>
#include<stdlib.h>
#include<sstream>
#include<chrono>
#include<time.h>
#include <iomanip>
#include"LogStream.h"


void output(char*,int);
class Logger
{
private:
	LogStream stream;
	char* filename;
	int line_num;
private:
    
	void format_time();
public:
	LogStream& get_stream(){
		return stream;
	}
	char* get_filename(){
		return filename;
	}
	int get_linenum(){
		return line_num;
	}
public:
	Logger(char* filename_,int line_num_):filename(filename_),line_num(line_num_)
	{
		format_time();
	}
	~Logger(){
		(*this).get_stream()<<" -- "<< (*this).get_filename() << ':' << (*this).get_linenum() <<'\n';
		const fixed_buffer& buf((*this).get_stream().get_buffer());
		output(buf.get_data(),buf.length());
	}
};
#define LOG Logger(__FILE__, __LINE__).get_stream()
#endif