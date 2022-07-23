#pragma once
#include<mutex>
#include<thread>

//懒汉式（静态实现）
class SingletonStatic {
public:
	static SingletonStatic& GetSingleton() {
		static SingletonStatic single;
		return single;
	}
private:
	SingletonStatic() {};
	SingletonStatic(const SingletonStatic&);
	~SingletonStatic() {};
	SingletonStatic& operator=(SingletonStatic&&db) = delete;
	SingletonStatic& operator=(const SingletonStatic& db) = delete;
};

//懒汉式（非静态实现）
static std::once_flag oc;
class SingletonNoStatic {
public:
	static SingletonNoStatic* GetSingleton() {
		std::call_once(oc, []() {
			single = new SingletonNoStatic();
		});
		return single;
	}
private:
	class Deletor {  //用来析构单例
	public:
		~Deletor() {
			if (SingletonNoStatic::single != nullptr) {
				delete SingletonNoStatic::single;
			}
		}
	};
	Deletor deletor;
	SingletonNoStatic() {};
	SingletonNoStatic(const SingletonNoStatic&);
	~SingletonNoStatic() {};
	SingletonNoStatic& operator=(SingletonNoStatic&& db) = delete;
	SingletonNoStatic& operator=(const SingletonNoStatic& db) = delete;
	static SingletonNoStatic* single;
};
