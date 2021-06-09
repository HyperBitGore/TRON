#include <iostream>
#define ASIO_STANDALONE
#include <asio.hpp>


int main() {
	bool exitf = false;
	asio::io_context co;
	asio::ip::tcp::acceptor accept(co, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13));
	std::cout << "Starting up server " << std::endl;
	while (!exitf) {
		asio::ip::tcp::socket soc(co);
		std::cout << "Server running, awaiting connections from clients" << std::endl;
		accept.accept(soc);
		asio::error_code ignore;
		std::string message = "Hello you!";
		asio::write(soc, asio::buffer(message), ignore);
		std::cout << "looped" << std::endl;
	}

	return 0;
}