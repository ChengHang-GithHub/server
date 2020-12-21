#include<iostream>
#include<stdlib.h>
#include<string>
using namespace std;
/*template<typename T>
const char* transformat(T number)
{
	string str_=to_string(number);
	const char* str=str_.c_str();
	return str;
}*/
int main()
{
	long long val=-100000000;
	//const char* str=transformat(val);
	string str=to_string(val);
	cout<<str<<endl;
	cout<<str.length()<<endl;
	return 0;
}