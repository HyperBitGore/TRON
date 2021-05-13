#pragma once
#include <iostream>
#include <time.h>
#include "SDL.h"


namespace Gore {
	struct RGB {
		Uint8 g;
		Uint8 b;
		Uint8 r;
		Uint8 a;
	};
	class Edit {
	private:
		Uint64 LAST = 0;
		Uint64 NOW = SDL_GetPerformanceCounter();
	public:
		//Utility functions
		double getDelta() {
			double delta = 0;
			LAST = NOW;
			NOW = SDL_GetPerformanceCounter();
			delta = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
			delta = delta * 0.001;
			return delta;
		}
		//Set the pixel color of a surface
		//If you wanna use textures, just convert the surface to a texture
		void setPixelRGB(SDL_Surface* tex, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
			SDL_LockSurface(tex);
			Uint32* pixels = (Uint32*)tex->pixels;
			Uint32* pixel = pixels + (y*tex->w) + x;
			*pixel = SDL_MapRGB(tex->format, r, g, b);
			SDL_UnlockSurface(tex);
		}
		//Copies raw pixel color data into a pixel
		void setPixel(SDL_Surface* surf, int x, int y, Uint32 pixel) {
			SDL_LockSurface(surf);
			Uint32 *pixels = (Uint32*)surf->pixels;
			pixels[(y*surf->w) + x] = pixel;
			SDL_UnlockSurface(surf);
		}
		//Same as set pixel rgb but added alpha value
		void setPixelRGBA(SDL_Surface* surf, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
			SDL_LockSurface(surf);
			Uint32* pixels = (Uint32*)surf->pixels;
			Uint32* pixel = pixels + (y*surf->w) + x;
			*pixel = SDL_MapRGBA(surf->format, r, g, b, a);
			SDL_UnlockSurface(surf);
		}
		//Incase you are using something that can push the x or y value past the limit
		void setPixelRGBASafe(SDL_Surface* surf, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int mh, int mw) {
			if (y < 0 || y >  mh || x < 0 || x > mw) {
				return;
			}
			SDL_LockSurface(surf);
			Uint32* pixels = (Uint32*)surf->pixels;
			Uint32* pixel = pixels + (y*surf->w) + x;
			*pixel = SDL_MapRGBA(surf->format, r, g, b, a);
			SDL_UnlockSurface(surf);
		}
		//For rgb surfaces
		void setPixelRGBSafe(SDL_Surface* tex, int x, int y, Uint8 r, Uint8 g, Uint8 b, int mh, int mw) {
			if (y < 0 || y >  mh || x < 0 || x > mw) {
				return;
			}
			SDL_LockSurface(tex);
			Uint32* pixels = (Uint32*)tex->pixels;
			Uint32* pixel = pixels + (y*tex->w) + x;
			*pixel = SDL_MapRGB(tex->format, r, g, b);
			SDL_UnlockSurface(tex);
		}
		//Copies raw pixel color data into a pixel checking to make sure point is on screen
		void setPixelSafeReduced(SDL_Surface* surf, int x, int y, int w, int h, Uint32 pixel, Uint32* pixels) {
			if (y > 0 && y < h && x > 0 && x < w) {
				pixels[(y*w) + x] = pixel;
			}
			else {
				return;
			}
		}
		//Copies raw pixel color data into a pixel checking to make sure point is on screen
		void setPixelSafe(SDL_Surface* surf, int x, int y, Uint32 pixel) {
			int w = surf->w;
			int h = surf->h;
			if (y < 0 || y >  h || x < 0 || x > w) {
				return;
			}
			SDL_LockSurface(surf);
			Uint32 *pixels = (Uint32*)surf->pixels;
			pixels[(y*surf->w) + x] = pixel;
			SDL_UnlockSurface(surf);
		}
		//returns the pixel color of a pixel at a certain x and y
		Uint32 getPixel(SDL_Surface* surf, int x, int y) {
			SDL_LockSurface(surf);
			Uint32 *pixels = (Uint32 *)surf->pixels;
			SDL_UnlockSurface(surf);
			Uint32 pixel = pixels[(y * surf->w) + x];
			return pixel;
		}
		//Safe version
		Uint32 getPixelSafe(SDL_Surface* surf, int x, int y) {
			int w = surf->w;
			int h = surf->h;
			if (y < 0 || y > h || x < 0 || x > w) {
				return 0;
			}
			SDL_LockSurface(surf);
			Uint32 *pixels = (Uint32 *)surf->pixels;
			SDL_UnlockSurface(surf);
			Uint32 pixel = pixels[(y * surf->w) + x];
			return pixel;
		}
		//returns rgb value of pixel
		RGB getPixelRGB(SDL_Surface* surf, int x, int y) {
			SDL_LockSurface(surf);
			Uint32 *pixels = (Uint32 *)surf->pixels;
			Uint32 pixel = pixels[(y * surf->w) + x];
			RGB c;
			SDL_GetRGB(pixel, surf->format, &c.r, &c.g, &c.b);
			SDL_UnlockSurface(surf);
			return c;
		}
		RGB getPixelRGBSafe(SDL_Surface* surf, int x, int y) {
			RGB c = { 0, 0, 0, 0 };
			int w = surf->w;
			int h = surf->h;
			if (y < 0 || y > h || x < 0 || x > w) {
				return c;
			}
			SDL_LockSurface(surf);
			Uint32 *pixels = (Uint32 *)surf->pixels;
			Uint32 pixel = pixels[(y * surf->w) + x];
			SDL_GetRGB(pixel, surf->format, &c.r, &c.g, &c.b);
			SDL_UnlockSurface(surf);
			return c;
		}
		//returns rgba value of pixel
		RGB getPixelRGBA(SDL_Surface* surf, int x, int y) {
			SDL_LockSurface(surf);
			Uint32 *pixels = (Uint32 *)surf->pixels;
			Uint32 pixel = pixels[(y * surf->w) + x];
			RGB c;
			SDL_GetRGBA(pixel, surf->format, &c.r, &c.g, &c.b, &c.a);
			SDL_UnlockSurface(surf);
			return c;
		}
		//Functions to compare RGB structs
		bool compareRGB(RGB rgb1, RGB rgb2) {
			if (rgb1.r == rgb2.r) {
				if (rgb1.g == rgb2.g) {
					if (rgb1.b == rgb2.b) {
						return true;
					}
				}
			}
			return false;
		}
		bool compareRGBA(RGB rgb1, RGB rgb2) {
			if (rgb1.r == rgb2.r) {
				if (rgb1.g == rgb2.g) {
					if (rgb1.b == rgb2.b) {
						if (rgb1.a == rgb2.a) {
							return true;
						}
					}
				}
			}
			return false;
		}
		bool compareColor(Uint32 c1, Uint32 c2) {
			if (c1 == c2) {
				return true;
			}
			return false;
		}
		void clearSurface(SDL_Surface* surf, int w, int h) {
			SDL_LockSurface(surf);
			std::memset(surf->pixels, 0, (w * h)*(sizeof(surf->pixels)));
			SDL_UnlockSurface(surf);
		}
	};
}