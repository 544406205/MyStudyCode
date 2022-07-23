#pragma once
#include<queue>
#include<functional>
#include<future>
#include"Myqueue.h"
using Func = std::function<void()>;

class ThreadPool {
public:
	ThreadPool(size_t t_num):m_threads(std::vector<std::thread>(t_num)), shutdown(false) {
		Init();
	}
	~ThreadPool() {
		Shutdown();
	}
	void Init() {
		for (int i = 0; i < m_threads.size(); ++i) {
			m_threads[i] = std::thread([this]() {
				while (!shutdown) {
					Func func;
					func = std::move(mqueue.pop());
					func();
				}
			});
		}
	}
	template<typename F, typename ...Args>
	auto Submit(F &&f, Args&& ...arg) ->std::future<decltype(f(arg...))> {
		auto wrapper = std::make_shared<std::packaged_task<decltype(f(arg...))()>>(std::bind(std::forward<F>(f), std::forward<Args>(arg)...));
		mqueue.push([wrapper]() {
			(*wrapper)();
		});
		return wrapper->get_future();
	}
	void Shutdown() {
		shutdown = true;
		mcv.notify_all();
		for (auto& t : m_threads) {
			if (t.joinable()) {
				t.join();
			}
		}
	}
private:
	bool shutdown;
	std::vector<std::thread> m_threads;
	Myqueue<Func> mqueue;
	std::condition_variable mcv;
};


