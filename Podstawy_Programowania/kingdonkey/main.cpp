#define _USE_MATH_DEFINES
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define BITY 8
#define T_LENGTH 128
#define TICK 0.001
#define M_MARGIN 10
#define T_MARGIN 30
#define M_WIDTH SCREEN_WIDTH - 2*M_MARGIN
#define M_HEIGHT 35
#define M_T1 15
#define M_T2 30
#define M_T3 50
#define M_T4 65
#define M_T5 80
#define M_T6 100
#define PLATFORMS1 2
#define PLATFORMS2 4
#define PLATFORMS3 6
#define P_MARGIN 70
#define P_WIDTH SCREEN_WIDTH - P_MARGIN
#define P_HEIGHT 20
#define P_SPACE 60
#define L_MARGIN P_MARGIN + 20
#define L_WIDTH 20
#define L_S_HEIGHT 5
#define B_MARGIN 80
#define B_HEIGHT 17
#define B_WIDTH 12
#define E_HEIGHT 10
#define E_WIDTH 10
#define GRAVITATION 300
#define SPEEDX 100
#define SPEEDY 50
#define B_SPEEDX 100
#define J_SPEEDY -150
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4
#define H_SPACE 35
#define LIFE 3

struct SDL
{
	SDL_Event event;
	SDL_Surface* screen, * charset;
	SDL_Surface* player_l;
	SDL_Surface* player_r;
	SDL_Surface* barrel;
	SDL_Surface* heart;
	SDL_Surface* enemy;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
};

struct Colors
{
	int czarny;
	int zielony;
	int czerwony;
	int niebieski;
};

struct Controls
{
	int quit;
	int nowa;
	int menu;
	int t;
	char text[T_LENGTH];
	int points;
	int level;
	int life;
	int platforms;
	double worldTime;
	double delta;
};

struct Player
{
	double positionX;
	double positionY;
	int direction;
	int jump;
	int j_direction;
	double j_velocity;
	int a_direction;
};

struct Barrel
{
	double positionX;
	double positionY;
	int direction;
	int colision;
	double velocity;
};

// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj¹ca znaki
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt œrodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
	};


// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
	};


// rysowanie linii o d³ugoœci l w pionie (gdy dx = 0, dy = 1) 
// b¹dŸ poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
		};
	};


// rysowanie prostok¹ta o d³ugoœci boków l i k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};


// main
#ifdef __cplusplus
extern "C"
#endif

void CreateColors(struct SDL* sdl, struct Colors* c)
{
	(*c).czarny = SDL_MapRGB((*sdl).screen->format, 0x00, 0x00, 0x00);
	(*c).zielony = SDL_MapRGB((*sdl).screen->format, 0x00, 0xFF, 0x00);
	(*c).czerwony = SDL_MapRGB((*sdl).screen->format, 0xFF, 0x00, 0x00);
	(*c).niebieski = SDL_MapRGB((*sdl).screen->format, 0x11, 0x11, 0xCC);
}

void DestroySDL(struct SDL* sdl)
{
	if ((*sdl).charset != NULL)
	{
		SDL_FreeSurface((*sdl).charset);
	}
	if ((*sdl).screen != NULL)
	{
		SDL_FreeSurface((*sdl).screen);
	}
	if ((*sdl).scrtex != NULL)
	{
		SDL_DestroyTexture((*sdl).scrtex);
	}
	if ((*sdl).window != NULL)
	{
		SDL_DestroyWindow((*sdl).window);
	}
	if ((*sdl).renderer != NULL)
	{
		SDL_DestroyRenderer((*sdl).renderer);
	}
	SDL_Quit();
}

