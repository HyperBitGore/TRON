#include "UpdateFuncs.h"
#undef main
bool exitf = false;
const Uint8 *keys;
int p1score = 0;
int p2score = 0;
asio::io_context aio;
std::vector<Dummy> dummies;

bool greaterDummyIndex(Dummy d1, Dummy d2) {
	return (d1.index > d2.index);
}

//Have vector of dummy players that only have draw rect in their function
void onlineMode(asio::ip::tcp::socket *sock, Entity *p, float delta, SDL_Renderer* rend) {
	for (int i = 0; i < dummies.size(); i++) {
		dummyUpdate(&dummies[i], delta, rend);
		//std::cout << dummies[i].index << " i: " << i << std::endl;
	}
	//Need server to be running or the connection will throw esoteric exception unless catch error code like so
	//Use points to store needed changes and then send said changes to server and clear the points vector
	//Have an online player update function that communicates with this function
	//Send what the packet is and have the packet contain the data of the coord
	messages m = SETX;
	int sendn = 0;
	if ((*p).dir > 2) {
		sendn = (int)(*p).x;
		m = SETX;
	}
	else {
		sendn = (int)(*p).y;
		m = SETY;
	}
	int send[4] = { m, sendn, (*p).index, (*p).dir};
	asio::error_code ignore;
	asio::write(*sock, asio::buffer(send), ignore);
	if ((*sock).available() > 16) {
		for (int i = 0; i < (*sock).available() / 16; i++) {
			readPass(sock, p);
		}
	}
	readPass(sock, p);

}
void startOnlineMode(asio::ip::tcp::socket *sock, Entity *p) {
	asio::error_code eg;
	std::string ipstring;
	std::cout << "Input IP address to connect to";
	std::cin >> ipstring;
	asio::ip::tcp::endpoint end(asio::ip::address::from_string(ipstring), 13);
	asio::ip::tcp::resolver resolv(aio);
	asio::ip::tcp::resolver::results_type endpoints = resolv.resolve(end);
	asio::error_code ec;
	(*sock).connect(*endpoints, ec);
	
	if (ec) {
		std::cout << "Connection error: " << ec.message() << std::endl;
		return;
	}
}

