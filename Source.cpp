#include "UpdateFuncs.h"
#undef main
bool exitf = false;
const Uint8 *keys;
//Improve enemy ai and add 1v1 mode with two players
int main(int argc, char **argv) {
	//SDL initilization
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "SDL Init failed: " << SDL_GetError << std::endl;
	}
	float delta;
	std::vector<Entity> enemies;
	Gore::Edit edit;
	SDL_Window *wind = SDL_CreateWindow("Tron", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
	SDL_Renderer *rend = SDL_CreateRenderer(wind, -1, 0);
	SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormat(0, 800, 800, 32, SDL_PIXELFORMAT_RGBA8888);
	keys = SDL_GetKeyboardState(NULL);
	Entity player = { 400, 400, 1, 10, 1 };
	SDL_Rect screenm;
	screenm.x = 0;
	screenm.y = 0;
	screenm.w = 800;
	screenm.h = 800;
	SDL_Event e;
	loadEnemies(enemies);
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
		playerUpdate(&player, enemies, surf, rend, delta);
		int c = 0;
		for (auto& i : enemies) {
			enemyUpdate(&i, player, surf, rend, delta);
			if (i.del) {
				enemies.erase(enemies.begin() + c);
				c--;
			}
			c++;
		}
		if (enemies.size() <= 0) {
			death(&player, enemies, surf);
		}
		SDL_Texture* sceen = SDL_CreateTextureFromSurface(rend, surf);
		SDL_RenderCopy(rend, sceen, NULL, &screenm);
		SDL_RenderPresent(rend);
		SDL_DestroyTexture(sceen);
	}

	return 0;
}