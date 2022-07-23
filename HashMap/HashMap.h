#pragma once
#include<vector>
#include<algorithm>
static const std::vector<size_t> prime_list{
	53ul, 97ul, 193ul, 389ul, 769ul,
	1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
	49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
	1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
	50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
	1610612741ul, 3221225473ul, 4294967291ul
};
template<typename K, typename V> class HashMap;

template<typename K, typename V>
class HashFunc {
	friend class HashMap<K, V>;
	const K& operator()(const K& key) {
		return key;
	}
};

template<typename K, typename V>
class HashNode {
	friend class HashMap<K, V>;
	HashNode(){};
	HashNode(const K& key) { 
		kv.first = key;
	}
	HashNode(const std::pair<K, V>& kv) :kv(kv) { }
	std::pair<K, V> kv;
	HashNode* next;
}; 

template<typename K, typename V>
class HashMap {
private:
	std::vector<HashNode<K, V>*> map;
	size_t size;
	HashFunc<K, V> hash;

public:
	HashMap() :map(std::vector<HashNode<K, V>*>()), size(0) {};

	bool count(const K &key) {
		if (map.size() == 0) {
			return false;
		}
		size_t index = hash(key) % map.size();
		HashNode<K, V>* node = map[index];
		while (node) {
			if (node->kv.first == key) {
				return true;
			}
			node = node->next;
		}
		return false;
	}

	HashNode<K, V>* find(const K& key) {
		if (map.size() == 0) {
			return nullptr;
		}
		size_t index = hash(key) % map.size();
		HashNode<K, V>* node = map[index];
		while (node) {
			if (node->kv.first == key) {
				return node;
			}
			node = node->next;
		}
		return nullptr;
	}

	bool earse(const K& key) {
		if (map.size() == 0) {
			return false;
		}
		size_t index = hash(key) % map.size();
		HashNode<K, V>* node = map[index];
		HashNode<K, V>* pre;
		while (node) {
			if (node->kv.first == key) {
				if (pre == nullptr) {  //node为头节点
					map[index] = node->next;
				}
				else {  //node不为头节点
					pre->next = node->next;
				}
				delete node;
				return true;
			}
			pre = node;
			node = node->next;
		}
		return false;
	}

	void insert(const std::pair<K, V>& kv) {
		try_emplace(kv.first) = kv.second;
	}

	V& operator[](const K& key) {
		return try_emplace(key);
	}
	V& operator[](const K&& key) {
		return try_emplace(key);
	}

	V& try_emplace(const K& key) {
		HashNode<K, V>* node = find(key);
		if (!node) {
			if (map.size() == size) {  //当负载因子等于1时，开散列
				rehash();
			}
			node = new HashNode<K, V>(key);
			size_t index = hash(node->kv.first) % map.size();
			node->next = map[index];
			map[index] = node;
			++size;
		}
		return node->kv.second;
	}


	void rehash() {
		int tsize = get_prime(size);
		std::vector<HashNode<K, V>*> newmap(tsize);
		for (HashNode<K, V>* node : map) {
			while (node) {
				size_t index = hash(node->kv.first) % tsize;
				if (newmap[index] == nullptr) {  //如果map[index]空，则直接填入
					newmap[index] = node;
				}
				else {  //如果map[index]有值，头插node节点
					node->next = newmap[index];
					newmap[index] = node;
				}
				node = node->next;
			}
		}
		std::swap(map, newmap);
	}

	size_t get_prime(const int &tsize) {
		return *std::upper_bound(prime_list.begin(), prime_list.end(), tsize);
	}

};

