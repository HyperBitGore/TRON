#include <iostream>
#define ASIO_STANDALONE
#include <asio.hpp>
#include <vector>
#include <thread>
#include <mutex>

enum messages { SETDIR, NEWDUMMY, RECIEVED, CLEAR, SETCOORDS};
struct Player {
	messages m;
	int x;
	int y;
	int dir;
	int index;
	bool write;
	int sx; 
	int sy;
};
std::mutex iosafe;
std::vector<asio::ip::tcp::socket*> sockets;
std::vector<Player> players;
bool exitf = false;
bool clear = false;
asio::io_context io;
void readInput(Player *p, asio::ip::tcp::socket* soc) {
	int buf[5];
	size_t bytes = (*soc).available();
	(*p).write = false;
	asio::error_code ecode;
	if (bytes > 0) {
		asio::read(*soc, asio::buffer(buf), ecode);
		//std::cout << "Recieved data: " << buf << " From: " << soc->remote_endpoint() << std::endl;
		switch (buf[0]) {
		case SETCOORDS:
			(*p).x = buf[1];
			(*p).y = buf[4];
			(*p).m = SETCOORDS;
			break;
		case CLEAR:
			clear = true;
			break;
		}
		(*p).dir = buf[3];
		(*p).write = true;
		if (ecode == asio::error::eof) {
			return;
		}
		else if (ecode) {
			return;
		}
	}
}
//too much data being written, figure out why
void sendChanges(Player *p, asio::ip::tcp::socket* soc) {
	asio::error_code ec;
	for (auto& i : players) {
		if (i.write) {
			int buf[5];
			buf[0] = i.index;
			buf[1] = i.m;
			buf[3] = i.dir;
			switch (i.m) {
			case SETCOORDS:
				buf[2] = i.x;
				buf[4] = i.y;
				break;
			}
			asio::write(*soc, asio::buffer(buf), ec);
			int readb[1];
			asio::read(*soc, asio::buffer(readb), ec);
			if (readb[0] == RECIEVED) {

			}
		}
	}
}
void sendReset(){
	clear = false;
	int j = 0;
	for (auto& i : sockets) {
		int buf[5];
		buf[0] = j;
		buf[1] = CLEAR;
		buf[2] = players[j].sx;
		buf[3] = players[j].sy;
		buf[4] = 0;
		asio::error_code ec;
		asio::write(*i, asio::buffer(buf), ec);
		j++;
	}
}
void sendNewDummy(int sx, int sy, int s, int sdir, asio::ip::tcp::socket* sock) {
	std::cout << "Writing new peer to: " << sock->remote_endpoint() << std::endl;
	int buf[5];
	asio::error_code ec;
	buf[0] = sy;
	buf[1] = NEWDUMMY;
	buf[2] = sx;
	buf[3] = s;
	buf[4] = sdir;
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
			sendNewDummy(sx, sy, s, 1, sockets[i]);
		}
		sockets.push_back(soc);
		Player p = { SETCOORDS, sx, sy, 1 };
		p.index = s;
		p.write = true;
		p.sx = p.x;
		p.sy = p.y;
		players.push_back(p);
		for (int i = 0; i < players.size(); i++) {
			sendNewDummy(players[i].x, players[i].y, players[i].index, 1, soc);
		}
		sx += 10;
		s++;
		std::cout << "New player from: " << soc->remote_endpoint() << std::endl;
		std::cout << "Players size: " << players.size() << std::endl;
		iosafe.unlock();
	}
}

int main() {
	std::cout << "Starting up server " << std::endl;
	std::cout << "Server running, awaiting connections from clients" << std::endl;
	std::thread listen(listeningThread);
	while (!exitf) {
		if (iosafe.try_lock()) {
			if (!clear) {
				for (int i = 0; i < players.size(); i++) {
					readInput(&players[i], sockets[i]);
					sendChanges(&players[i], sockets[i]);
				}
			}
			else {
				sendReset();
			}
			iosafe.unlock();
		}
	}
	listen.join();
	return 0;
}