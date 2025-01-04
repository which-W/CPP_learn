#include<iostream>
#include<algorithm>
#include<vector>

int main() {
	//寻找相邻元素算法,返回一个迭代器
	std::vector<int> ivce{ 1,2,3,3,4,5,6 };
	//adjacent_find可以判断是否有相邻元素（就是相邻重复元素）如果有则返回相同的第一个元素没有则返回end
	//可以修改相邻元素的定义不一定是相同也可以在参数加上表达式表示大于4的相邻数字。
	auto iter = std::adjacent_find(ivce.cbegin(), ivce.cend(), [](int elem1, int elem2){
		return elem1 >= 4 && elem2 >= 4;
		});
	if (iter == ivce.cend()) {
		std::cout << "not adjaced element" << std::endl;

	}
	else {
		std::cout << std::distance(ivce.cbegin(), iter);
	}
		return 0;
	//二分查找算法
	std::vector<int> ivce2{ 1,2,3,3,4,5,6 };
	//先排序
	std::sort(ivce2.cbegin(), ivce2.cend());
	//二分查找,返回的是一个Bool
	bool finded = std::binary_search(ivce2.cbegin(), ivce2.cend(), 4);


	//也可以自定义可调用参数
	std::vector<int> ivce3{ 1,2,3,3,4,5,6 };
	//先排序，必须二分的和可调用参数一样
	std::sort(ivce3.cbegin(), ivce3.cend(), [](int elem, int elem2) {
		return elem > elem2;
		});
	//二分查找,返回的是一个Bool,第一个elem表示每一个数，第二个elem2表示所找的4（即第三个参数）
	bool finded = std::binary_search(ivce3.cbegin(), ivce3.cend(), 4 , [](int elem, int elem2) {
		return elem > elem2;
		});

	//计数算法
	std::vector<int> ivce4{ 1,2,3,3,4,5,6 };
	int sum = std::count(ivce4.cbegin(), ivce4.cend(), 3);
	return sum;
	//有几个大于三的元素
	//std::vector<int> ivce4{ 1,2,3,3,4,5,6 };
	//int sum = std::count(ivce4.cbegin(), ivce4.cend(), 3, [](int elem) {
		//return elem > 3;
	//	});
	//return sum;
}