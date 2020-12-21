#include"Logger.h"
#include<string>
int main()
{
	logger log(__FILE__, __LINE__);
	log.get_stream().get_buffer();
	std::string str=((log.get_stream()).get_buffer()).get_data();
	for(auto x:str)
		std::cout<<x;
	return 0;
}