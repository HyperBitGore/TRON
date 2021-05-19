#pragma once
#include "PixelEdit.h"
#include "SDL_ttf.h"
#include <vector>
//#define ASIO_STANDALONE
//#include <asio.hpp>
//#include <asio/ts/buffer.hpp>
//#include <asio/ts/internet.hpp>

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
};
void loadEnemies(std::vector<Entity>& enemies) {
	Entity e1 = { 200, 100, 1, 10, 2 };
	e1.del = false;
	e1.dirtimer = 0.0f;
	e1.chngdir = false;
	enemies.push_back(e1);
	e1.x = 100;
	e1.y = 500;
	enemies.push_back(e1);
	e1.x = 600;
	e1.y = 50;
	enemies.push_back(e1);
	e1.x = 700;
	e1.y = 600;
	enemies.push_back(e1);
	e1.x = 60;
	e1.y = 300;
	enemies.push_back(e1);
}
void death(Entity *p, std::vector<Entity>& enemies, SDL_Surface* surf, bool p2) {
	Gore::Edit edit;
	(*p).x = (*p).resetx;
	(*p).y = (*p).resety;
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


