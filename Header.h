#pragma once
#include "PixelEdit.h"
#include "SDL_ttf.h"
#include <vector>
#include <algorithm>
#include <string.h>
#define ASIO_STANDALONE
#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_TYPE_TRAITS
#define ASIO_HAS_VARIADIC_TEMPLATES
#define ASIO_HAS_STD_FUNCTION
#define ASIO_HAS_STD_CHRONO
#define BOOST_ALL_NO_LIB
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <asio.hpp>
//#include <asio/ts/buffer.hpp>
//#include <asio/ts/internet.hpp>
//switch to visual studio 2019
extern const Uint8 *keys;
extern int p1score;
extern int p2score;

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


