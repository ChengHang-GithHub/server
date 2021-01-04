#ifndef __THREAD_POOL__
#define __THREAD_POOL__

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace myserver {

class ThreadPool {
public:
	ThreadPool(int numWorkers): stop_(false)
	{
		numWorkers = numWorkers <= 0 ? 1 : numWorkers;
		for(int i = 0; i < numWorkers; ++i)
		std::thread(		
		[this]() {     //lambda表达式来书写线程函数，捕获当前类的this指针
				std::unique_lock<std::mutex> locker(lock_);    //std::unique_lock<std::mutex>
						while(true) {
							if(!tasks.empty()) {
								auto task = std::move(tasks.front());
								tasks.pop();
								locker.unlock();   //解锁
								task();
								locker.lock();   //注意这里要上锁
							} 
							else if(stop_) break;
							else cond_.wait(locker);   //void wait (unique_lock<mutex>& lck)
						}
			}).detach();
	}
   ~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> locker(lock_);//上锁！！！
			stop_ = true;
		} 
		cond_.notify_all();
	}
    void pushJob(const std::function<void()>& job)
	{
		{
			std::unique_lock<std::mutex> locker(lock_);//上锁！
			tasks.emplace(job);
		}
		cond_.notify_one();
	}
	/*template<class F>
    void pushJob(F&& task) 
	{
        {
            std::lock_guard<std::mutex> locker(lock_);  //std::lock_guard<std::mutex>
            tasks.emplace(std::forward<F>(task));   //std::forward<>  类模板用于实现完美转发
        }
        cond_.notify_one();
    }*/
private:
    std::mutex lock_;
    std::condition_variable cond_;
    std::queue<std::function<void()>> tasks;     //线程池任务队列
    bool stop_;                      
};
}
#endif
