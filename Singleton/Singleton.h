#pragma once
#include<mutex>
#include<thread>

//����ʽ����̬ʵ�֣�
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

//����ʽ���Ǿ�̬ʵ�֣�
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
	class Deletor {  //������������
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
