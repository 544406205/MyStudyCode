#pragma once
#include<iostream>

class Count { //������
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
	friend class mshared_ptr;  //Ϊ��֧�ָ��ൽ�����ת��

public:
	explicit mshared_ptr(T* ptr = nullptr) :_ptr(ptr){
		if (_ptr) {
			_count = new Count();
		}
	}
	//��������ģ�壨Ϊ�˼����ظ����룩
	template<class U>
	void copy_consturct_from(const mshared_ptr<U>& _left)  {
		this->_ptr = _left._ptr;
		this->_count = _left._count;
		_left._count->add_count();
	}

	/*
	Ϊ��֧�ָ��ൽ�����ת������Ҫ�õ�ģ�壬

	��ģ���ڵ���ʱ��չ����ʵ������������һֱ��������������������Զ���Ŀ������캯��
	
	������������Ϊû���Զ���Ŀ������죬������Ĭ�Ͽ������캯��

	��˲������ģ���ȥ����Ĭ�Ͽ������캯����������ƶ�����ͬ��
	*/
	//�������캯������ֹ����Ĭ�Ͽ�������
	mshared_ptr(const mshared_ptr& other) {
		copy_consturct_from(other);
	}
	//�������캯��ģ��
	template<class U>
	mshared_ptr(const mshared_ptr<U>& other) {
		copy_consturct_from(other);
	}
	//�ƶ�����ģ�壨Ϊ�˼����ظ����룩
	template<class U>
	void move_consturct_from(mshared_ptr<U>&& _right) noexcept {
		this->_ptr = _right._ptr;
		this->_count = _right._count;
		_right._ptr = nullptr;
		_right._count = nullptr;
	}
	//�ƶ����캯������ֹ����Ĭ���ƶ�����
	mshared_ptr(mshared_ptr&& other) noexcept {
		move_consturct_from(std::move(other));
	}
	//�ƶ����캯��ģ��
	template<class U>
	mshared_ptr(mshared_ptr<U>&& other) {
		move_consturct_from(std::move(other));
	}

	//������ֵģ��
	template<class U>
	mshared_ptr& operator=(const mshared_ptr<U>& other) {
		mshared_ptr(other).swap(*this); //���ÿ������죬������ֵ�������������ֵ����
		return *this;
	}
	//�ƶ���ֵģ��
	template<class U>
	mshared_ptr& operator=(mshared_ptr<U>&& other) noexcept {
		mshared_ptr(std::move(other)).swap(*this); //�����ƶ����죬������ֵ�������������ֵ����
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
	//����swap
	void swap(mshared_ptr& other) {
		std::swap(this->_ptr, other._ptr);
		std::swap(this->_count, other._count);
	}
	//�������ü���
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