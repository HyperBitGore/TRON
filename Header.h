#pragma once
#include "PixelEdit.h"
#include "SDL_ttf.h"
#include <vector>
#include <algorithm>
#include <string.h>
#define ASIO_STANDALONE
#include <asio.hpp>
//#include <asio/ts/buffer.hpp>
//#include <asio/ts/internet.hpp>
//switch to visual studio 2019
extern const Uint8 *keys;
extern int p1score;
extern int p2score;
//Send this message and a sucessive packet which contains the number by
enum messages { SETDIR, NEWDUMMY, RECIEVED, CLEAR, SETCOORDS};
struct Dummy {
	int x;
	int y;
	int dir;
	int w;
	int h;
	int index;
	int lx;
	int ly;
};
extern std::vector<Dummy> dummies;

struct Entity {
	float x;
	float y;
	int w;
	int h;
	Uint8 dir;
	bool del;
	float dirtimer;
	bool chngdir;
	float resetx;
	float resety;
	float lx;
	float ly;
	int index;
	bool clear;
};
void loadEnemies(std::vector<Entity>& enemies) {
	Entity e1 = { 200, 100, 1, 10, 2 };
	e1.del = false;
	e1.dirtimer = 0.0f;
	e1.chngdir = false;
	e1.lx = e1.x;
	e1.ly = e1.y;
	enemies.push_back(e1);
	e1.x = 100;
	e1.y = 500;
	e1.lx = e1.x;
	e1.ly = e1.y;
	enemies.push_back(e1);
	e1.x = 600;
	e1.y = 50;
	e1.lx = e1.x;
	e1.ly = e1.y;
	enemies.push_back(e1);
	e1.x = 700;
	e1.y = 600;
	e1.lx = e1.x;
	e1.ly = e1.y;
	enemies.push_back(e1);
	e1.x = 60;
	e1.y = 300;
	e1.lx = e1.x;
	e1.ly = e1.y;
	enemies.push_back(e1);
}
void death(Entity *p, std::vector<Entity>& enemies, SDL_Surface* surf, bool p2) {
	Gore::Edit edit;
	(*p).x = (*p).resetx;
	(*p).y = (*p).resety;
	(*p).lx = (*p).resetx;
	(*p).ly = (*p).resety;
	(*p).dir = 1;
	(*p).h = 10;
	(*p).w = 1;
	edit.clearSurface(surf, 800, 800);
	enemies.clear();
	loadEnemies(enemies);
	if (!p2) {
		p2score++;
		std::cout << "Player 2 Win; New score: " << "Player 1: " << p1score << " Player 2: " << p2score << std::endl;
	}
	else {
		p1score++;
		std::cout << "Player 1 Win; New score: " << "Player 1: " << p1score << " Player 2: " << p2score << std::endl;
	}

}
void multiDeath(Entity* p, SDL_Surface* surf, asio::ip::tcp::socket* sock) {
	Gore::Edit edit;
	edit.clearSurface(surf, 800, 800);
	//(*p).clear = true;
	(*p).x = (*p).resetx;
	(*p).y = (*p).resety;
	int send[5];
	send[0] = SETCOORDS;
	send[1] = (*p).x;
	send[2] = (*p).index;
	send[3] = (*p).dir;
	send[4] = (*p).y;
	asio::error_code ignore;
	asio::write(*sock, asio::buffer(send), ignore);
	std::cout << "Multiplayer death called" << std::endl;
}

size_t readPass(asio::ip::tcp::socket* sock, Entity* p) {
	int buf[5];
	asio::error_code ecode;
	size_t bytes = (*sock).available();
	if (bytes > 0) {
		asio::read(*sock, asio::buffer(buf), ecode);
		Dummy d;
		asio::error_code ec;
		//std::cout << "Reading " << bytes << " bytes for dummy: " << buf[0] << ":" << buf[2] << "Dir:" << buf[3] << std::endl;
		switch (buf[1]) {
		case NEWDUMMY:
			d.y = buf[4];
			d.x = buf[2];
			d.w = 1;
			d.h = 10;
			d.dir = buf[3];
			d.index = buf[0];
			if (d.index == (*p).index) {
				(*p).resetx = d.x;
				(*p).resety = d.y;
				(*p).y = buf[4];
				(*p).x = buf[2];
				(*p).dir = buf[3];
			}
			d.lx = d.x;
			d.ly = d.y;
			std::cout << "Inserting new dummmy at " << d.index << " X: " << buf[2] << " Y: " << buf[4] << std::endl;
			dummies.insert(dummies.begin() + d.index, d);
			break;
		case SETCOORDS:
			dummies[buf[0]].x = buf[2];
			dummies[buf[0]].y = buf[4];
			dummies[buf[0]].dir = buf[3];
			std::cout << "SETCOORDS Recieved" << " X:" << buf[2] << " Y:" << buf[4] << std::endl;
			break;
		}
		if (ecode == asio::error::eof) {
			return 0;
		}
		else if (ecode) {
			return 0;
		}
	}
	return bytes;
}

