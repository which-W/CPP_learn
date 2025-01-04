#include<ostream>
#include<type_traits>
template<typename T>
struct get_type
{
	using type = T;
};

template<typename T>
struct get_type<T*>
{
	using type = T;
};
template<typename T>
class Myarray
{
	using iterator = T*;
	using const_iterator = const T*;
public:
	Myarray(size_t count);
	Myarray(const std::initializer_list<T>& list);
	Myarray(const std::initializer_list<T>&& list);
	~Myarray();
	iterator bigin()const;
	const_iterator cbigin()const;
private:
	std::vector<int> data;
};

template<typename T>
Myarray<T>::Myarray(size_t count)
{
	if (count) {
		data = new T[count]();
	}
	else {
		data = nullptr;
	}
}


template<typename T>
Myarray<T>::~Myarray()
{
	if (data) {
		delete[] data;
	}
}
template<typename T>
typename Myarray<T>::iterator Myarray<T>::bigin()const {
	return data;
};

template<typename T>
typename Myarray<T>::const_iterator Myarray<T>::cbigin()const {
	return data;
};

template<typename T>
Myarray<T>::Myarray(const std::initializer_list<T>& list) {
	if (list.size()) {
		int count = 0;
		data = new T[list.size()]();
		//这里用萃取技术来判断T是否是指针
		if (std::is_pointer<T>::value) {
			for (auto elem : list) {
				data[count++] = new typename get_type<T>::type(*elem);
			}
		}
		else {
		for (const auto &elem : list) {
			data[count++] = elem;
		}
		}
	else {
		data = nullptr;
	}
	}
};

template<typename T>
Myarray<T>::Myarray(const std::initializer_list<T>&& list) {
	if (list.size()) {
		int count = 0;
		data = new T[list.size()]();
		for (const auto& elem : list) {
				data[count++] = elem;
		}
	else {
		data = nullptr;
	}
	}
};