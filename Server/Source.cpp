#include <iostream>
#define ASIO_STANDALONE
#include <asio.hpp>
#include <vector>
#include <thread>
#include <mutex>

enum messages { SETX, SETY, SETDIR, NEWDUMMY };
struct Player {
	messages m;
	int x;
	int y;
	int dir;
	int index;
	int chng;
};
std::mutex iosafe;
std::vector<asio::ip::tcp::socket*> sockets;
std::vector<Player> players;
bool exitf = false;
asio::io_context io;
void readInput(Player *p, asio::ip::tcp::socket* soc) {
	int buf[2];
	asio::error_code ecode;
	asio::read(*soc, asio::buffer(buf), ecode);
	std::cout << "Recieved data: " << buf << " From: " << soc->remote_endpoint() << std::endl;
	switch (buf[0]) {
	case SETX:
		(*p).x = buf[1];
		(*p).m = SETX;
		break;
	case SETY:
		(*p).y = buf[1];
		(*p).m = SETY;
		break;
	case SETDIR:
		(*p).dir = buf[1];
		(*p).m = SETDIR;
		break;
	}
	if (ecode == asio::error::eof) {
		return;
	}
	else if (ecode) {
		return;
	}
}
void sendChanges(Player p,asio::ip::tcp::socket* soc) {
	int buf[3];
	asio::error_code ec;
	std::cout << "Writing: " << p.m << " Index: " << p.index << " At: " << soc->remote_endpoint() << std::endl;
	buf[0] = p.index;
	buf[1] = p.m;
	switch (p.m) {
	case SETX:
		buf[2] = p.x;
		break;
	case SETY:
		buf[2] = p.y;
		break;
	}
	asio::write(*soc, asio::buffer(buf), ec);
}
void sendNewDummy(int sx, asio::ip::tcp::socket* sock) {
	std::cout << "Writing new peer to: " << sock->remote_endpoint() << std::endl;
	char buf[3];
	asio::error_code ec;
	buf[0] = sockets.size();
	buf[1] = NEWDUMMY;
	buf[2] = sx;
	asio::write(*sock, asio::buffer(buf), ec);
}

void listeningThread() {
	int sx = 0;
	int sy = 400;
	int s = 0;
	while (!exitf) {
		asio::ip::tcp::acceptor accept(io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13));
		asio::ip::tcp::socket* soc = new asio::ip::tcp::socket(io);
		asio::error_code ec;
		accept.listen();
		accept.accept(*soc, ec);
		iosafe.lock();
		for (int i = 0; i < sockets.size(); i++) {
			sendNewDummy(sx, sockets[i]);
		}
		sockets.push_back(soc);
		Player p = { SETY, sx, sy, 1 };
		p.index = s;
		p.chng = 0;
		players.push_back(p);
		sx += 10;
		s++;
		std::cout << "New player from: " << soc->remote_endpoint() << std::endl;
		iosafe.unlock();
	}
}

int main() {
	std::cout << "Starting up server " << std::endl;
	std::cout << "Server running, awaiting connections from clients" << std::endl;
	std::thread listen(listeningThread);
	while (!exitf) {
		if (iosafe.try_lock()) {
			for (int i = 0; i < sockets.size(); i++) {
				readInput(&players[i], sockets[i]);
				sendChanges(players[i],sockets[i]);
			}
			iosafe.unlock();
		}
	}
	listen.join();
	return 0;
}