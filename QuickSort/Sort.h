#pragma once
#include<vector>
#include<iostream>
#include<queue>
template<typename T>
class List;

template<typename T>
class Node {
	friend class List<T>;
	T val;
	Node* next;
	Node() :next(nullptr) {}
	Node(T val) :val(val), next(nullptr) {}

};

template<typename T>
class List {
private:
	Node<T>* head;
public:
	List() : head(new Node<T>()) {}
	List(std::vector<T> nums) :List() {
		Node<T>* help = head;
		for (auto& n : nums) {
			help->next = new Node<T>(n);
			help = help->next;
		}

		std::cout << "Construct: " << std::endl;
		Print();
	}

	//데蕨졍깊우醵탤埼
	void QuickSort() {
		HelpQuick(head->next, nullptr);
		std::cout << "After QuickSort of List: " << std::endl;
		Print();
	}

	void HelpQuick(Node<T>* begin, Node<T>* end) {
		if (begin == end) return;
		T key = begin->val;
		Node<T>* prepi = begin;
		Node<T>* pi = begin->next, * pj = begin->next;
		while (pj != end) {
			if (pj->val < key) {
				std::swap(pi->val, pj->val);
				prepi = pi;
				pi = pi->next;
			}
			pj = pj->next;
		}
		std::swap(begin->val, prepi->val);
		HelpQuick(begin, prepi);
		HelpQuick(pi, end);
	}

	//데蕨졍깊우醵탤埼（렷뒵백）
	void NoRecurQuickSort() {
		std::queue<std::pair<Node<T>*, Node<T>*>> NodeQueue;
		NodeQueue.emplace(std::pair<Node<T>*, Node<T>*>(head->next, nullptr));
		while (!NodeQueue.empty()) {
			Node<T>* low = NodeQueue.front().first, *high = NodeQueue.front().second;
			NodeQueue.pop();
			if (low == high) continue;
			Node<T>* pi = low->next, *pj = pi, *pre = low;
			T key = low->val;
			while (pj != high) {
				if (pj->val < key) {
					std::swap(pi->val, pj->val);
					pre = pi;
					pi = pi->next;
				}
				pj = pj->next;
			}
			std::swap(low->val, pre->val);
			NodeQueue.push(std::pair<Node<T>*, Node<T>*>(low, pre));
			NodeQueue.push(std::pair<Node<T>*, Node<T>*>(pi, high));
		}
		std::cout << "After NoRecurQuickSort of List: " << std::endl;
		Print();
	}

	void Print() {
		Node<T>* cur = head->next;
		while (cur) {
			std::cout << cur->val << " ";
			cur = cur->next;
		}
		std::cout << std::endl;
	}
};

//鑒莉우醵탤埼
template<typename T>
void QuickSort(std::vector<T>& nums, size_t begin, size_t end) {
	if (begin >= end) return;
	T key = nums[begin];
	int i = begin + 1, j = begin + 1;
	while (j <= end) {
		if (nums[j] < key) {
			std::swap(nums[i], nums[j]);
			++i;
		}
		++j;
	}
	std::swap(nums[begin], nums[i - 1]);
	QuickSort(nums, begin, i - 2);
	QuickSort(nums, i, end);
}

//鑒莉우醵탤埼끽숨경굶
template<typename T>
void QuickSortNormal(std::vector<T>& nums, size_t begin, size_t end) {
	if (begin >= end) return;
	T key = nums[begin];
	int i = begin, j = end;
	while (i < j) {
		while (i < j && nums[j] >= key) {
			--j;
		}
		nums[i] = nums[j];
		while (i < j && nums[i] < key) {
			++i;
		}
		nums[j] = nums[i];
	}
	nums[i] = key;
	QuickSortNormal(nums, begin, i - 1);
	QuickSortNormal(nums, i + 1, end);
}



