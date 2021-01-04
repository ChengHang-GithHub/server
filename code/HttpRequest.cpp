#include "HttpRequest.h"
#include <iostream>
#include <cassert>
#include <unistd.h>

using namespace myserver;

HttpRequest::HttpRequest(int fd)
    : fd_(fd),
      working_(false),
      timer_(nullptr),          //定时器初始化为空
      state_(ExpectRequestLine),    //初始化状态
      method_(Invalid),       
      version_(Unknown)
{
	headers_.clear();
    assert(fd_ >= 0);
}

HttpRequest::~HttpRequest()
{
	headers_.clear();
    close(fd_);    //析构时关闭文件描述符
}

int HttpRequest::read(int* savedErrno)
{
    int ret = inBuff_.readFd(fd_, savedErrno);  //从内核缓冲区读到用户读缓冲区
    return ret;
}

int HttpRequest::write(int* savedErrno)
{
    int ret = outBuff_.writeFd(fd_, savedErrno); //从用户缓冲区写入内核缓冲区
    return ret;
}

bool HttpRequest::parseRequest()     //解析请求行和头部信息
{
    bool ok = true;
    bool hasMore = true;

    while(hasMore) 
	{
        if(state_ == ExpectRequestLine) // 处理请求行
		{
            const char* crlf = inBuff_.findCRLF();   //寻找换行符'/r/n'
            if(crlf) 
			{
                ok = __parseRequestLine(inBuff_.peek(), crlf);  //解析请求行
                if(ok) 
				{
                    inBuff_.retrieveUntil(crlf + 2);
                    state_ = ExpectHeaders;    //状态转移
                } else 
				{
                    hasMore = false;
                }
            } 
			else 
			{
                hasMore = false;
            }
        } 
		else if(state_ == ExpectHeaders) // 处理报文头部字段
		{
            const char* crlf = inBuff_.findCRLF();//寻找回车换行符'/r/n'
            if(crlf) 
			{
                const char* colon = std::find(inBuff_.peek(), crlf, ':');//寻找':'
                if(colon != crlf)     //找到':'
				{
                    __addHeader(inBuff_.peek(), colon, crlf);
                } 
				else  //没找到':'
				{
					if(method_==Post)
						state_ = ExpectBody;
					else
					{
						state_ = GotAll;
						hasMore = false;
					}  
                }
                inBuff_.retrieveUntil(crlf + 2);
            } 
			else 
			{
                hasMore = false;
            } 
        } 
		else if(state_ == ExpectBody) {
            // TODO 处理报文体 
			Parsebody(inBuff_.peek(), inBuff_.beginWrite());
			state_ = GotAll;
			hasMore = false;
        }
    }
	//std::cout<<"state_:"<<state_<<std::endl;
    return ok;
}

std::string HttpRequest::getMethod() const
{
    std::string res;
    if(method_ == Get)
        res = "GET";
    else if(method_ == Post)
        res = "POST";
    else if(method_ == Head)
        res = "HEAD";
    else if(method_ == Put)
        res = "Put";
    else if(method_ == Delete)
        res = "DELETE";
    
    return res;
}

std::string HttpRequest::getHeader(const std::string& field) const
{
    std::string res;
    auto itr = headers_.find(field);
    if(itr != headers_.end())
        res = itr -> second;
    return res;
}

bool HttpRequest::keepAlive() const
{
    std::string connection = getHeader("Connection");
    bool res = connection == "Keep-Alive" || 
               (version_ == HTTP11 && connection != "close");

    return res;
}

void HttpRequest::resetParse()
{
    state_ = ExpectRequestLine; // 报文解析状态
    method_ = Invalid; // HTTP方法
    version_ = Unknown; // HTTP版本
    path_ = ""; // URL路径
    query_ = ""; // URL参数
    headers_.clear(); // 报文头部
}

