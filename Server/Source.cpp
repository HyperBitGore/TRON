#include <iostream>
#define ASIO_STANDALONE
#include <asio.hpp>
#include <vector>
#include <thread>
#include <mutex>

enum messages { SETX, SETY, SETDIR, NEWDUMMY, RECIEVED};
struct Player {
	messages m;
	int x;
	int y;
	int dir;
	int index;
	bool write;
};
std::mutex iosafe;
std::vector<asio::ip::tcp::socket*> sockets;
std::vector<Player> players;
bool exitf = false;
asio::io_context io;
void readInput(Player *p, asio::ip::tcp::socket* soc) {
	int buf[4];
	size_t bytes = (*soc).available();
	(*p).write = false;
	asio::error_code ecode;
	if (bytes > 0) {
		asio::read(*soc, asio::buffer(buf), ecode);
		//std::cout << "Recieved data: " << buf << " From: " << soc->remote_endpoint() << std::endl;
		switch (buf[0]) {
		case SETX:
			if ((*p).x == buf[1]) {
				(*p).write = false;
				return;
			}
			(*p).x = buf[1];
			(*p).m = SETX;
			break;
		case SETY:
			if ((*p).y == buf[1]) {
				(*p).write = false;
				return;
			}
			(*p).y = buf[1];
			(*p).m = SETY;
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
	/*for (auto& i : players) {
		if (i.write) {
			int buf[4];
			buf[0] = i.index;
			buf[1] = i.m;
			buf[3] = i.dir;
			switch (i.m) {
			case SETX:
				buf[2] = i.x;
				//std::cout << "Wrote X: " << buf[2] << std::endl;
				break;
			case SETY:
				buf[2] = i.y;
				//std::cout << "Wrote Y: " << buf[2] << std::endl;
				break;
			}
			asio::write(*soc, asio::buffer(buf), ec);
		}
	}*/
	if ((*p).write) {
		//std::cout << "Writing: " << (*p).m << " Index: " << (*p).index << " At: " << soc->remote_endpoint() << std::endl;
		for (auto& i : sockets) {
				int buf[4];
				buf[0] = (*p).index;
				buf[1] = (*p).m;
				buf[3] = (*p).dir;
				switch ((*p).m) {
				case SETX:
					buf[2] = (*p).x;
					//std::cout << "Wrote X: " << buf[2] << std::endl;
					break;
				case SETY:
					buf[2] = (*p).y;
					//std::cout << "Wrote Y: " << buf[2] << std::endl;
					break;
				}
				asio::write(*i, asio::buffer(buf), ec);
				int readb[1];
				asio::read(*i, asio::buffer(readb), ec);
				if (readb[0] == RECIEVED) {
					std::cout << "Data recieved by socket" << std::endl;
				}

		}
	}
}
void sendNewDummy(int sx, int sy, int s, asio::ip::tcp::socket* sock) {
	std::cout << "Writing new peer to: " << sock->remote_endpoint() << std::endl;
	int buf[4];
	asio::error_code ec;
	buf[0] = sy;
	buf[1] = NEWDUMMY;
	buf[2] = sx;
	buf[3] = s;
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
			sendNewDummy(sx, sy, s, sockets[i]);
		}
		sockets.push_back(soc);
		Player p = { SETY, sx, sy, 1 };
		p.index = s;
		p.write = true;
		players.push_back(p);
		for (int i = 0; i < players.size(); i++) {
			sendNewDummy(players[i].x, players[i].y, players[i].index, soc);
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
			for (int i = 0; i < players.size(); i++) {
				readInput(&players[i], sockets[i]);
				sendChanges(&players[i], sockets[i]);
			}
			iosafe.unlock();
		}
	}
	listen.join();
	return 0;
}