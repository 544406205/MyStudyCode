#pragma once
#include<iostream>

class Count { //计数类
private:
	size_t _count;

public:
	Count() :_count(1) {}
	
	size_t get_count() const {
		return _count;
	}

	size_t add_count() {
		return ++_count;
	}

	size_t reduce_count() {
		return --_count;
	}
};

template<class T>
class mshared_ptr {
private:
	T* _ptr;
	Count* _count;
	friend class mshared_ptr;  //为了支持父类到子类的转换

public:
	explicit mshared_ptr(T* ptr = nullptr) :_ptr(ptr){
		if (_ptr) {
			_count = new Count();
		}
	}
	//拷贝构造模板（为了减少重复代码）
	template<class U>
	void copy_consturct_from(const mshared_ptr<U>& _left)  {
		this->_ptr = _left._ptr;
		this->_count = _left._count;
		_left._count->add_count();
	}

	/*
	为了支持父类到子类的转换，需要用到模板，

	而模板在调用时才展开（实例化），所以一直到编译结束，不会生成自定义的拷贝构造函数
	
	而编译器会认为没有自定义的拷贝构造，会生成默认拷贝构造函数

	因此不会调用模板而去调用默认拷贝构造函数，下面的移动构造同理
	*/
	//拷贝构造函数，防止生成默认拷贝构造
	mshared_ptr(const mshared_ptr& other) {
		copy_consturct_from(other);
	}
	//拷贝构造函数模板
	template<class U>
	mshared_ptr(const mshared_ptr<U>& other) {
		copy_consturct_from(other);
	}
	//移动构造模板（为了减少重复代码）
	template<class U>
	void move_consturct_from(mshared_ptr<U>&& _right) noexcept {
		this->_ptr = _right._ptr;
		this->_count = _right._count;
		_right._ptr = nullptr;
		_right._count = nullptr;
	}
	//移动构造函数，防止生成默认移动构造
	mshared_ptr(mshared_ptr&& other) noexcept {
		move_consturct_from(std::move(other));
	}
	//移动构造函数模板
	template<class U>
	mshared_ptr(mshared_ptr<U>&& other) {
		move_consturct_from(std::move(other));
	}

	//拷贝赋值模板
	template<class U>
	mshared_ptr& operator=(const mshared_ptr<U>& other) {
		mshared_ptr(other).swap(*this); //调用拷贝构造，生成右值，交换，随后右值析构
		return *this;
	}
	//移动赋值模板
	template<class U>
	mshared_ptr& operator=(mshared_ptr<U>&& other) noexcept {
		mshared_ptr(std::move(other)).swap(*this); //调用移动构造，生成右值，交换，随后右值析构
		return *this;
	}

	~mshared_ptr() {
		if (_count) {
			if (_count->reduce_count() == 0) {
				delete _count;
				delete _ptr;
			}
		}
	}
	//重载swap
	void swap(mshared_ptr& other) {
		std::swap(this->_ptr, other._ptr);
		std::swap(this->_count, other._count);
	}
	//返回引用计数
	size_t get_count() {
		if (_ptr) return _count->get_count();
		return 0;
	}

	T* get() {
		return _ptr;
	}

	T& operator *() const{
		return *get();
	}

	T* operator->() {
		return get();
	}
};