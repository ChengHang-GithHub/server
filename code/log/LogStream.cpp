#include"LogStream.h"

template<typename T>
void LogStream::transformat(T number){
	string str_=to_string(number);
	size_t len=str_.length();
	const char* str=str_.c_str();  //c_str()返回值为const char*
	buffer.append(str,len);
	return;
}

void LogStream::append(const char* str,size_t len){
	buffer.append(str,len);
	return;
}

fixed_buffer& LogStream::get_buffer(){
	return buffer;
}
 
LogStream& LogStream::operator<<(bool val){
	buffer.append(val? "1":"0",1);
	return *this;
}

LogStream& LogStream::operator<<(int val){
	transformat(val);
	return *this;
}

LogStream& LogStream::operator<<(unsigned int val){
	transformat(val);
	return *this;
}

LogStream& LogStream::operator<<(short val){
	*this<<static_cast<int>(val);
	return *this;
}

LogStream& LogStream::operator<<(unsigned short val){
	*this<<static_cast<unsigned int>(val);
	return *this;
}

LogStream& LogStream::operator<<(long val){
	transformat(val);
	return *this;
}

LogStream& LogStream::operator<<(unsigned long val){
	transformat(val);
	return *this;
}

LogStream& LogStream::operator<<(long long val){
	transformat(val);
	return *this;
}

LogStream& LogStream::operator<<(unsigned long long val){
	transformat(val);
	return *this;
}

LogStream& LogStream::operator<<(double val){
	transformat(val);
	return *this;
}

LogStream& LogStream::operator<<(float val){
	transformat(val);
	return *this;
}

LogStream& LogStream::operator<<(char val){
	buffer.append(&val,1);
	return *this;
}

LogStream& LogStream::operator<<(char* val){
	if(val!=nullptr)
		buffer.append(val,strlen(val));
	else
		buffer.append("(null)",6);
	return *this;
}

LogStream& LogStream::operator<<(unsigned char* val){
	*this<<reinterpret_cast<char*>(val);     //强转！
	return *this;
}

LogStream& LogStream::operator<<(string& val){
	char* tmp=const_cast<char*>(val.c_str());
	*this<<tmp;
	return *this;
}

LogStream& LogStream::operator<<(void* val){
	char* tmp=static_cast<char*>(val);
	*this<<tmp;
	return *this;
}

/*int main()
{
	LogStream log;
	string a="1.314aasadddjg";
	log<<a;
	char* data=(log.get_buffer()).get_data();
	cout<<"(log.get_buffer()).length():"<<(log.get_buffer()).length()<<endl;
	for(int i=0;i<strlen(data);++i)
	{
		cout<<*(data+i)<<endl;
	}
	return 0;
}*/
 
 