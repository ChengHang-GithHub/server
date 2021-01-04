#include "sqlconnpool.h"

SqlConnPool::SqlConnPool() {
    useCount_ = 0;
    freeCount_ = 0;
}

SqlConnPool* SqlConnPool::Instance() {   //外界只能通过这个函数创建SqlConnPool对象
    static SqlConnPool connPool;
    return &connPool;
}

void SqlConnPool::Init(const char* host, int port,const char* user,const char* pwd, const char*dbName,int connSize = 10) 
{
    assert(connSize > 0);
    for (int i = 0; i < connSize; i++) {
        MYSQL *sql = nullptr;
        sql = mysql_init(sql);
        if (!sql) {
            //LOG_ERROR("MySql init error!");
			std::cout<<"MySql init error!"<<std::endl;
            assert(sql);
        }
        sql = mysql_real_connect(sql, host,
                                 user, pwd,
                                 dbName, port, nullptr, 0);
        if (!sql) {
            //LOG_ERROR("MySql Connect error!");
			std::cout<<"MySql Connect error!"<<std::endl;
        }
        connQue_.push(sql);
    }
    MAX_CONN_ = connSize;
    sem_init(&semId_, 0, MAX_CONN_);   //信号量初始值为MAX_CONN_
}

MYSQL* SqlConnPool::GetConn() {
    MYSQL *sql = nullptr;
    if(connQue_.empty()){
        //LOG_WARN("SqlConnPool busy!");
		std::cout<<"SqlConnPool busy!"<<std::endl;
        return nullptr;
    }
    sem_wait(&semId_);//P操作,等待信号量,如果信号量的值大于0,将信号量的值减1,立即返回。如果信号量的值为0,则线程阻塞
    {
        std::lock_guard<std::mutex> locker(mtx_);
        sql = connQue_.front();
        connQue_.pop();
    }
    return sql;
}

void SqlConnPool::FreeConn(MYSQL* sql) {
    assert(sql);
    std::lock_guard<std::mutex> locker(mtx_);   //上锁
    connQue_.push(sql);            //放回去
    sem_post(&semId_);         //V操作,释放信号量,让信号量的值加1。
}

void SqlConnPool::ClosePool() {
    std::lock_guard<std::mutex> locker(mtx_);    //上锁
    while(!connQue_.empty()) {
        auto item = connQue_.front();   //出队
        connQue_.pop();
        mysql_close(item);     //关闭连接
    }
    mysql_library_end();        
}

int SqlConnPool::GetFreeConnCount() {
    std::lock_guard<std::mutex> locker(mtx_);
    return connQue_.size();
}

SqlConnPool::~SqlConnPool() {
    ClosePool();
}



//mysql_init()
//mysql_real_connect()
//mysql_close()
//mysql_library_end()

