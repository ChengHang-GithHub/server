#include"Logger.h"
#include"Asynclogger.h"
static pthread_once_t once_control = PTHREAD_ONCE_INIT;
static Asynclogger* AsyncLogger_;
void once_init(){
	AsyncLogger_=new Asynclogger("/mnt/hgfs/myshare/tranning/server/code/log/chenghang.log");
}
void output(char* logline,int len){
	pthread_once(&once_control,once_init);
	AsyncLogger_->append(logline,len);
}
void Logger::format_time(){
	time_t tt=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());  //获取time_t结构体
	std::stringstream ss;   //
	ss << std::put_time(std::localtime(&tt), "%F %T");  //struct tm* localtime(const time_t *timer)
	std::string str = ss.str();
	(*this).get_stream()<<str;
	return;
}