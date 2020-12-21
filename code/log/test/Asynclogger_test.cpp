#include<iostream>
#include<stdlib.h>
#include<vector>
#include<memory>  //shared_ptr
using namespace std;
int main(){
	vector<shared_ptr<int>>myv;
	shared_ptr<int> ptr1(new int(1));
	cout<<"ptr1.use_count():"<<ptr1.use_count()<<endl;//1
	myv.push_back(ptr1);
	cout<<"ptr1.use_count():"<<ptr1.use_count()<<endl;//2
	shared_ptr<int> ptr2=myv.back();
	cout<<"ptr1.use_count():"<<ptr1.use_count()<<endl;//3
	myv.pop_back();
	cout<<"ptr1.use_count():"<<ptr1.use_count()<<endl;//2
	ptr1.reset();
	cout<<"ptr2.use_count():"<<ptr2.use_count()<<endl;//1
	return 0;
}