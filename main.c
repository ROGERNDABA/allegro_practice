#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

typedef struct			s_all
{
	ALLEGRO_DISPLAY		*display;
	ALLEGRO_EVENT_QUEUE	*ev_queue;
	ALLEGRO_BITMAP		*bckg;
	ALLEGRO_BITMAP		*icon;
	ALLEGRO_SAMPLE		*beep;
	ALLEGRO_FONT		*font;
	ALLEGRO_EVENT		ev;
	float				f_size;
	ALLEGRO_COLOR		text_c;
	float				win_h;
	float				win_w;
}						t_all;

t_all *make_struct(void)
{
	t_all	*e;

	e = (t_all *)malloc(sizeof(t_all));
	e->display = NULL;
	e->bckg = NULL;
	e->ev_queue = NULL;
	e->font = NULL;
	e->icon = NULL;
	e->beep = NULL;
	e->win_w = 1600;
	e->win_h = 1200;
	e->f_size = 24;
	return(e);
}

ALLEGRO_BITMAP	*load_bitmap_at_size(const char *filename, int w, int h)
{
	ALLEGRO_BITMAP *resized_bmp;
	ALLEGRO_BITMAP *loaded_bmp;
	ALLEGRO_BITMAP *prev_target;

	resized_bmp = al_create_bitmap(w, h);
	if (!resized_bmp)
		return (NULL);
	loaded_bmp = al_load_bitmap(filename);
	if (!loaded_bmp)
	{
		al_destroy_bitmap(resized_bmp);
		return NULL;
	}
	prev_target = al_get_target_bitmap();
	al_set_target_bitmap(resized_bmp);
	al_draw_scaled_bitmap(loaded_bmp, 0, 0,
	al_get_bitmap_width(loaded_bmp), al_get_bitmap_height(loaded_bmp),
	0, 0, w, h, 0);
	al_set_target_bitmap(prev_target);
	al_destroy_bitmap(loaded_bmp);
	return (resized_bmp);
}

t_all	*init_stuff(void)
{
	t_all	*e;

	e = make_struct();
	al_init();
	al_install_audio();
	al_init_primitives_addon();
	al_init_acodec_addon();
	al_init_image_addon();
	al_init_ttf_addon();
	al_reserve_samples(1);
	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	al_set_new_window_title("Allegro TEST");
	al_install_keyboard();
	e->display = al_create_display(e->win_w, e->win_h);
	e->ev_queue = al_create_event_queue();
	e->beep = al_load_sample( "beep.wav");
	e->icon = al_load_bitmap("yy.png");
	al_set_display_icon(e->display, e->icon);
	e->bckg = load_bitmap_at_size("color.png", e->win_w, e->win_h);
	e->text_c =  al_map_rgb(35, 141, 250);
	al_register_event_source(e->ev_queue,
							al_get_display_event_source(e->display));
	al_register_event_source(e->ev_queue, al_get_keyboard_event_source());
	return (e);
}


void	draw_grid(float x, float y, ALLEGRO_COLOR col, t_all *e)
{
	int		x1;
	int		y1;

	int s = x;
	al_clear_to_color(al_map_rgb(82, 95, 117));
	al_draw_text(e->font, e->text_c, 10, 10, 0, "yeah yeah");
	while (x <= 1400)
	{
		x1 = x + 20;
		y1 = y + 20;
		al_draw_filled_rectangle(x, y, x1, y1, col);
		//al_rest(0.001);
		//al_flip_display();
		x += 40;
		if (x == 1400 && y <= 1000)
		{
			x = s;
			x1 = x + 20;
			y += 40;
			y1 = y + 20;
		}
	}
	al_flip_display();
}
int main(void)
{
	t_all		*e;

	e = init_stuff();
	al_clear_to_color(al_map_rgb(82, 95, 117));
	int s = e->win_h;
	e->font = al_load_ttf_font("fonts/Consolas.ttf", e->f_size, 0);
	//al_draw_bitmap(e->bckg, 0, 0, 0);
	draw_grid(80, 80, al_map_rgb(157, 167, 183), e);
	while (1)
	{
		e->font = al_load_ttf_font("fonts/Consolas.ttf", e->f_size, 0);
		if (e->ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			al_play_sample(e->beep, 1.0, 0.0,1.5,ALLEGRO_PLAYMODE_ONCE,NULL);
			if (e->ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				break;
			//else if (e->ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
				//draw_grid(80, 80, al_map_rgb(157, 167, 183), e);
			//al_flip_display();
		}
		else if (e->ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
		{
			al_acknowledge_resize(e->display);
			e->win_w = al_get_display_width(e->display);
			e->win_h = al_get_display_height(e->display);
			if (e->win_h <= 800)
				e->f_size = e->win_h / 20;
			draw_grid(80, 80, al_map_rgb(157, 167, 183), e);
		}
		else if(e->ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;
		//al_draw_bitmap(e->bckg, 0, 0, 0);
		//al_rest(0.1);
		al_draw_text(e->font, e->text_c, 10, 10, 0, "yeah yeah");
		//al_flip_display();
		al_wait_for_event(e->ev_queue, &e->ev);
		s = e->win_h;
	}
	al_destroy_display(e->display);
	al_destroy_bitmap(e->bckg);
	al_destroy_sample(e->beep);
	return 0;
}
