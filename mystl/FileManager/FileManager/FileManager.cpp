// FileManager.cpp: 定义应用程序的入口点。
//

#include "FileManager.h"
#include <memory>
using namespace std;

int main()
{
	{
		//第一种方法
		std::shared_ptr<FILE> file(fopen("example.txt", "w"), FileDeleter());
		if (file) {
			std::cout << "File opened successfully." << std::endl;
			printf("File pointer: %p\n", file.get());
			fprintf(file.get(), "Hello, World!\n");
		}

	}

	{
		//第二种方法
		auto FileDeleter = [](FILE* file) {
			if (file)
			{
				fclose(file);
			}
		};
		std::unique_ptr<FILE, decltype(FileDeleter)> file(fopen("example.txt", "w"), FileDeleter);
		if (file) {
			std::cout << "File opened successfully." << std::endl;
			printf("File pointer: %p\n", file.get());
			fprintf(file.get(), "Hello, World!\n");
		}

	
	return 0;
}
