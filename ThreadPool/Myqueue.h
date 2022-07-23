#pragma once
#include<mutex>
#include<vector>
#include<thread>

template<typename T>
class Myqueue {
public:
	void push(T val) {
		std::unique_lock lock(mtx);
		m_arr.emplace_back(std::move(val));
		m_cv.notify_one();
	}
	T pop() {
		std::unique_lock lock(mtx);
		m_cv.wait(lock, [this] { return !m_arr.empty(); });
		T tmp = std::move(m_arr.back());
		m_arr.pop_back();
		return tmp;
	}
private:
	std::mutex mtx;
	std::condition_variable m_cv;
	std::vector<T> m_arr;
};