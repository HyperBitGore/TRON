#pragma once
#include "Header.h"

//Don't update dummy x and y client side, get x and y changes from server
void dummyUpdate(Dummy *p, float delta, SDL_Renderer* rend) {
	switch ((*p).dir) {
	case 1:
		(*p).w = 1;
		(*p).h = 10;
		break;
	case 2:
		(*p).w = 1;
		(*p).h = 10;
		break;
	case 3:
		(*p).w = 10;
		(*p).h = 1;
		break;
	case 4:
		(*p).w = 10;
		(*p).h = 1;
		break;
	}
	std::cout << "X: " << (*p).x << " Y:" << (*p).y << std::endl;
	SDL_Rect pal = { (*p).x, (*p).y, (*p).w, (*p).h };
	SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
	SDL_RenderFillRect(rend, &pal);
}
//Player input/path drawing
void playerUpdateMultiplayer(Entity* p, std::vector<Entity>& enemies, SDL_Surface* surf, SDL_Renderer* rend, float delta, bool wasd = true) {
	Gore::Edit edit;
	SDL_PumpEvents();
	if (wasd) {
		if (keys[SDL_SCANCODE_W]) {
			(*p).dir = 1;
			(*p).w = 1;
			(*p).h = 10;
		}
		else if (keys[SDL_SCANCODE_S]) {
			(*p).dir = 2;
			(*p).w = 1;
			(*p).h = 10;
		}
		else if (keys[SDL_SCANCODE_A]) {
			(*p).dir = 3;
			(*p).w = 10;
			(*p).h = 1;
		}
		else if (keys[SDL_SCANCODE_D]) {
			(*p).dir = 4;
			(*p).w = 10;
			(*p).h = 1;
		}
	}
	else {
		if (keys[SDL_SCANCODE_UP]) {
			(*p).dir = 1;
			(*p).w = 1;
			(*p).h = 10;
		}
		else if (keys[SDL_SCANCODE_DOWN]) {
			(*p).dir = 2;
			(*p).w = 1;
			(*p).h = 10;
		}
		else if (keys[SDL_SCANCODE_LEFT]) {
			(*p).dir = 3;
			(*p).w = 10;
			(*p).h = 1;
		}
		else if (keys[SDL_SCANCODE_RIGHT]) {
			(*p).dir = 4;
			(*p).w = 10;
			(*p).h = 1;
		}
	}
	if ((*p).x < 1 || (*p).x > 799 || (*p).y < 1 || (*p).y > 799) {
		death(&(*p), enemies, surf, !wasd);
	}
	switch ((*p).dir) {
	case 1:
		(*p).y -= (float)250 * delta;
		for (int i = (*p).y; i < (*p).ly; i++) {
			//edit.setPixelRGBA(surf, (*p).x, i, 0, 100, 255, 255);
		}
		if (edit.getPixel(surf, (*p).x, (*p).y - 1) != 0) {
			death(&(*p), enemies, surf, !wasd);
		}
		break;
	case 2:
		(*p).y += (float)250 * delta;
		for (int i = (*p).ly; i < (*p).y; i++) {
			//edit.setPixelRGBA(surf, (*p).x, i, 0, 100, 255, 255);
		}
		if (edit.getPixel(surf, (*p).x, (*p).y + (*p).h + 1) != 0) {
			death(&(*p), enemies, surf, !wasd);
		}
		break;
	case 3:
		(*p).x -= (float)250 * delta;
		for (int i = (*p).x; i < (*p).lx; i++) {
			//edit.setPixelRGBA(surf, i, (*p).y, 0, 100, 255, 255);
		}
		if (edit.getPixel(surf, (*p).x - 1, (*p).y) != 0) {
			death(&(*p), enemies, surf, !wasd);
		}
		break;
	case 4:
		(*p).x += (float)250 * delta;
		for (int i = (*p).lx; i < (*p).x; i++) {
			//edit.setPixelRGBA(surf, i, (*p).y, 0, 100, 255, 255);
		}
		if (edit.getPixel(surf, (*p).x + (*p).w + 1, (*p).y) != 0) {
			death(&(*p), enemies, surf, !wasd);
		}
		break;

	}
	(*p).ly = (*p).y;
	(*p).lx = (*p).x;
}

