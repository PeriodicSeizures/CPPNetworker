#include <assert.h>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#ifndef _WIN32
#include <signal.h>
#endif

#include "Engine.h"

static SDL_Window* sdl_window;
static SDL_Renderer* sdl_renderer;

//static SDL_Texture* sdl_texture; // send to gpu

static SDL_Texture* font;



void drawChar(char c, int x, int y, const SDL_Color &color, uint8_t size) {
    if (c >= 32 && c <= 126) {
        int offset = (int)(c - 32) * 7;

        //const int startingScreenX = x + (int)(i * fonts[0].w * size) + centerXOffset;
        //const int startingScreenY = y + centerYOffset;
        //
        //int currentScreenX = startingScreenX;
        //int currentScreenY = startingScreenY;

        SDL_SetTextureColorMod(font, color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(font, color.a);

        SDL_Rect srcrect = { offset, 0, 7, 11 };
        SDL_Rect dstrect = { x, y, 7*size, 11*size };

        SDL_RenderCopy(sdl_renderer, font, &srcrect, &dstrect);
    }
}

namespace Engine {
    SDL_Texture* loadTexture(std::string path)
    {
        //The final texture
        SDL_Texture* newTexture = NULL;

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == NULL)
        {
            printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        }
        else
        {
            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface(sdl_renderer, loadedSurface);
            if (newTexture == NULL)
            {
                printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
            }

            //Get rid of old loaded surface
            SDL_FreeSurface(loadedSurface);
        }

        return newTexture;
    }

	void init() {
        SDL_CreateWindowAndRenderer(800, 600, 
            SDL_WINDOW_SHOWN 
            | SDL_RENDERER_PRESENTVSYNC
            | SDL_RENDERER_ACCELERATED, 
            &sdl_window, &sdl_renderer);

        //sdl_texture = SDL_CreateTexture(sdl_renderer,
        //    SDL_PIXELFORMAT_ARGB8888,
        //    SDL_TEXTUREACCESS_STREAMING,
        //    800, 600);

        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        }

        SDL_SetWindowTitle(sdl_window, "Client");

        
        font = loadTexture("resources/font7x11.png");

#ifndef _WIN32
        signal(SIGINT, SIG_DFL);
#endif
        assert(sdl_renderer);
        assert(sdl_window);
        //assert(sdl_texture);
	}

    void uninit() {
        //Free loaded image
        SDL_DestroyTexture(font);

        //Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }

    void doRender() {
        //SDL_UpdateTexture(sdl_texture, NULL, screen, SCREEN_WIDTH * sizeof(COLOR));
        //SDL_RenderClear(sdl_renderer);
        //SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, NULL);
        SDL_RenderPresent(sdl_renderer);
    }

    void drawTexture(SDL_Texture* texture, SDL_Rect &src_rect, SDL_Rect &rect) {
        SDL_RenderCopy(sdl_renderer, texture, &src_rect, &rect);
    }

    static SDL_Color colors[] = {
        {0x00, 0x00, 0x00, 0xFF},
        {0x00, 0x00, 0xAA, 0xFF},
        {0x00, 0xAA, 0x00, 0xFF},
        {0x00, 0xAA, 0xAA, 0xFF},
        {0xAA, 0x00, 0x00, 0xFF},
        {0xAA, 0x00, 0xAA, 0xFF},
        {0xFF, 0xAA, 0x00, 0xFF},
        {0xAA, 0xAA, 0xAA, 0xFF},
        {0x55, 0x55, 0x55, 0xFF},
        {0x55, 0x55, 0xFF, 0xFF},

        {0x55, 0xFF, 0x55, 0xFF},
        {0x55, 0xFF, 0xFF, 0xFF},
        {0xFF, 0x55, 0x55, 0xFF},
        {0xFF, 0x55, 0xFF, 0xFF},
        {0xFF, 0xFF, 0x55, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF}
    };

    void drawFormattedString(const std::string& str, int x, int y, uint8_t alpha, int size, bool centered) {

        //SDL_SetTextureColorMod(font, colors[)

        const int w = 7 * size * str.length(),
            h = 11 * size;

        if (centered) {
            x -= w / 2;
            y -= h / 2;
        }

        //int copy_x = x;

        SDL_Color color = { 255, 255, 255, alpha };

        unsigned int i = 0;
        unsigned int draw_i = 0;
        while (i < str.length()) {
            //for (uint16_t i = 0; i < str.length(); i++) {

            //char &c = str[i];

            while (str[i] == '\n') {
                y += 9 * size;//  7 * 
                //copy_x = x;
                draw_i = 0;
                i++;
            }

            // detect a color code &ccc...
            if (str[i] == '&') {
                const char c1 = str[i + 1];

                if (c1 >= '0' && c1 <= '9') {
                    color = colors[c1 - '0'];
                    i += 2;
                }
                else if (c1 >= 'a' && c1 <= 'f') {
                    color = colors[c1 - 'a' + 10];
                    i += 2;
                }
            }

            drawChar(str[i], x + draw_i * 7 * size, y, color, size);
            draw_i++;
            i++;
        }

    }

    void drawString(const std::string &str, int x, int y, const SDL_Color &color, int size, bool centered) {
        
        //SDL_SetTextureColorMod(font, colors[)

        const int w = 7 * size * str.length(),
                  h = 11 * size;
        
        if (centered) {
            x -= w / 2;
            y -= h / 2;
        }

        unsigned int i = 0;
        while (i < str.length()) {
        //for (uint16_t i = 0; i < str.length(); i++) {

            //while (str[i] == '\n') {
            //    x += 7;
            //    i++;
            //}

            drawChar(str[i], x + i * 7 * size, y, color, size);
            i++;
        }
        
    }

    void fill(const SDL_Color &color) {
        SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(sdl_renderer, NULL);
    }

    void fillRect(const SDL_Color& color, const SDL_Rect& rect) {
        SDL_RenderFillRect(sdl_renderer, &rect);
    }

}