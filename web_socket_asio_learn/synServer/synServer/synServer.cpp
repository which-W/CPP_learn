#include<boost/asio.hpp>
#include<iostream>
#include<set>
#include<memory>
using namespace boost;
typedef std::shared_ptr<asio::ip::tcp::socket> socket_ptr;
std::set<std::shared_ptr<std::thread>> thread_set;
const int max_length = 1024;

void session(socket_ptr socket) {
	try
	{
		for (;;)
		{
			char data[max_length];
			memset(data, '\0', max_length);
			boost::system::error_code error;
			size_t length = socket->read_some(asio::buffer(data, max_length), error);
			if (error == boost::asio::error::eof) {
				std::cout << "connection closed by peer" << std::endl;
				break;
			}
			else if (error) {
				throw boost::system::system_error(error);
			}

			std::cout << "ip is :" << socket->remote_endpoint().address().to_string() << std::endl;
			std::cout << "receive:" << data << std::endl;

			asio::write(*socket, asio::buffer(data, max_length));
		}
	}
	catch (std::exception& ec)
	{
		std::cerr << "Exception:" << ec.what() ;
	}
}

void server(asio::io_context & ioc , unsigned short port_num) {
	asio::ip::tcp::acceptor ac(ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_num));
	for (;;)
	{
		socket_ptr sock(new asio::ip::tcp::socket(ioc));
		ac.accept(*sock);

		auto t = std::make_shared<std::thread>(session, sock);
		thread_set.insert(t);
	}
}
int main()
{
	asio::io_context ioc;
	server(ioc, 10086);
	for (auto t : thread_set) {
		t->join();
	}
}