//Player input/path drawing
void playerUpdate(Entity *p, std::vector<Entity>& enemies, SDL_Surface* surf, SDL_Renderer *rend, float delta, bool wasd = true) {
	Gore::Edit edit;
	SDL_PumpEvents();
	if (wasd) {
		if (keys[SDL_SCANCODE_W]) {
			(*p).dir = 1;
			(*p).w = 1;
			(*p).h = 10;
		}
		else if (keys[SDL_SCANCODE_S]) {
			(*p).dir = 2;
			(*p).w = 1;
			(*p).h = 10;
		}
		else if (keys[SDL_SCANCODE_A]) {
			(*p).dir = 3;
			(*p).w = 10;
			(*p).h = 1;
		}
		else if (keys[SDL_SCANCODE_D]) {
			(*p).dir = 4;
			(*p).w = 10;
			(*p).h = 1;
		}
	}
	else {
		if (keys[SDL_SCANCODE_UP]) {
			(*p).dir = 1;
			(*p).w = 1;
			(*p).h = 10;
		}
		else if (keys[SDL_SCANCODE_DOWN]) {
			(*p).dir = 2;
			(*p).w = 1;
			(*p).h = 10;
		}
		else if (keys[SDL_SCANCODE_LEFT]) {
			(*p).dir = 3;
			(*p).w = 10;
			(*p).h = 1;
		}
		else if (keys[SDL_SCANCODE_RIGHT]) {
			(*p).dir = 4;
			(*p).w = 10;
			(*p).h = 1;
		}
	}
	if ((*p).x < 1 || (*p).x > 799 || (*p).y < 1 || (*p).y > 799) {
		death(&(*p), enemies, surf, !wasd);
	}
	switch ((*p).dir) {
	case 1:
		(*p).y -= (float)250 * delta;
		for (int i = (*p).y; i < (*p).ly; i++) {
			edit.setPixelRGBA(surf, (*p).x, i, 0, 100, 255, 255);
		}
		if (edit.getPixel(surf, (*p).x, (*p).y - 1) != 0) {
			death(&(*p), enemies, surf, !wasd);
		}
		break;
	case 2:
		(*p).y += (float)250 * delta;
		for (int i = (*p).ly; i < (*p).y; i++) {
			edit.setPixelRGBA(surf, (*p).x, i, 0, 100, 255, 255);
		}
		if (edit.getPixel(surf, (*p).x, (*p).y + (*p).h + 1) != 0) {
			death(&(*p), enemies, surf, !wasd);
		}
		break;
	case 3:
		(*p).x -= (float)250 * delta;
		for (int i = (*p).x; i < (*p).lx; i++) {
			edit.setPixelRGBA(surf, i, (*p).y, 0, 100, 255, 255);
		}
		if (edit.getPixel(surf, (*p).x - 1, (*p).y) != 0) {
			death(&(*p), enemies, surf, !wasd);
		}
		break;
	case 4:
		(*p).x += (float)250 * delta;
		for (int i = (*p).lx; i < (*p).x; i++) {
			edit.setPixelRGBA(surf, i, (*p).y, 0, 100, 255, 255);
		}
		if (edit.getPixel(surf, (*p).x + (*p).w + 1, (*p).y) != 0) {
			death(&(*p), enemies, surf, !wasd);
		}
		break;

	}
	SDL_Rect pal = { (*p).x, (*p).y, (*p).w, (*p).h };
	SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
	SDL_RenderFillRect(rend, &pal);
	(*p).lx = (*p).x;
	(*p).ly = (*p).y;
}

