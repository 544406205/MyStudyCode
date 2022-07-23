#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;


class Person {
public:
    string name;
    int age;
    //friend struct hash_name;//应对成员变量为私有情况
    Person(string n, int a) {
        name = n;
        age = a;
    }

    bool operator==(const Person& p) const {
        return name == p.name && age == p.age;
    }
};

template<typename K>
size_t _hash(const K& key) {
    return reinterpret_cast<size_t>(&key);
}

template<typename P>
size_t _hash(const P* ptr) {
    return reinterpret_cast<size_t>(ptr);
}

template<typename K>
class myhash {
public:
    size_t operator()(const K& key1) const {
        return _hash<K>(key1);
    }
};

template<typename K>
class myhash_compare {
public:
    size_t operator()(const K& key1, const K& key2) const {
        return key2 == key1;
    }
};

int main() {
    unordered_map<Person, int, myhash<Person>> ids;
    //unordered_map<Person, int, hash_name, equal_person> ids;  //不需要重载operator==写法
    ids[Person("Mark", 17)] = 40561;
    ids[Person("Andrew", 16)] = 40562;
    for (auto ii = ids.begin(); ii != ids.end(); ii++)
        cout << ii->first.name << " " << ii->first.age << " : " << ii->second
        << endl;
    getchar();
    return 0;
}


