#include <iostream>
#define ASIO_STANDALONE
#include <asio.hpp>
#include <vector>
#include <thread>
#include <mutex>
//Maybe just use clear again and just write to all sockets clear
enum messages { NEWDUMMY, RECIEVED, SETCOORDS, CLEAR};
struct Player {
	messages m;
	int x;
	int y;
	int dir;
	int index;
	bool write;
	int sx; 
	int sy;
	int writetime;
	bool del;
};
std::mutex iosafe;
std::vector<asio::ip::tcp::socket*> sockets;
std::vector<Player> players;
bool exitf = false;
bool resett = false;
void sendReset();
asio::io_context io;
void readInput(Player *p, asio::ip::tcp::socket* soc) {
	int buf[5];
	size_t bytes = (*soc).available();
	(*p).write = false;
	asio::error_code ecode;
	(*p).writetime++;
	if ((*p).writetime > 200000) {
		std::cout << "Terminating socket connection at " << soc->remote_endpoint() << std::endl;
		(*p).del = true;
	}
	if (bytes > 0) {
		asio::read(*soc, asio::buffer(buf), ecode);
		switch (buf[0]) {
		case SETCOORDS:
			(*p).x = buf[1];
			(*p).y = buf[4];
			(*p).m = SETCOORDS;
			(*p).writetime = 0;
			(*p).dir = buf[3];
			break;
		case CLEAR:
			(*p).x = buf[1];
			(*p).y = buf[4];
			(*p).m = CLEAR;
			(*p).writetime = 0;
			(*p).dir = buf[3];
			resett = true;
			break;
		}
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
void sendChanges(asio::ip::tcp::socket* soc) {
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
				//std::cout << "Sending set coords X: " << i.x << " Y:" << i.y << std::endl;
				break;
			case CLEAR:
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
	for (auto& i : players) {
		i.x = i.sx;
		i.y = i.sy;
		i.m = CLEAR;
		i.dir = 1;
	}
	std::cout << "Sent clear call" << std::endl;
}
void sendNewDummy(int sx, int sy, int s, int sdir, asio::ip::tcp::socket* sock) {
	std::cout << "Writing new peer to: " << sock->remote_endpoint() << std::endl;
	int buf[5];
	asio::error_code ec;
	buf[0] = s;
	buf[1] = NEWDUMMY;
	buf[2] = sx;
	buf[3] = sdir;
	buf[4] = sy;
	asio::write(*sock, asio::buffer(buf), ec);
}

void listeningThread() {
	int sx = 20;
	int sy = 400;
	while (!exitf) {
		asio::ip::tcp::acceptor accept(io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13));
		asio::ip::tcp::socket* soc = new asio::ip::tcp::socket(io);
		asio::error_code ec;
		accept.listen();
		accept.accept(*soc, ec);
		iosafe.lock();
		int buf[1];
		buf[0] = sockets.size();
		asio::write(*soc, asio::buffer(buf), ec);
		for (int i = 0; i < sockets.size(); i++) {
			sendNewDummy(sx, sy, sockets.size(), 1, sockets[i]);
		}
		sockets.push_back(soc);
		Player p = { SETCOORDS, sx, sy, 1 };
		p.index = players.size();
		p.write = true;
		p.sx = p.x;
		p.sy = p.y;
		p.writetime = 0;
		p.del = false;
		players.push_back(p);
		for (auto& i : players) {
			sendNewDummy(i.x, i.y, i.index, 1, soc);
		}
		sx += 10;
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
				if (resett) {
					sendReset();
					resett = false;
				}
				sendChanges(sockets[i]);
				if (players[i].del) {
					std::cout << "Deleting socket connection at " << sockets[i]->remote_endpoint() << std::endl;
					players.erase(players.begin() + i);
					sockets[i]->close();
					delete sockets[i];
					sockets.erase(sockets.begin() + i);
				}
			}
			iosafe.unlock();
		}
	}
	listen.join();
	return 0;
}