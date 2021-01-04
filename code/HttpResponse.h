#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__
#include "Buffer.h"
#include <map>
#include <string>
#include <mutex>
#include <iostream>
#include <cassert>
#include <cstring>
#include <fcntl.h> // open
#include <unistd.h> // close
#include <sys/stat.h> // stat
#include <sys/mman.h> // mmap, munmap
#define CONNECT_TIMEOUT 500 // 非活跃连接500ms断开

namespace myserver {

class HttpResponse 
{
public:
    HttpResponse(int statusCode, std::string path, bool keepAlive)//初始化时传入：状态码、URL路径、是否为长连接
        : statusCode_(statusCode),
          path_(path),
          keepAlive_(keepAlive),
		  fileSize(0)
    {		
	}
    ~HttpResponse() {
		munmap(srcAddr, fileSize);//解除映射
		//std::cout<<"unmmap:"<<fileSize<<std::endl;
	}
    Buffer makeResponse();    
private:
    std::string __getFileType();
	void doErrorResponse(Buffer& output, std::string message);
    void doStaticRequest(Buffer& output, long fileSize);

private:
    static const std::map<int, std::string> statusCode2Message;//存储四种状态码
    static const std::map<std::string, std::string> suffix2Type;//存储后缀类型
    int statusCode_; // 响应状态码
    std::string path_; // 请求资源路径
    bool keepAlive_; // 是否为长连接
	char* srcAddr;
	int fileSize;
}; 
} 
#endif
