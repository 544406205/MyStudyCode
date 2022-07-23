#include"mshared_ptr.h"
#include<memory>
class Base {
public:
	void ok() {

	}
};

class Son : public Base {

};

int main() {

	mshared_ptr<Base> ptr1(new Son());
	std::cout << ptr1.get_count() << std::endl;
	std::cout << &ptr1 << std::endl;
	mshared_ptr<Son> ptr2(new Son());
	
	std::cout << ptr2.get_count() << std::endl;
	std::cout << &ptr2 << std::endl;
	ptr1 = ptr2;
	std::cout << ptr1.get_count() << std::endl;
	std::cout << ptr2.get_count() << std::endl;

	
	
	mshared_ptr<Son> ptr3(new Son());
	std::cout << ptr3.get_count() << std::endl;
	ptr2 = std::move(ptr3);
	std::cout << ptr1.get_count() << std::endl;
	std::cout << ptr2.get_count() << std::endl;
	//std::cout << ptr3.get_count() << std::endl;
	std::cout << &ptr1 << " " << &ptr2 << " " << &ptr3 << std::endl;

	return 0;
}