void LoadBMP(struct SDL* sdl)
{
	// wczytanie obrazka cs8x8.bmp
	(*sdl).charset = SDL_LoadBMP("./cs8x8.bmp");
	if ((*sdl).charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		DestroySDL(sdl);
		exit(0);
	};
	SDL_SetColorKey((*sdl).charset, true, 0x000000);

	(*sdl).player_l = SDL_LoadBMP("./donkey_l.bmp");
	if ((*sdl)
		.player_l == NULL) {
		printf("SDL_LoadBMP(donkey_l.bmp) error: %s\n", SDL_GetError());
		DestroySDL(sdl);
		exit(0);
	};

	(*sdl).player_r = SDL_LoadBMP("./donkey_r.bmp");
	if ((*sdl)
		.player_r == NULL) {
		printf("SDL_LoadBMP(donkey_r.bmp) error: %s\n", SDL_GetError());
		DestroySDL(sdl);
		exit(0);
	};

	(*sdl).enemy = SDL_LoadBMP("./king.bmp");
	if ((*sdl)
		.enemy == NULL) {
		printf("SDL_LoadBMP(enemy.bmp) error: %s\n", SDL_GetError());
		DestroySDL(sdl);
		exit(0);
	};

	(*sdl).barrel = SDL_LoadBMP("./barrel.bmp");
	if ((*sdl)
		.barrel == NULL) {
		printf("SDL_LoadBMP(barrel.bmp) error: %s\n", SDL_GetError());
		DestroySDL(sdl);
		exit(0);
	};

	(*sdl).heart = SDL_LoadBMP("./heart.bmp");
	if ((*sdl)
		.heart == NULL) {
		printf("SDL_LoadBMP(barrel.bmp) error: %s\n", SDL_GetError());
		DestroySDL(sdl);
		exit(0);
	};

}

