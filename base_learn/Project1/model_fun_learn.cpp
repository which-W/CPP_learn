#include<ostream>
#include<vector>
#include<functional>
namespace mystd {
	//����ģ��
	//��ʹ��Ĭ�ϲ�������ʱ��ʹ�õĲ���Ҳ������Ĭ�ϵ�
	//���е�void��ʾ�����������κ�ֵ��int��ʾ������Ҫ�����ֵ����
	using void_func_type = std::function<void(int&)>;
	template<typename iter_type, typename func_type = void_func_type>
	void for_each(iter_type first, iter_type end, func_type func = [](int& elem) {
		elem++;
		}) {

		for (auto iter = first; iter != end; iter++) {
			func(*iter);
		}
	}
}

int main() {
	std::vector<int> ive = { 1 ,2 ,3 ,4 ,5 };
	//ʹ��ģ�庯��ʹ��ÿһ������һ��
	/*��ʹ��Ĭ�ϲ���ʱ[](int& elem) {
	elem++;
}����ʡ��*/
	mystd::for_each(ive.begin(), ive.end(), [](int& elem) {
		elem++;
		});
	return 0;
}