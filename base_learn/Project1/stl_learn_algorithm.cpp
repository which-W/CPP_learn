#include<iostream>
#include<algorithm>
#include<vector>

int main() {
	//Ѱ������Ԫ���㷨,����һ��������
	std::vector<int> ivce{ 1,2,3,3,4,5,6 };
	//adjacent_find�����ж��Ƿ�������Ԫ�أ����������ظ�Ԫ�أ�������򷵻���ͬ�ĵ�һ��Ԫ��û���򷵻�end
	//�����޸�����Ԫ�صĶ��岻һ������ͬҲ�����ڲ������ϱ��ʽ��ʾ����4���������֡�
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
	//���ֲ����㷨
	std::vector<int> ivce2{ 1,2,3,3,4,5,6 };
	//������
	std::sort(ivce2.cbegin(), ivce2.cend());
	//���ֲ���,���ص���һ��Bool
	bool finded = std::binary_search(ivce2.cbegin(), ivce2.cend(), 4);


	//Ҳ�����Զ���ɵ��ò���
	std::vector<int> ivce3{ 1,2,3,3,4,5,6 };
	//�����򣬱�����ֵĺͿɵ��ò���һ��
	std::sort(ivce3.cbegin(), ivce3.cend(), [](int elem, int elem2) {
		return elem > elem2;
		});
	//���ֲ���,���ص���һ��Bool,��һ��elem��ʾÿһ�������ڶ���elem2��ʾ���ҵ�4����������������
	bool finded = std::binary_search(ivce3.cbegin(), ivce3.cend(), 4 , [](int elem, int elem2) {
		return elem > elem2;
		});

	//�����㷨
	std::vector<int> ivce4{ 1,2,3,3,4,5,6 };
	int sum = std::count(ivce4.cbegin(), ivce4.cend(), 3);
	return sum;
	//�м�����������Ԫ��
	//std::vector<int> ivce4{ 1,2,3,3,4,5,6 };
	//int sum = std::count(ivce4.cbegin(), ivce4.cend(), 3, [](int elem) {
		//return elem > 3;
	//	});
	//return sum;
}