//Finish multiplayer mode
int main(int argc, char **argv) {
	//SDL initilization
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "SDL Init failed: " << SDL_GetError << std::endl;
	}
	if (!TTF_Init()) {
		std::cout << "Font init failed" << TTF_GetError << std::endl;
	}
	float delta;
	std::vector<Entity> enemies;
	Gore::Edit edit;
	SDL_Window *wind = SDL_CreateWindow("Tron", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
	SDL_Renderer *rend = SDL_CreateRenderer(wind, -1, 0);
	SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormat(0, 800, 800, 32, SDL_PIXELFORMAT_RGBA8888);
	keys = SDL_GetKeyboardState(NULL);
	TTF_Font* font = TTF_OpenFont("DelaGothicOne-Regular.ttf", 12);
	Entity player = { 400, 400, 1, 10, 1 };
	player.resetx = 400;
	player.resety = 400;
	player.lx = player.x;
	player.ly = player.y;
	Entity p2 = { 300, 400, 1, 10, 1 };
	p2.lx = p2.x;
	p2.ly = p2.y;
	p2.resetx = 300;
	p2.resety = 400;
	SDL_Rect screenm;
	screenm.x = 0;
	screenm.y = 0;
	screenm.w = 800;
	screenm.h = 800;
	SDL_Color white = { 255, 255, 255 };
	SDL_Event eventi;
	SDL_Surface* text = TTF_RenderText_Solid(font, "Normal Mode", white);
	SDL_Texture* rtext = SDL_CreateTextureFromSurface(rend, text);
	text = TTF_RenderText_Solid(font, "Two Player", white);
	SDL_Texture* rtext2 = SDL_CreateTextureFromSurface(rend, text);
	text = TTF_RenderText_Solid(font, "Online", white);
	SDL_Texture* rtext3 = SDL_CreateTextureFromSurface(rend, text);
	text = TTF_RenderText_Solid(font, "Exit", white);
	SDL_Texture* rtext4 = SDL_CreateTextureFromSurface(rend, text);
	text = TTF_RenderText_Solid(font, "Online arrows", white);
	SDL_Texture* rtext5 = SDL_CreateTextureFromSurface(rend, text);
	int renmode = 2;
	bool owasd = true;
	asio::ip::tcp::socket sock(aio);
	loadEnemies(enemies);
	//Main game code
	while (!exitf) {
		while (SDL_PollEvent(&eventi)) {
			switch (eventi.type) {
			case SDL_QUIT:
				exitf = true;
				break;
			}
		}
		delta = edit.getDelta();
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
		SDL_RenderClear(rend);
		SDL_Texture* sceen = SDL_CreateTextureFromSurface(rend, surf);
		SDL_Rect button;
		button.x = 100;
		button.y = 150;
		button.w = 100;
		button.h = 50;
		int mx;
		int my;
		int c = 0;
		switch (renmode) {
		case 1:
			playerUpdate(&player, enemies, surf, rend, delta);
			c = -1;
			for (auto& i : enemies) {
				c++;
				enemyUpdate(&i, player, surf, rend, delta);
				if (i.del) {
					enemies.erase(enemies.begin() + c);
					c--;
				}
			}
			if (enemies.size() <= 0) {
				death(&player, enemies, surf, true);
			}
			SDL_RenderCopy(rend, sceen, NULL, &screenm);
			SDL_RenderPresent(rend);
			break;
		case 2:
			//Normal, Two player, online, and exit button
			button.y = 151;
			SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
			SDL_RenderFillRect(rend, &button);
			SDL_RenderCopy(rend, rtext, NULL, &button);
			button.y = 201;
			SDL_RenderFillRect(rend, &button);
			SDL_RenderCopy(rend, rtext2, NULL, &button);
			button.y = 251;
			SDL_RenderFillRect(rend, &button);
			SDL_RenderCopy(rend, rtext3, NULL, &button);
			button.y = 301;
			SDL_RenderFillRect(rend, &button);
			SDL_RenderCopy(rend, rtext4, NULL, &button);
			button.y = 351;
			SDL_RenderFillRect(rend, &button);
			SDL_RenderCopy(rend, rtext5, NULL, &button);
			if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
				if (mx >= 100 && mx <= 200) {
					if (my >= 151 && my <= 201) {
						renmode = 1;
					}
					else if (my >= 201 && my < 251) {
						renmode = 3;
					}
					else if (my >= 251 && my < 301) {
						renmode = 4;
						startOnlineMode(&sock, &player);
						owasd = true;
					}
					else if (my >= 301 && my < 350) {
						exitf = true;
					}
					else if (my >= 351 && my < 400) {
						renmode = 4;
						startOnlineMode(&sock, &player);
						owasd = false;
					}
				}
			}
			SDL_RenderPresent(rend);
			break;
		case 3:
			//Two player mode
			playerUpdate(&player, enemies, surf, rend, delta);
			playerUpdate(&p2, enemies, surf, rend, delta, false);
			SDL_RenderCopy(rend, sceen, NULL, &screenm);
			SDL_RenderPresent(rend);
			break;
		case 4:
			playerUpdateMultiplayer(&player, &sock,enemies, surf, rend, delta, owasd);
			onlineMode(&sock, &player, delta, rend);

			SDL_RenderCopy(rend, sceen, NULL, &screenm);
			SDL_RenderPresent(rend);
			break;
		}
		SDL_DestroyTexture(sceen);
	}
	SDL_DestroyTexture(rtext);
	SDL_DestroyTexture(rtext2);
	SDL_DestroyTexture(rtext3);
	SDL_DestroyTexture(rtext4);
	SDL_FreeSurface(surf);
	SDL_DestroyRenderer(rend);
	SDL_FreeSurface(text);
	return 0;
}