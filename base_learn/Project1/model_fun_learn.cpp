#include<ostream>
#include<vector>
#include<functional>
namespace mystd {
	//函数模板
	//当使用默认参数类型时所使用的参数也必须是默认的
	//其中的void表示函数不返回任何值而int表示函数需要传入的值类型
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
	//使用模板函数使得每一个数加一；
	/*当使用默认参数时[](int& elem) {
	elem++;
}可以省略*/
	mystd::for_each(ive.begin(), ive.end(), [](int& elem) {
		elem++;
		});
	return 0;
}