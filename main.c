#include <stdio.h>
#include <SDL2/SDL.h>
#include <errno.h>
#include <stdbool.h>
#include "pongine.h"

struct ball {
	SDL_Rect rect;
	float velocity;
	int pos_x;
	int pos_y;
};

void destroy (struct engine *e)
{
	SDL_DestroyRenderer(e->renderer);
	SDL_DestroyWindow(e->window);
	SDL_Quit();
}

int draw (struct paddle *p, struct engine *e)
{
	SDL_SetRenderDrawColor(e->renderer, WHITE);
	SDL_RenderFillRect(e->renderer, &p->rect);
	SDL_RenderDrawRect(e->renderer, &p->rect);
	return 0;
}

void set_position (struct paddle *p, int x, int y)
{
	p->rect.x = x;
	p->rect.y = y;
}

void set_size(struct paddle *p, int w, int h)
{
	p->rect.w = w;
	p->rect.h = h;
}

int clear_screen(struct engine *e, unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	SDL_SetRenderDrawColor(e->renderer,r,g,b,a);
	SDL_RenderClear(e->renderer);
	return 0;
}
/* init sdl, create renderer, and setup window 
 * TODO:
 *	error managament
 */
int init(struct engine *e)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "%s\n",SDL_GetError());
		return errno;
	}	

	if(strlen(e->title) == 0) {
		fprintf(stderr, "no title set\n");
		return -1;
	}

	if(e->width == 0) {
		fprintf(stderr, "resolution width not set\n");
		return -1;
	}

	if(e->height == 0) {
		fprintf(stderr, "resolution height not set\n");
		return -1;
	}
	e->window = SDL_CreateWindow(e->title,0,0,e->width,e->height,SDL_WINDOW_SHOWN);

	if(e->window == NULL) {
		fprintf(stderr,"%s\n", SDL_GetError());
		return -1;
	}
	e->renderer = SDL_CreateRenderer(e->window, -1, SDL_RENDERER_ACCELERATED | 
			SDL_RENDERER_PRESENTVSYNC);

	if(e->renderer == NULL) {
		SDL_DestroyWindow(e->window);
		fprintf(stderr, "%s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

int render_texture(struct engine *e, SDL_Texture *t)
{

	SDL_RenderClear(e->renderer);
	SDL_RenderCopy(e->renderer, t, NULL, NULL);

	return 0;
}
int set_title(struct engine *e, const char *title) 
{
	if(title == NULL) {
		return -1;
	}

	strcpy(e->title, title);

	return 0;
}

int check_collision(void *r1, void *r2)
{
	struct paddle *rect_1 = (struct paddle *)r1;
	struct paddle *rect_2 = (struct paddle *)r2;
	SDL_Rect result;

	if(SDL_IntersectRect(&rect_1->rect, &rect_2->rect, &result) == SDL_TRUE) {
		return 1;
	}

	return 0;
}

void set_velocity(struct paddle *p, float v)
{
	p->velocity = v;
}
void poll_event(struct engine *e, void *p)
{
	const Uint8 *key_state = SDL_GetKeyboardState(NULL);
	struct paddle *pd = (struct paddle *)p;

	if(key_state[SDL_SCANCODE_W]) {
		pd->rect.y -= pd->velocity;
		if(pd->rect.y < 4) {
			pd->rect.y += pd->velocity;
		}
	}
	if(key_state[SDL_SCANCODE_S]) {
		pd->rect.y += pd->velocity;
		if(pd->rect.y > SCREEN_HEIGHT - pd->rect.h) {
			pd->rect.y -= pd->velocity;
		}
	}
	if(key_state[SDL_SCANCODE_ESCAPE]) {
		e->running = false;
	}

	while(SDL_PollEvent(&e->event)) {
		if(e->event.type == SDL_QUIT) {
			e->running = false;
		}

	}

}

int main(void)
{
	/* TODO: 
	   rename paddle struct to GOB
	*/

	struct engine pong = ENGINE_PRESETS;
	struct paddle p = PADDLE_PRESETS;
	struct paddle ball = PADDLE_PRESETS;
	
	struct paddle p2 = PADDLE_PRESETS;
	p.set_position(&p, PADDLE_WIDTH/2, (SCREEN_HEIGHT/2)-(PADDLE_HEIGHT/2));
	p2.set_position(&p2, (SCREEN_WIDTH - (PADDLE_WIDTH)-22),(SCREEN_HEIGHT/2)-(PADDLE_HEIGHT/2));
	ball.set_position(&ball, (SCREEN_WIDTH/2)-20,(SCREEN_HEIGHT/2)-20);
	ball.set_size(&ball, 40,40);
	pong.set_title(&pong, "pong");
	pong.width = SCREEN_WIDTH;
	pong.height = SCREEN_HEIGHT;
	pong.init(&pong);
	pong.running = true;

	ball.set_velocity(&ball,2);

	while(pong.running == true) {
		ball.rect.x -= ball.velocity;
		pong.poll_event(&pong, &p);	
		pong.clear_screen(&pong,BLACK);
		p.draw(&p,&pong);
		p2.draw(&p2,&pong);
		ball.draw(&ball,&pong);

		if(check_collision(&p, &ball) == 1) {
			ball.velocity++;
			ball.velocity *= -1;
		}
		else if(check_collision(&p2, &ball) == 1) {
			ball.velocity--;
			ball.velocity *= -1;
		}
		
		SDL_RenderPresent(pong.renderer);
	}
	

	pong.destroy(&pong); //ADD
	return 0;
}