void CreateWindow(struct SDL *sdl)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		exit(0);
	}
	// tryb pe³noekranowy / fullscreen mode
	//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
	//	                                 &window, &renderer);
	int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &(*sdl).window, &(*sdl).renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		exit(0);
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize((*sdl).renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor((*sdl).renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle((*sdl).window, "King Donkey by Kacper Petla 198186");

	(*sdl).screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	(*sdl).scrtex = SDL_CreateTexture((*sdl).renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);
}

void DrawMenu(struct SDL* sdl, struct Colors* c, struct Controls* s)
{
	char text[T_LENGTH];

	SDL_FillRect((*sdl).screen, NULL, (*c).niebieski);

	sprintf(text, "KING DONKEY by Kacper Petla 198186");
	DrawString((*sdl).screen, (*sdl).screen->w - strlen(text) * BITY - T_MARGIN, M_T1, text, (*sdl).charset);

	sprintf(text, "MENU:");
	DrawString((*sdl).screen, (*sdl).screen->w - SCREEN_WIDTH + T_MARGIN, M_T2, text, (*sdl).charset);

	sprintf(text, "Wyjscie -'Esc'   //(sprwadzenie wynikow nie zaimplementowane)");
	DrawString((*sdl).screen, (*sdl).screen->w - SCREEN_WIDTH + T_MARGIN, M_T3, text, (*sdl).charset);

	sprintf(text, "Dostepne poziomy: '1' , '2' , '3'");
	DrawString((*sdl).screen, (*sdl).screen->w - SCREEN_WIDTH + T_MARGIN, M_T4, text, (*sdl).charset);

	sprintf(text, "Wpisz komende:");
	DrawString((*sdl).screen, (*sdl).screen->w - SCREEN_WIDTH + T_MARGIN, M_T5, text, (*sdl).charset);

	DrawString((*sdl).screen, (*sdl).screen->w - SCREEN_WIDTH + T_MARGIN, M_T6, (*s).text, (*sdl).charset);

	SDL_UpdateTexture((*sdl).scrtex, NULL, (*sdl).screen->pixels, (*sdl).screen->pitch);
	//		SDL_RenderClear(renderer);
	SDL_RenderCopy((*sdl).renderer, (*sdl).scrtex, NULL, NULL);
	SDL_RenderPresent((*sdl).renderer);
}

void DrawDescription(struct SDL* sdl, struct Colors* c, struct Controls* s)
{
	char text[T_LENGTH];
	DrawRectangle((*sdl).screen, M_MARGIN, M_MARGIN, M_WIDTH, M_HEIGHT, (*c).czerwony, (*c).niebieski);

	sprintf(text, "Czas gry: = %.1lf s", (*s).worldTime);
	DrawString((*sdl).screen, (*sdl).screen->w - strlen(text)*BITY - T_MARGIN, M_T1, text, (*sdl).charset);

	sprintf(text, "Twoje punkty: = %d p", (*s).points);
	DrawString((*sdl).screen, (*sdl).screen->w - SCREEN_WIDTH + T_MARGIN, M_T1, text, (*sdl).charset);
	
	sprintf(text, "Wykonane podpunkty: 1-4 , A-D");
	DrawString((*sdl).screen, (*sdl).screen->w - SCREEN_WIDTH + T_MARGIN, M_T2, text, (*sdl).charset);
}

void DrawSteps(struct SDL* sdl, struct Colors* c, int X, int Y)
{
	int steps = P_SPACE / L_S_HEIGHT / 2;
	for (int s = 0; s < steps; s++)
	{
		DrawRectangle((*sdl).screen, X, Y + (2 * s * L_S_HEIGHT), L_WIDTH, L_S_HEIGHT, (*c).czerwony, (*c).czarny);
	}
}

void DrawLadder(struct SDL* sdl, struct Colors* c, int platform)
{
	int margin = (platform+1) % 2; // skondensowany if dla pozycji z lewej lub prawej w zale¿noœci od platform
	int m[2] = { 1,-1 };
	int mode = m[margin];

	int X = (margin * SCREEN_WIDTH) + (mode * (L_MARGIN)) - (margin * L_WIDTH);
	int Y = SCREEN_HEIGHT - (P_SPACE * platform) - P_HEIGHT;

	DrawRectangle((*sdl).screen, X, Y, L_WIDTH, P_SPACE, (*c).czerwony, (*c).czarny);

	DrawSteps(sdl, c, X, Y);
}

void DrawPlatforms(struct SDL* sdl, struct Colors* c, struct Controls* s)
{
	for (int platform = 1; platform <= (*s).platforms; platform++)
	{
		int m = platform % 2;

		DrawRectangle((*sdl).screen, P_MARGIN * m, SCREEN_HEIGHT - P_HEIGHT - (P_SPACE * platform), P_WIDTH, P_HEIGHT, (*c).czerwony, (*c).czarny);
		DrawLadder(sdl, c, platform);
	}

	DrawRectangle((*sdl).screen, 0, SCREEN_HEIGHT - P_HEIGHT, SCREEN_WIDTH, P_HEIGHT, (*c).zielony, (*c).zielony);
}

void DrawStatic(struct SDL* sdl, struct Colors* c, struct Controls* s, struct Player* p, struct Barrel* b1)
{
	SDL_FillRect((*sdl).screen, NULL, (*c).czarny);

	DrawDescription(sdl, c, s);
	DrawPlatforms(sdl, c, s);

	SDL_SetColorKey((*sdl).player_l, true, 0x000000);
	SDL_SetColorKey((*sdl).player_r, true, 0x000000);
	SDL_SetColorKey((*sdl).barrel, true, 0x000000);
	SDL_SetColorKey((*sdl).enemy, true, 0x000000);

	for (int l = 1; l <= (*s).life; l++)
	{
		DrawSurface((*sdl).screen, (*sdl).heart, SCREEN_WIDTH - (l * H_SPACE), M_T4);
	}

	if ((*p).a_direction == LEFT)
	{
		DrawSurface((*sdl).screen, (*sdl).player_l, (*p).positionX, (*p).positionY);
	}
	else
	{
		DrawSurface((*sdl).screen, (*sdl).player_r, (*p).positionX, (*p).positionY);
	}
	DrawSurface((*sdl).screen, (*sdl).barrel, (*b1).positionX, (*b1).positionY);
	DrawSurface((*sdl).screen, (*sdl).enemy, (B_MARGIN + B_WIDTH), (SCREEN_HEIGHT - P_HEIGHT - (P_SPACE * (*s).platforms) - B_HEIGHT));

	SDL_UpdateTexture((*sdl).scrtex, NULL, (*sdl).screen->pixels, (*sdl).screen->pitch);
	//		SDL_RenderClear(renderer);
	SDL_RenderCopy((*sdl).renderer, (*sdl).scrtex, NULL, NULL);
	SDL_RenderPresent((*sdl).renderer);
}

int OnLadder(struct Controls* s, struct Player* p)
{
	for (int platform = 1; platform <= (*s).platforms; platform++)
	{
		int margin1 = (platform + 1) % 2; // skondensowany if dla pozycji z lewej lub prawej w zale¿noœci od platform
		int margin2 = platform % 2;
		int m[2] = { 1,-1 };
		int mode = m[margin1];

		int X1 = (margin1 * SCREEN_WIDTH) + (mode * (L_MARGIN)) - (margin1 * L_WIDTH);  // kordynaty graniczne drabiny
		int X2 = (margin1 * SCREEN_WIDTH) + (mode * (L_MARGIN)) + (margin2 * L_WIDTH);
		int Y1 = SCREEN_HEIGHT - (P_SPACE * platform) - P_HEIGHT - E_HEIGHT;
		int Y2 = SCREEN_HEIGHT - (P_SPACE * (platform - 1)) - P_HEIGHT - E_HEIGHT;

		if (((int)(*p).positionX >= X1) && ((int)(*p).positionX <= X2) && ((int)(*p).positionY >= Y1) && ((int)(*p).positionY <= Y2))
		{
			return 1;
		}

	}
	return 0;
}

int OnPlatform(struct Controls* s, int P_X, int P_Y)
{
	for (int platform = 1; platform <= (*s).platforms; platform++)
	{
		int margin1 = platform % 2;
		int margin2 = (platform + 1) % 2;

		int X1 = P_MARGIN * margin1;											// kordynaty graniczne platformy
		int X2 = SCREEN_WIDTH - (P_MARGIN * margin2);
		int Y = SCREEN_HEIGHT - P_HEIGHT - (P_SPACE * platform) - E_HEIGHT;
		if ((P_Y == Y) && (P_X >= X1) && (P_X <= X2))
		{
			return 1;
		}
	}

	if (P_Y == (SCREEN_HEIGHT - P_HEIGHT - E_HEIGHT))
	{
		return 1;
	}
	
	return 0;
}

int BarrelColision(struct Controls* s, struct Player* p, struct Barrel* b1) // zderzono z beczk¹
{
	int X1 = (*b1).positionX - (2 * E_WIDTH);
	int X2 = (*b1).positionX + (2 * E_WIDTH);
	int Y1 = (*b1).positionY - (2 * E_WIDTH);
	int Y2 = (*b1).positionY + (2 * E_WIDTH);

	if (((int)(*p).positionX >= X1) && ((int)(*p).positionX <= X2) && ((int)(*p).positionY >= Y1) && ((int)(*p).positionY <= Y2))
	{
		printf("berrelcolision\n");
		(*s).life--;
		(*b1).colision = 1;
		if ((*s).life == 0)
		{
			(*s).nowa = 1;
			(*s).menu = 1;
		}
		return 1;
	}
	return 0;
}

int EnemyColision(struct Controls* s, struct Player* p) // dotarto do punktu koncowego
{
	int X1 = B_MARGIN - E_WIDTH;
	int X2 = B_MARGIN + (2*B_WIDTH) + E_WIDTH;
	int Y1 = SCREEN_HEIGHT - P_HEIGHT - (P_SPACE * (*s).platforms) - (2 * B_HEIGHT) - E_HEIGHT;
	int Y2 = SCREEN_HEIGHT - P_HEIGHT - (P_SPACE * (*s).platforms) - E_HEIGHT;

	if (((int)(*p).positionX >= X1) && ((int)(*p).positionX <= X2) && ((int)(*p).positionY >= Y1) && ((int)(*p).positionY <= Y2))
	{
		printf("finished level\n");
		(*s).points++;
		switch ((*s).level)
		{
		case 1:
			(*s).nowa = 1;
			(*s).level = 2;
			(*s).platforms = PLATFORMS2;
			break;
		case 2:
			(*s).nowa = 1;
			(*s).level = 3;
			(*s).platforms = PLATFORMS3;
			break;
		case 3:
			(*s).nowa = 1;
			(*s).menu = 1;
			break;
		};
		return 1;
	}
	return 0;
}

int Jumping(struct Player* p) // przeskakiwanie przez platformy
{
	if ((*p).j_velocity < 0)
	{
		return 1;
	}
	return 0;
}

int ControlBoarder(int X1, double *X2) // granice planszy
{
	if ((*X2 <= 0 + E_WIDTH) || (*X2 >= SCREEN_WIDTH - E_WIDTH))
	{
		*X2 = X1;
		return 1;
	}
	return 0;
}

void ControlY(struct Controls* s, struct Player* p, int Y) // zapobieganie wchodzenia w pod³oge
{
	if (!OnLadder(s, p))
	{
		(*p).positionY = Y;
	}
}

void JumpMovement(struct Controls* s, struct Player* p) // zachowanie gracza w sytuacji skoku i spadania
{
	int X;

	(*p).j_velocity += GRAVITATION * (*s).delta;

	if ((*p).jump)
	{
		if (OnPlatform(s, (*p).positionX, (*p).positionY) && !Jumping(p))
		{
			(*p).positionY -= 1;
			if (!OnLadder(s, p))
			{
				(*p).j_velocity = J_SPEEDY;
				(*p).j_direction = (*p).direction;
			}
			else (*p).positionY += 1;
		}
	}

	if (!OnLadder(s, p) && (!OnPlatform(s, (*p).positionX, (*p).positionY) || Jumping(p)))
	{
		(*p).positionY += (*p).j_velocity * (*s).delta;
		switch ((*p).j_direction)
		{
		case LEFT:
			X = (*p).positionX;
			(*p).positionX -= SPEEDX * (*s).delta;
			ControlBoarder(X, &(*p).positionX);
			(*p).a_direction = LEFT;
			break;
		case RIGHT:
			X = (*p).positionX;
			(*p).positionX += SPEEDX * (*s).delta;
			ControlBoarder(X, &(*p).positionX);
			(*p).a_direction = RIGHT;
			break;
		};
	}
}

void P_Movement(struct Controls* s, struct Player* p)
{
	int X, Y;

	switch ((*p).direction) 
	{
		case LEFT:
			if (OnPlatform(s, (*p).positionX, (*p).positionY) && !Jumping(p))
			{
				X = (*p).positionX;
				(*p).positionX -= SPEEDX * (*s).delta;
				(*p).j_direction = LEFT;
				ControlBoarder(X, &(*p).positionX);
				(*p).a_direction = LEFT;
			}
			break;
		case RIGHT:
			if (OnPlatform(s, (*p).positionX, (*p).positionY) && !Jumping(p))
			{
				X = (*p).positionX;
				(*p).positionX += SPEEDX * (*s).delta;
				(*p).j_direction = RIGHT;
				ControlBoarder(X, &(*p).positionX);
				(*p).a_direction = RIGHT;
			}
			break;
		case UP:
			if (OnLadder(s, p))
			{
				Y = (*p).positionY;
				(*p).positionY -= SPEEDY * (*s).delta;
				ControlY(s, p, Y);
			}
			break;
		case DOWN:
			if (OnLadder(s, p))
			{
				Y = (*p).positionY;
				(*p).positionY += SPEEDY * (*s).delta;
				ControlY(s, p, Y);
			}
			break;
	};

	JumpMovement(s, p);

	if (OnLadder(s, p) || (OnPlatform(s, (*p).positionX, (*p).positionY) && !Jumping(p)))
	{
		(*p).j_direction = 0;
		(*p).j_velocity = 0;
	}
}

int StopBarrel(struct Controls* s, struct Barrel* b1) // pozycja koncowa toru beczki
{
	int X0 = SCREEN_WIDTH - E_WIDTH - 1;
	int Y0 = SCREEN_HEIGHT - P_HEIGHT - E_HEIGHT;

	if (((int)(*b1).positionX == X0) && ((int)(*b1).positionY == Y0))
	{
		return 1;
	}
	return 0;
}

void B_Movement(struct Controls* s, struct Barrel* b1)
{
	int X;

	switch ((*b1).direction)
	{
	case LEFT:
		X = (*b1).positionX;
		(*b1).positionX -= B_SPEEDX * (*s).delta;
		if (ControlBoarder(X, &(*b1).positionX))
		{
			(*b1).direction = RIGHT;
		}
		break;
	case RIGHT:
		X = (*b1).positionX;
		(*b1).positionX += B_SPEEDX * (*s).delta;
		if (ControlBoarder(X, &(*b1).positionX))
		{
			(*b1).direction = LEFT;
		}
		break;
	};

	(*b1).velocity += GRAVITATION * (*s).delta;

	if (OnPlatform(s, (*b1).positionX, (*b1).positionY))
	{
		(*b1).velocity = 0;
	}

	(*b1).positionY += (*b1).velocity * (*s).delta;

	if (StopBarrel(s, b1) || (*b1).colision)
	{
		(*b1).colision = 0;
		(*b1).positionX = E_WIDTH + B_MARGIN;
		(*b1).positionY = SCREEN_HEIGHT - P_HEIGHT - (P_SPACE * (*s).platforms) - E_HEIGHT;
	}
}

void Menu(struct SDL* sdl, struct Colors* c, struct Controls* s)
{
	while (SDL_PollEvent(&(*sdl).event))
	{
		switch ((*sdl).event.type) {
		case SDL_KEYDOWN:
			if ((*sdl).event.key.keysym.sym == SDLK_ESCAPE) (*s).quit = 1;
			else if ((*sdl).event.key.keysym.sym == SDLK_RETURN)
			{
				switch ((*s).text[0])
				{
				case '1':
					(*s).level = 1;
					(*s).platforms = PLATFORMS1;
					(*s).menu = 0;
					break;
				case '2':
					(*s).level = 2; 
					(*s).platforms = PLATFORMS2; 
					(*s).menu = 0;
					break;
				case '3':
					(*s).level = 3;
					(*s).platforms = PLATFORMS3;
					(*s).menu = 0;
					break;
				default:
					(*s).t = 0;
					(*s).text[(*s).t] = '\0';
					break;
				};
			}
			else if ((*sdl).event.key.keysym.sym == SDLK_BACKSPACE)
			{
				if ((*s).t - 1 >= 0)
				{
					(*s).t--;
					(*s).text[(*s).t] = '\0';
				}
			}
			else
			{
				if ((*s).t + 1 < T_LENGTH)
				{
					(*s).text[(*s).t] = (*sdl).event.key.keysym.sym;
					(*s).t++;
					(*s).text[(*s).t] = '\0';
				}
			}
			break;
		case SDL_QUIT:
			(*s).quit = 1;
			break;
		};

	};

	DrawMenu(sdl, c, s);
}

void Controls(struct SDL* sdl, struct Controls* s, struct Player* p)
{
	// obs³uga zdarzeñ (o ile jakieœ zasz³y)
	while (SDL_PollEvent(&(*sdl).event)) 
	{
		switch ((*sdl).event.type) {
		case SDL_KEYDOWN:
			if ((*sdl).event.key.keysym.sym == SDLK_ESCAPE) { (*s).quit = 1; (*s).menu = 1; (*s).nowa = 1; }
			else if ((*sdl).event.key.keysym.sym == SDLK_n) (*s).nowa = 1;
			else if ((*sdl).event.key.keysym.sym == SDLK_m) { (*s).nowa = 1; (*s).menu = 1; }
			else if ((*sdl).event.key.keysym.sym == SDLK_1) { (*s).level = 1; (*s).platforms = PLATFORMS1; (*s).nowa = 1; }
			else if ((*sdl).event.key.keysym.sym == SDLK_2) { (*s).level = 2; (*s).platforms = PLATFORMS2; (*s).nowa = 1; }
			else if ((*sdl).event.key.keysym.sym == SDLK_3) { (*s).level = 3; (*s).platforms = PLATFORMS3; (*s).nowa = 1; }
			else if ((*sdl).event.key.keysym.sym == SDLK_RIGHT) (*p).direction = RIGHT;
			else if ((*sdl).event.key.keysym.sym == SDLK_LEFT) (*p).direction = LEFT;
			else if ((*sdl).event.key.keysym.sym == SDLK_UP) (*p).direction = UP;
			else if ((*sdl).event.key.keysym.sym == SDLK_DOWN) (*p).direction = DOWN;
			else if ((*sdl).event.key.keysym.sym == SDLK_SPACE) (*p).jump = 1;
			break;
		case SDL_KEYUP:
			if ((*sdl).event.key.keysym.sym == SDLK_SPACE)
			{
				(*p).jump = 0;
			}
			else
			{
				(*p).direction = 0;
			}
			break;
		case SDL_QUIT:
			(*s).quit = 1;
			(*s).menu = 1;
			(*s).nowa = 1;
			break;
		};
	};
}

void Game(struct SDL* sdl, struct Colors* c, struct Controls *s, struct Player *p, struct Barrel *b1)
{
	(*s).quit = 0;
	(*s).points = 0;
	(*s).t = 0;
	(*s).text[(*s).t] = '\0';
	(*s).menu = 1;

	while (!(*s).quit)
	{
		Menu(sdl, c, s);

		while (!(*s).menu)
		{
			int t1, t2;

			(*s).nowa = 0;
			(*s).worldTime = 0;
			(*s).life = LIFE;
			(*p).positionX = SCREEN_WIDTH - E_WIDTH;
			(*p).positionY = SCREEN_HEIGHT - P_HEIGHT - E_HEIGHT;
			(*p).direction = 0;
			(*p).jump = 0;
			(*p).j_direction = 0;
			(*p).j_velocity = 0;
			(*p).a_direction = LEFT;
			(*b1).positionX = E_WIDTH + B_MARGIN;
			(*b1).positionY = SCREEN_HEIGHT - P_HEIGHT - (P_SPACE * (*s).platforms) - E_HEIGHT;
			(*b1).direction = RIGHT;
			(*b1).colision = 0;
			(*s).t = 0;
			(*s).text[(*s).t] = '\0';

			t1 = SDL_GetTicks();

			while (!(*s).nowa)
			{
				t2 = SDL_GetTicks();

				// w tym momencie t2-t1 to czas w milisekundach,
				// jaki uplyna³ od ostatniego narysowania ekranu
				// delta to ten sam czas w sekundach
				(*s).delta = (t2 - t1) * TICK;
				t1 = t2;

				(*s).worldTime += (*s).delta;

				DrawStatic(sdl, c, s, p, b1);
				BarrelColision(s, p, b1);
				EnemyColision(s, p);
				Controls(sdl, s, p);
				P_Movement(s, p);
				B_Movement(s, b1);
			}
		}
	}
	
	DestroySDL(sdl);
	exit(0);
}


int main(int argc, char **argv) {
	struct SDL sdl;
	struct Colors c;
	struct Controls s;
	struct Player p;
	struct Barrel b1;

	// okno konsoli nie jest widoczne, je¿eli chcemy zobaczyæ
	// komunikaty wypisywane printf-em trzeba w opcjach:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// zmieniæ na "Console"
	CreateWindow(&sdl);

	LoadBMP(&sdl);

	CreateColors(&sdl, &c);

	Game(&sdl, &c, &s, &p, &b1);

	return 0;
	};
