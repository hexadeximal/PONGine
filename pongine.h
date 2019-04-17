#ifndef _PONGINE_H
#define _PONGINE_H

#define ENGINE_PRESETS {{'\0'},\
	NULL,\
	NULL,\
	{0},\
	0,\
	0,\
	false,\
	false,\
	init,\
	set_title,\
	render_texture,\
	clear_screen,\
	destroy,\
	poll_event,\
	check_collision}

#define PADDLE_PRESETS {\
	{0,0,PADDLE_WIDTH,PADDLE_HEIGHT},\
	8.0f,\
	0,\
	0,\
	WHITE,\
	draw,\
	set_position,\
	set_size,\
	set_velocity}

#define BLACK 0,0,0,0
#define WHITE 255,255,255,255
#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1024
#define PADDLE_HEIGHT 200
#define PADDLE_WIDTH 40

extern int errno;

struct engine {
	char title[100];
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	unsigned int width;
	unsigned int height;
	bool fullscreen;
	bool running;
	int (*init) (struct engine *e);
	int (*set_title) (struct engine *e, const char *title);
	int (*render_texture) (struct engine *e, SDL_Texture *t);
	int (*clear_screen) (struct engine *e, unsigned int r, unsigned int g, unsigned int b,unsigned int a);
	void (*destroy) (struct engine *);
	void (*poll_event) (struct engine *, void *);
	int (*check_collision) (void *, void *);
};

struct paddle {
	SDL_Rect rect;
	float velocity;
	int pos_x;
	int pos_y;
	unsigned int r;
	unsigned int g;
	unsigned int b;
	unsigned int a;
	int (*draw) (struct paddle *p, struct engine *e);
	void (*set_position) (struct paddle *p, int x, int y);
	void (*set_size) (struct paddle *p, int w, int h);
	void (*set_velocity) (struct paddle *p, float v);
};

/* paddle prototypes */
int draw (struct paddle *p, struct engine *e);
void set_position (struct paddle *p, int x, int y);
void set_velocity(struct paddle *p, float v);
void set_size(struct paddle *p, int w, int h);

/* engine prototypes */
void destroy (struct engine *e);
int clear_screen(struct engine *e, unsigned int r, unsigned int g, unsigned int b, unsigned int a);
int init(struct engine *e);
int render_texture(struct engine *e, SDL_Texture *t);
int set_title(struct engine *e, const char *title);
void poll_event(struct engine *e, void *p);
int check_collision(void *r1, void *r2);

#endif