bool HttpRequest::UserVerify(const std::string &name, const std::string &pwd, bool isLogin) {
    if(name == "" || pwd == "") { return false; }
    MYSQL* sql;
    SqlConnRAII(&sql,  SqlConnPool::Instance());
    assert(sql);
    
    bool flag = false;
    unsigned int j = 0;
    char order[256] = { 0 };
    MYSQL_FIELD *fields = nullptr;
    MYSQL_RES *res = nullptr;
    
    if(!isLogin) { flag = true; }    //注册
    /* 查询用户及密码 */
    snprintf(order, 256, "SELECT username, password FROM user WHERE username='%s' LIMIT 1", name.c_str());

    if(mysql_query(sql, order)) { 
        mysql_free_result(res);
        return false; 
    }
    res = mysql_store_result(sql);   //存储查询结果
    j = mysql_num_fields(res);       //列数
    fields = mysql_fetch_fields(res);    //获取所有列 

    while(MYSQL_ROW row = mysql_fetch_row(res)) {    //获取结果行
        std::string password(row[1]);

        if(isLogin) {      //已注册，验证密码是否正确
            if(pwd == password) { flag = true; }
            else {          //密码错误
                flag = false;
                std::cout<<"pwd error!"<<std::endl;
            }
        } 
        else {  					//未注册，却查询到结果，说明注册的用户名重复
            flag = false; 
			std::cout<<"user used!"<<std::endl;
        }
    }
    mysql_free_result(res);

    if(!isLogin && flag == true) {     //注册
        bzero(order, 256);
        snprintf(order, 256,"INSERT INTO user(username, password) VALUES('%s','%s')", name.c_str(), pwd.c_str());
        if(mysql_query(sql, order)) { 
			std::cout<<"Insert error!"<<std::endl;
            flag = false; 
        }
        flag = true;
    }
    SqlConnPool::Instance()->FreeConn(sql);
    std::cout<<"UserVerify success!!"<<std::endl;
    return flag;
}

void HttpRequest::ParseFromUrlencoded_() {
    if(body_.size() == 0) { return; }

    int num = 0;
    int n = body_.size();
    int i = 0, j = 0;
	//std::cout<<"body:"<<body_<<std::endl;

    for(; i < n; i++) {
        char ch = body_[i];
        switch (ch) {
        case '=':
            key = body_.substr(j, i - j);
            j = i + 1;
            break;
        case '&':
            value = body_.substr(j, i - j);
            j = i + 1;
            break;
        default:
            break;
        }
    }
	//std::cout<<"key:"<<key<<std::endl;
	//std::cout<<"value:"<<value<<std::endl;
}

void HttpRequest::Parsebody(const char* start, const char* end) {
	body_=std::string(start,end);
    if(method_ == Post && headers_["Content-Type"] == "application/x-www-form-urlencoded") 
	{
        ParseFromUrlencoded_();
		bool isLogin=false;
		if(path_=="../resources/register.html")
			isLogin=false;
		else if(path_=="../resources/login.html")
			isLogin=true;
		else{
			std::cout<<"path should be ../resources/register.html or ../resources/login.html! "<<std::endl;
		}
		if(UserVerify(key, value, isLogin)) 
			path_ = "../resources/welcome.html";
		else
			path_ = "../resources/error.html";
    } 
}
















bool HttpRequest::__parseRequestLine(const char* begin, const char* end)
{
    bool succeed = false;
    const char* start = begin;
    const char* space = std::find(start, end, ' '); //寻找空格
    if(space != end && __setMethod(start, space)) //设置方法
	{
        start = space + 1;     //移动指针
        space = std::find(start, end, ' ');
        if(space != end) 
		{
            const char* question = std::find(start, space, '?');  //寻找'？'
            if(question != space) //找到了'?'
			{
                __setPath(start, question);   
                __setQuery(question, space);   //设置URL参数
            } 
			else //没找到'?'
			{
                __setPath(start, space);//设置URL路径
            }
            start = space + 1;
            succeed = end - start == 8 && std::equal(start, end - 1, "HTTP/1.");
            if(succeed) 
			{
                if(*(end - 1) == '1')
                    __setVersion(HTTP11);
                else if(*(end - 1) == '0')
                    __setVersion(HTTP10);
                else
                    succeed = false;
            } 
        }
    }
    return succeed;
}

bool HttpRequest::__setMethod(const char* start, const char* end)
{
    std::string m(start, end);
    if(m == "GET")
        method_ = Get;
    else if(m == "POST")
        method_ = Post;
    else if(m == "HEAD")
        method_ = Head;
    else if(m == "PUT")
        method_ = Put;
    else if(m == "DELETE")
        method_ = Delete;
    else
        method_ = Invalid;

    return method_ != Invalid;
}

void HttpRequest::__setPath(const char* begin, const char* end)
{ 
    std::string subPath;
    subPath.assign(begin, end);
    if(subPath == "/")
        subPath = "/index.html";
    path_= STATIC_ROOT+subPath;
	//std::cout<<"path_:"<<path_<<std::endl;
}

void HttpRequest::__setQuery(const char* begin, const char* end)
{ 
    query_.assign(begin, end); 
}

void HttpRequest::__setVersion(Version version) 
{ 
	version_ = version; 
}

void HttpRequest::__addHeader(const char* start, const char* colon, const char* end)
{
    std::string field(start, colon);
    ++colon;
    while(colon < end && *colon == ' ')
        ++colon;
    std::string value(colon, end);
    while(!value.empty() && value[value.size() - 1] == ' ')//去除最后的空格
        value.resize(value.size() - 1);
    headers_[field] = value;
}

