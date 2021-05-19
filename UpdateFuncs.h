#pragma once
#include "Header.h"


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
		edit.setPixelRGBA(surf, (*p).x, (*p).y + (*p).h + 1, 0, 100, 255, 255);
		(*p).y -= (float)250 * delta;
		if (edit.getPixel(surf, (*p).x, (*p).y - 1) != 0) {
			death(&(*p), enemies, surf, !wasd);
		}
		break;
	case 2:
		edit.setPixelRGBA(surf, (*p).x, (*p).y - 1, 0, 100, 255, 255);
		(*p).y += (float)250 * delta;
		if (edit.getPixel(surf, (*p).x, (*p).y + (*p).h + 1) != 0) {
			death(&(*p), enemies, surf, !wasd);
		}
		break;
	case 3:
		edit.setPixelRGBA(surf, (*p).x + (*p).w + 1, (*p).y, 0, 100, 255, 255);
		(*p).x -= (float)250 * delta;
		if (edit.getPixel(surf, (*p).x - 1, (*p).y) != 0) {
			death(&(*p), enemies, surf, !wasd);
		}
		break;
	case 4:
		edit.setPixelRGBA(surf, (*p).x + 1, (*p).y, 0, 100, 255, 255);
		(*p).x += (float)250 * delta;
		if (edit.getPixel(surf, (*p).x + (*p).w + 1, (*p).y) != 0) {
			death(&(*p), enemies, surf, !wasd);
		}
		break;

	}
	SDL_Rect pal = { (*p).x, (*p).y, (*p).w, (*p).h };
	SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
	SDL_RenderFillRect(rend, &pal);
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
	(*p).dirtimer += delta;
	if ((*p).dirtimer >= 0.08f) {
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


	switch ((*p).dir) {
	case 1:
		edit.setPixelRGBASafe(surf, (*p).x, (*p).y + (*p).h + 1, 255, 100, 0, 255, 800, 800);;
		(*p).y -= (float)250 * delta;

		if (edit.getPixelSafe(surf, (*p).x, (*p).y - 10) != 0 && (*p).chngdir) {
			(*p).dir = 4;
			(*p).w = 10;
			(*p).h = 1;
			(*p).chngdir = false;

		}
		if (edit.getPixelSafe(surf, (*p).x, (*p).y - 1) != 0) {
			(*p).del = true;
		}
		break;
	case 2:
		edit.setPixelRGBASafe(surf, (*p).x, (*p).y - 1, 255, 100, 0, 255, 800, 800);
		(*p).y += (float)250 * delta;
		if (edit.getPixelSafe(surf, (*p).x, (*p).y + (*p).h + 10) != 0 && (*p).chngdir) {
			(*p).dir = 3;
			(*p).w = 10;
			(*p).h = 1;
			(*p).chngdir = false;

		}
		if (edit.getPixelSafe(surf, (*p).x, (*p).y + (*p).h + 1) != 0) {
			(*p).del = true;
		}
		break;
	case 3:
		edit.setPixelRGBASafe(surf, (*p).x + (*p).w + 1, (*p).y, 255, 100, 0, 255, 800, 800);
		(*p).x -= (float)250 * delta;
		if (edit.getPixelSafe(surf, (*p).x - 10, (*p).y) != 0 && (*p).chngdir) {
			(*p).dir = 1;
			(*p).w = 1;
			(*p).h = 10;
			(*p).chngdir = false;

		}

		if (edit.getPixelSafe(surf, (*p).x - 1, (*p).y) != 0) {
			(*p).del = true;
		}
		break;
	case 4:
		edit.setPixelRGBASafe(surf, (*p).x + 1, (*p).y, 255, 100, 0, 255, 800, 800);
		(*p).x += (float)250 * delta;
		if (edit.getPixelSafe(surf, (*p).x - 10, (*p).y) != 0 && (*p).chngdir) {
			(*p).dir = 2;
			(*p).w = 1;
			(*p).h = 10;
			(*p).chngdir = false;

		}
		if (edit.getPixelSafe(surf, (*p).x + (*p).w + 1, (*p).y) != 0) {
			(*p).del = true;
		}
		break;

	}
	SDL_Rect pal = { (*p).x, (*p).y, (*p).w, (*p).h };
	SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
	SDL_RenderFillRect(rend, &pal);
}