#include "Asynclogger.h"


Asynclogger::Asynclogger(char* basename_,int flushinterval_):
							thread_(&Asynclogger::threadfunc,this),
							basename(basename_),
							flushinterval(flushinterval_),
							currentbuffer(new fixed_buffer),
							nextbuffer(new fixed_buffer),
							running(true)
							{
	currentbuffer->bzero();
	nextbuffer->bzero();
	buffers.reserve(16);
}
Asynclogger::~Asynclogger(){
	if(running)
		stop();
}

void Asynclogger::append(char* logline,int len){
	std::unique_lock<mutex>lock(mutex_);
	if(currentbuffer->avail()>len)
	{
		currentbuffer->append(logline,len);
	}
	else
	{
		buffers.push_back(currentbuffer);
		currentbuffer.reset();
		if(nextbuffer)
		{
			currentbuffer=std::move(nextbuffer);
		}
		else
		{
			currentbuffer.reset(new fixed_buffer);
		}
		currentbuffer->append(logline,len);
		cond_.notify_one();
	}
}
void Asynclogger::threadfunc(){
	assert(running==true);
	std::shared_ptr<fixed_buffer> newbuffer1(new fixed_buffer);
	std::shared_ptr<fixed_buffer> newbuffer2(new fixed_buffer);
	newbuffer1->bzero();
	newbuffer2->bzero();
	std::vector<std::shared_ptr<fixed_buffer>> buffersToWrite;
	buffersToWrite.reserve(16);
	Logfile output_file(basename);
	while(running)
	{
		assert(newbuffer1 && newbuffer1->length()==0);
		assert(newbuffer2 && newbuffer2->length()==0);
		assert(buffersToWrite.empty());
		{
			std::unique_lock<mutex> lock(mutex_);   //上锁
			if(buffers.empty())
				continue;
			buffers.push_back(currentbuffer);
			currentbuffer.reset();
			currentbuffer=std::move(newbuffer1);
			buffersToWrite.swap(buffers);
			if(!nextbuffer)
				nextbuffer=std::move(newbuffer2);
		}
		assert(!buffersToWrite.empty());
		
		if(buffersToWrite.size()>25)
			buffersToWrite.erase(buffersToWrite.begin()+2,buffersToWrite.end());
		
		for(int i=0;i<buffersToWrite.size();++i)
			output_file.append(buffersToWrite[i]->get_data(),buffersToWrite[i]->length());
		
		if(buffersToWrite.size()>2)
			buffersToWrite.resize(2);
		if(!newbuffer1)
		{
			assert(!buffersToWrite.empty());
			newbuffer1=buffersToWrite.back();
			buffersToWrite.pop_back();
			newbuffer1->reset();
		}
		if(!newbuffer2)
		{
			assert(!buffersToWrite.empty());
			newbuffer2=buffersToWrite.back();
			buffersToWrite.pop_back();
			newbuffer2->reset();
		}
		buffersToWrite.clear();
		output_file.flush();
	}
	output_file.flush();
}

