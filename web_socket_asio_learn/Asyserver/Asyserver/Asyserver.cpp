#include "CServer.h"
using namespace std;
#include <iostream>
int main()
{
	try {
		boost::asio::io_context  io_context;
		CServer s(io_context, 10086);
		io_context.run();
	}
	catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
	boost::asio::io_context io_context;
}