//Enemy ai/path drawing
//Have delta timer to reduce its reaction time to blockages in front of it
void enemyUpdate(Entity *p, Entity player, SDL_Surface* surf, SDL_Renderer *rend, float delta) {
	Gore::Edit edit;
	if (player.x > (*p).x) {
		(*p).dir = 4;
		(*p).w = 10;
		(*p).h = 1;
	}
	else if (player.x < (*p).x) {
		(*p).dir = 2;
		(*p).w = 1;
		(*p).h = 10;
	}
	else if (player.x == (*p).x) {
		(*p).dir = 4;
		(*p).w = 10;
		(*p).h = 1;
	}
	(*p).dirtimer += delta;
	if ((*p).dirtimer >= 0.03f) {
		(*p).chngdir = true;
		(*p).dirtimer = 0;
	}
	if ((*p).y > 750) {
		if ((*p).x < 400) {
			(*p).dir = 4;
		}
		else if ((*p).x > 400) {
			(*p).dir = 3;
		}
		else {
			(*p).dir = 3;
		}
		(*p).w = 10;
		(*p).h = 1;
	}
	else if ((*p).y < 50) {
		if ((*p).x < 400) {
			(*p).dir = 4;
		}
		else if ((*p).x > 400) {
			(*p).dir = 3;
		}
		else {
			(*p).dir = 3;
		}
		(*p).w = 10;
		(*p).h = 1;
	}
	else if ((*p).x <= 50) {
		(*p).dir = 1;
		(*p).w = 1;
		(*p).h = 10;
	}
	else if ((*p).x >= 750) {
		(*p).dir = 1;
		(*p).w = 1;
		(*p).h = 10;
	}

	if ((*p).x < 1 || (*p).x > 799 || (*p).y < 1 || (*p).y > 799) {
		(*p).del = true;
		return;
	}
	dirc:
	switch ((*p).dir) {
	case 1:
		for (int i = 1; i < 11; i++) {
			if (edit.getPixelSafe(surf, (*p).x, (*p).y - i) != 0 && (*p).chngdir) {
				(*p).dir = 4;
				(*p).w = 10;
				(*p).h = 1;
				(*p).chngdir = false;
				//goto dirc;
			}
		}
		(*p).y -= (float)250 * delta;
		for (int i = (*p).y; i < (*p).ly; i++) {
			edit.setPixelRGBA(surf, (*p).x, i, 255, 100, 0, 255);
		}
		if (edit.getPixelSafe(surf, (*p).x, (*p).y - 1) != 0) {
			(*p).del = true;
		}
		break;
	case 2:
		for (int i = 1; i < 11; i++) {
			if (edit.getPixelSafe(surf, (*p).x, (*p).y + (*p).h + i) != 0 && (*p).chngdir) {
				(*p).dir = 3;
				(*p).w = 10;
				(*p).h = 1;
				(*p).chngdir = false;
				//goto dirc;
			}
		}
		(*p).y += (float)250 * delta;
		for (int i = (*p).ly; i < (*p).y; i++) {
			edit.setPixelRGBA(surf, (*p).x, i, 255, 100, 0, 255);
		}
		if (edit.getPixelSafe(surf, (*p).x, (*p).y + (*p).h + 1) != 0) {
			(*p).del = true;
		}
		break;
	case 3:
		for (int i = 1; i < 11; i++) {
			if (edit.getPixelSafe(surf, (*p).x - i, (*p).y) != 0 && (*p).chngdir) {
				(*p).dir = 1;
				(*p).w = 1;
				(*p).h = 10;
				(*p).chngdir = false;
				//goto dirc;
			}
		}
		(*p).x -= (float)250 * delta;
		for (int i = (*p).x; i < (*p).lx; i++) {
			edit.setPixelRGBA(surf, i, (*p).y, 255, 100, 0, 255);
		}
		if (edit.getPixelSafe(surf, (*p).x - 1, (*p).y) != 0) {
			(*p).del = true;
		}
		break;
	case 4:
		for (int i = 1; i < 11; i++) {
			if (edit.getPixelSafe(surf, (*p).x - i, (*p).y) != 0 && (*p).chngdir) {
				(*p).dir = 2;
				(*p).w = 1;
				(*p).h = 10;
				(*p).chngdir = false;
				//goto dirc;
			}
		}
		(*p).x += (float)250 * delta;
		for (int i = (*p).lx; i < (*p).x; i++) {
			edit.setPixelRGBA(surf, i, (*p).y, 255, 100, 0, 255);
		}
		if (edit.getPixelSafe(surf, (*p).x + (*p).w + 1, (*p).y) != 0) {
			(*p).del = true;
		}
		break;

	}
	SDL_Rect pal = { (*p).x, (*p).y, (*p).w, (*p).h };
	SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
	SDL_RenderFillRect(rend, &pal);
	(*p).lx = (*p).x;
	(*p).ly = (*p).y;
}