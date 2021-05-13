#include "PixelEdit.h"
#include <vector>
#undef main
bool exitf = false;
SDL_Surface *surf;
SDL_Renderer *rend;
const Uint8 *keys;
float delta;
struct Entity {
	float x;
	float y;
	int w;
	int h;
	Uint8 dir;
	bool del;
	float dirtimer;
	bool chngdir;
};
std::vector<Entity> enemies;
Gore::Edit edit;
void loadEnemies();

//implement enemy ai

void death(Entity *p) {
	(*p).x = 400;
	(*p).y = 400;
	(*p).dir = 1;
	(*p).h = 10;
	(*p).w = 1;
	edit.clearSurface(surf, 800, 800);
	enemies.clear();
	loadEnemies();
}

//Player input/path drawing
void playerUpdate(Entity *p) {
	SDL_PumpEvents();
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
	switch ((*p).dir) {
	case 1:
		edit.setPixelRGBA(surf, (*p).x, (*p).y + (*p).h + 1, 0, 100, 255, 255);
		(*p).y -= (float)250 * delta;
		if (edit.getPixel(surf, (*p).x, (*p).y-1) != 0) {
			death(&(*p));
		}
		break;
	case 2:
		edit.setPixelRGBA(surf, (*p).x, (*p).y - 1, 0, 100, 255, 255);
		(*p).y += (float)250 * delta;
		if (edit.getPixel(surf, (*p).x, (*p).y+(*p).h+1) != 0) {
			death(&(*p));
		}
		break;
	case 3:
		edit.setPixelRGBA(surf, (*p).x+(*p).w+1, (*p).y, 0, 100, 255, 255);
		(*p).x -= (float)250 * delta;
		if (edit.getPixel(surf, (*p).x-1, (*p).y) != 0) {
			death(&(*p));
		}
		break;
	case 4:
		edit.setPixelRGBA(surf, (*p).x + 1, (*p).y, 0, 100, 255, 255);
		(*p).x += (float)250 * delta;
		if (edit.getPixel(surf, (*p).x+(*p).w+1, (*p).y) != 0) {
			death(&(*p));
		}
		break;
	
	}
	SDL_Rect pal = { (*p).x, (*p).y, (*p).w, (*p).h };
	SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
	SDL_RenderFillRect(rend, &pal);
}
//Enemy ai/path drawing
//Have delta timer to reduce its reaction time to blockages in front of it
void enemyUpdate(Entity *p, Entity player) {
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
	} else if ((*p).x >= 750) {
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
void loadEnemies() {
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

int main(int argc, char **argv) {
	//SDL initilization
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "SDL Init failed: " << SDL_GetError << std::endl;
	}
	SDL_Window *wind = SDL_CreateWindow("Tron", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(wind, -1, 0);
	surf = SDL_CreateRGBSurfaceWithFormat(0, 800, 800, 32, SDL_PIXELFORMAT_RGBA8888);
	keys = SDL_GetKeyboardState(NULL);
	Entity player = { 400, 400, 1, 10, 1 };
	SDL_Rect screenm;
	screenm.x = 0;
	screenm.y = 0;
	screenm.w = 800;
	screenm.h = 800;
	SDL_Event e;
	loadEnemies();
	//Main game code
	while (!exitf) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				exitf = true;
				break;
			}
		}
		delta = edit.getDelta();
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
		SDL_RenderClear(rend);
		playerUpdate(&player);
		int c = 0;
		for (auto& i : enemies) {
			enemyUpdate(&i, player);
			if (i.del) {
				enemies.erase(enemies.begin() + c);
			}
			c++;
		}
		SDL_Texture* sceen = SDL_CreateTextureFromSurface(rend, surf);
		SDL_RenderCopy(rend, sceen, NULL, &screenm);
		SDL_RenderPresent(rend);
		SDL_DestroyTexture(sceen);
	}

	return 0;
}