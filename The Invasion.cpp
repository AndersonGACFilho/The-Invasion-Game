#include <allegro5/allegro.h>
#include <time.h>
#include "Assets/Personagens.h"
#include <allegro5/timer.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/utf8.h>
#include <allegro5/mouse.h>
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;
//Variáveis globais
#define TAM_X 1280
#define TAM_Y 720
ALLEGRO_BITMAP* enemies1 = NULL;
ALLEGRO_BITMAP* enemies2 = NULL;
ALLEGRO_BITMAP* active_enemies1 = NULL;
ALLEGRO_BITMAP* active_enemies2 = NULL;
ALLEGRO_BITMAP* PlayerImage1 = NULL, *PlayerImage2 = NULL,*PlayerImage3 = NULL,*PlayerImage4 = NULL,*PlayerImage5 = NULL,*PlayerImage6 = NULL,*PlayerImage7 = NULL,*PlayerImage8 = NULL,*PlayerImage9 = NULL;
    						//Prototipo
void initializer(Player& player,Stars stars[3][100],Enemies* enemies, Powers* enemies_missile, short int Enemies_Number, Powers* missile, short int shotsNumber);
void DrawPlayer(Player& player,short int curFrame);
//Movimentação
void MoveUpPlayer(Player& player);
void MoveDownPlayer(Player& player);
void MoveLeftPlayer(Player& player);
void MoveRightPlayer(Player& player);
//Plano de Fundo
void background_atualizer(Stars stars[3][100]);
void background_draw(Stars stars[3][100]);
//Atirar
void shootMissile(Player player, Powers shots[], short int shots_number);
void draw_shoot(Powers shots[], short int shots_number);
void atualizeShoot(Player &player,Powers shots[], short int shots_number, Enemies enemies[], short int enemiesNumber);
//Nascer inimigos
void spawn_enemies(Player& player, Enemies enemies[], short int& enemiesNumber);
void draw_enemies(Enemies enimies[], short int enemies_number);
void atualize_enemies(Enemies enimies[], short int enimiesNumber);
//Tiros Inimigos
void enemie_shoot(Player player, Enemies enimies[], short int enemiesNumber, Powers enimies_missile[]);
void draw_enemie_shoot(Powers enimies_missile[], short int num_enemies);
void atualize_enemie_shoot(Player& player, Powers enemies_missile[], short int num_enemies);
void enemies_shot_collision_with_player_shot(Powers enemies_missile[],Powers shots[],short int enemies_number,short int shotsNumber);
int main()
{
	//Variáveis
	int FPS = 75,highscore=0;
	short int active_missiles = 0, active_enemies = 0, explosion_aux = 0, maxFrame = 8, curFrame = 0, frameCount = 0, frameDelay = 20, frameWidth = 64, frameHeight = 64;
	short int i=0,j=0, enemiesNumber = 10,buttons=1,button1,button2,button3,pre_time=-1;
	bool credits = false, mainMenu = true, game_over = false, PlayerAnimation = false;
	bool Running = false, draw = true, keys[4] = { false,false,false,false };
	fstream arq;
	arq.open("HighScore",ios::binary|ios::in);
	arq >> highscore;
	ALLEGRO_EVENT_QUEUE* event_list;
	ALLEGRO_TIMER* timer=NULL;
	ALLEGRO_TIMER* exposion_time = NULL;
	ALLEGRO_TIMER* moving_timer = NULL;
	ALLEGRO_SAMPLE* music=NULL;
	ALLEGRO_SAMPLE* mainmenumusic = NULL;
	ALLEGRO_SAMPLE* buttonselected = NULL;
	ALLEGRO_SAMPLE_INSTANCE* inst_music=NULL;
	ALLEGRO_SAMPLE_INSTANCE* inst_mainmenumusic = NULL;
	ALLEGRO_SAMPLE_INSTANCE* inst_buttonselected = NULL;
	ALLEGRO_BITMAP* with_life = NULL;
	ALLEGRO_BITMAP* lifeless = NULL;
	ALLEGRO_BITMAP* ammo = NULL;
	ALLEGRO_BITMAP* no_ammo = NULL;
	ALLEGRO_BITMAP* explosion = NULL;

	Enemies enemies[100];
	Player player;
	Powers missile[3];
	Powers enemiesMissile[100];
	Stars stars[3][100];
	//inicialização
	if (!al_init())
	{
		cout << "ERROR TO ALLEGRO INICIALIZATION\n";
		exit(-1);
	}
	//Display
	ALLEGRO_DISPLAY* window;
	window = al_create_display(TAM_X, TAM_Y);
	if (!window)
	{
		cout << "ERROR TO CREATE DISPLAY\n";
		exit(-1);
	}
	al_set_window_title(window, "The Invasion");
	//inicialização de addons
	al_install_keyboard();
	timer = al_create_timer(1.0 / FPS);
	exposion_time = al_create_timer(0.5);
	moving_timer = al_create_timer(1.0/9);
	al_install_mouse();
	al_install_audio();
	al_reserve_samples(10);
	al_init_image_addon();
	al_init_acodec_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	//Criação da fila de eventos
	event_list = al_create_event_queue();
	//Carregar sons
	music = al_load_sample("sounds/music.ogg");
	mainmenumusic = al_load_sample("sounds/menumusic.ogg");
	buttonselected = al_load_sample("sounds/button_selected.wav");
	//Addons
	with_life = al_load_bitmap("Assets/com_vida.jpg");
	lifeless = al_load_bitmap("Assets/sem_vida.png");
	ammo = al_load_bitmap("Assets/missel.png");
	no_ammo = al_load_bitmap("Assets/sem_missel.png");
	explosion = al_load_bitmap("Assets/explosion.png");
	enemies1 = al_load_bitmap("Assets/inimigos.png");
	enemies2 = al_load_bitmap("Assets/inimigos2.png");
	active_enemies1 = al_load_bitmap("Assets/inimigos_ativo.png");
	active_enemies2 = al_load_bitmap("Assets/inimigos2_ativo.png");

	al_set_system_mouse_cursor(window, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
	ALLEGRO_FONT* fontTitle = al_load_font("fonts/Title.otf", 100, NULL);
	ALLEGRO_FONT* fontButton = al_load_font("fonts/buttons.otf", 25, NULL);
	ALLEGRO_FONT* score = al_load_font("fonts/Score.ttf", 30, NULL);
	inst_music = al_create_sample_instance(music);
	inst_mainmenumusic = al_create_sample_instance(mainmenumusic);
	inst_buttonselected = al_create_sample_instance(buttonselected);

	al_attach_sample_instance_to_mixer(inst_music, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_mainmenumusic, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_buttonselected, al_get_default_mixer());
	
	al_set_sample_instance_playmode(inst_music,ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_playmode(inst_mainmenumusic, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_playmode(inst_buttonselected, ALLEGRO_PLAYMODE_ONCE);

	al_set_sample_instance_gain(inst_mainmenumusic, 0.01);
	al_set_sample_instance_gain(inst_music,0.008);
	al_set_sample_instance_gain(inst_buttonselected, 1.0);

	//registrar fila de eventos
	al_register_event_source(event_list, al_get_keyboard_event_source());
	al_register_event_source(event_list, al_get_mouse_event_source());
	al_register_event_source(event_list, al_get_display_event_source(window));
	al_register_event_source(event_list, al_get_timer_event_source(timer));
	//Loop Principal
	srand(time(NULL));
	al_start_timer(timer);
	initializer(player,stars, enemies, enemiesMissile,enemiesNumber, missile, 10);
	background_draw(stars);
	al_hide_mouse_cursor(window);
	while (mainMenu)
	{
		al_draw_text(fontTitle, al_map_rgb(0, 255, 100), TAM_X - 100, 50, ALLEGRO_ALIGN_RIGHT, "The Invasion");
		al_play_sample_instance(inst_mainmenumusic);
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_list, &ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			mainMenu = false;
		}
		if (buttons > 3)
		{
			buttons = 1;
		}
		if (buttons < 1)
		{
			buttons = 3;
		}
		if ((buttons == 1))
		{
			button1 = 100;
		}
		else button1 = 75;
		if (buttons == 2)
		{
			button2 = 100;
		}
		else button2 = 75;
		if (buttons == 3)
		{
			button3 = 100;
		}
		else button3 = 75;
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_W:
				al_play_sample_instance(inst_buttonselected);
				buttons--;
				break;
			case ALLEGRO_KEY_S:
				al_play_sample_instance(inst_buttonselected);
				buttons++;
				break;
			case ALLEGRO_KEY_UP:
				al_play_sample_instance(inst_buttonselected);
				buttons--;
				break;
			case ALLEGRO_KEY_DOWN:
				al_play_sample_instance(inst_buttonselected);
				buttons++;
				break;
			case ALLEGRO_KEY_SPACE:
				al_play_sample_instance(inst_buttonselected);
				if (buttons == 1)
				{
					al_stop_sample_instance(inst_mainmenumusic);
					Running = true;
				}
				if (buttons == 2)
				{
					al_stop_sample_instance(inst_mainmenumusic);
					credits = true;
				}
				if (buttons == 3)
				{
					al_stop_sample_instance(inst_mainmenumusic);
					mainMenu = false;
				}
				break;
			case ALLEGRO_KEY_ENTER:
				al_play_sample_instance(inst_buttonselected);
				if (buttons == 1)
				{
					al_stop_sample_instance(inst_mainmenumusic);
					Running = true;
				}
				if (buttons == 2)
				{
					al_stop_sample_instance(inst_mainmenumusic);
					credits = true;
				}
				if (buttons == 3)
				{
					al_stop_sample_instance(inst_mainmenumusic);
					mainMenu = false;
				}
				break;
			}
		}
		while (credits)
		{
			al_wait_for_event(event_list, &ev);
			al_play_sample_instance(inst_mainmenumusic);
			if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{

				al_play_sample_instance(inst_buttonselected);
				credits = false;
			}
			if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				background_atualizer(stars);
			}
			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				credits = false;
				mainMenu = false;
			}
			background_draw(stars);
			al_draw_text(fontTitle, al_map_rgb(20, 255, 200), 640, 25, ALLEGRO_ALIGN_CENTER, "Credits");
			al_draw_text(fontButton, al_map_rgb(110, 255, 110), 540, 360, ALLEGRO_ALIGN_CENTER, "PROGRAMMER      ANDERSON GONÇALVES");
			al_draw_text(fontButton, al_map_rgb(110, 255, 110), 540, 390, ALLEGRO_ALIGN_CENTER, "GAME DESIGN      ANDERSON GONÇALVES");
			al_draw_text(fontButton, al_map_rgb(110, 255, 110), 615, 420, ALLEGRO_ALIGN_CENTER, "ARTIST                    ANDERSON GONÇALVES    HUGO CITY");
			i++;
			if (i > 30)
			{
				al_draw_text(fontButton, al_map_rgb(175, 255, 110), 615, 648, ALLEGRO_ALIGN_CENTER, "PRESS ANY BUTTON FOR RETURN TO MAIN MENU");
			}
			if (i == 60)
			{
				i = 0;
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(2, 2, 10));
		}
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			background_atualizer(stars);

		}
		background_draw(stars);
		al_draw_filled_rectangle(100, 300, 450, 350, al_map_rgb(button1, button1, button1));
		al_draw_filled_rectangle(100, 400, 450, 450, al_map_rgb(button2, button2, button2));
		al_draw_filled_rectangle(100, 500, 450, 550, al_map_rgb(button3, button3, button3));
		al_draw_textf(fontButton, al_map_rgb(0, 255, 200), 150, 212, ALLEGRO_ALIGN_LEFT, "HIGHSCORE - %hd",highscore);
		al_draw_text(fontButton, al_map_rgb(0, 0, 0), 300, 312, ALLEGRO_ALIGN_RIGHT, "PLAY");
		al_draw_text(fontButton, al_map_rgb(0, 0, 0), 325, 412, ALLEGRO_ALIGN_RIGHT, "CREDITS");
		al_draw_text(fontButton, al_map_rgb(0, 0, 0), 300, 512, ALLEGRO_ALIGN_RIGHT, "QUIT");
		al_flip_display();
		al_clear_to_color(al_map_rgb(2, 2, 10));
		j = 0;
		while (Running)
		{
			if (j == 0)
			{
				initializer(player, stars, enemies, enemiesMissile, enemiesNumber, missile, 10);
				PlayerImage1 = al_load_bitmap("Assets/Nave/nave1.png");
				PlayerImage2 = al_load_bitmap("Assets/Nave/nave2.png");
				PlayerImage3 = al_load_bitmap("Assets/Nave/nave3.png");
				PlayerImage4 = al_load_bitmap("Assets/Nave/nave4.png");
				PlayerImage5 = al_load_bitmap("Assets/Nave/nave5.png");
				PlayerImage6 = al_load_bitmap("Assets/Nave/nave6.png");
				PlayerImage7 = al_load_bitmap("Assets/Nave/nave7.png");
				PlayerImage8 = al_load_bitmap("Assets/Nave/nave8.png");
				PlayerImage9 = al_load_bitmap("Assets/Nave/nave9.png");
				curFrame = 9;
				j++;
			}
			
			al_wait_for_event(event_list, &ev);
			if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_W:
					keys[UP] = true;

					break;
				case ALLEGRO_KEY_S:
					keys[DOWN] = true;

					break;
				case ALLEGRO_KEY_A:
					keys[LEFT] = true;

					break;
				case ALLEGRO_KEY_D:
					keys[RIGHT] = true;

					break;
				case ALLEGRO_KEY_ESCAPE:
					al_stop_sample_instance(inst_music);
					Running = false;
					break;
				case ALLEGRO_KEY_SPACE:
					shootMissile(player, missile, player.shots);
					break;
				}
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_W:
					keys[UP] = false;

					break;
				case ALLEGRO_KEY_S:
					keys[DOWN] = false;

					break;
				case ALLEGRO_KEY_A:
					keys[LEFT] = false;

					break;
				case ALLEGRO_KEY_D:
					keys[RIGHT] = false;

					break;
				}
			}
			PlayerAnimation = false;
			if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				frameCount++;
				draw = true;
				if (frameCount == frameDelay)
				{
					PlayerAnimation = true;
					frameCount=0;
					curFrame++;
					if (curFrame > maxFrame)
					{
						curFrame = 0;
					}
				}
				background_atualizer(stars);
				if (keys[UP])
				{
					MoveUpPlayer(player);
				}
				else if (keys[DOWN])
				{
					MoveDownPlayer(player);
				}
				else if (keys[LEFT])
				{
					MoveLeftPlayer(player);
				}
				else if (keys[RIGHT])
				{
					MoveRightPlayer(player);
				}
				spawn_enemies(player, enemies, enemiesNumber);
				atualize_enemies(enemies, enemiesNumber);
				enemie_shoot(player, enemies, enemiesNumber, enemiesMissile);
				atualizeShoot(player, missile, player.shots, enemies, enemiesNumber);
				atualize_enemie_shoot(player, enemiesMissile, enemiesNumber);
				enemies_shot_collision_with_player_shot(enemiesMissile, missile, enemiesNumber, player.shots);
				
			}
			al_play_sample_instance(inst_music);
			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				al_stop_sample_instance(inst_music);
				Running = false;
				mainMenu = false;
			}
			if (al_is_event_queue_empty(event_list) && draw)
			{
				//Desenha

				background_draw(stars);
				draw_enemies(enemies, enemiesNumber);
				DrawPlayer(player,curFrame);
				draw_shoot(missile, player.shots);
				draw_enemie_shoot(enemiesMissile, enemiesNumber);
				al_draw_filled_rectangle(0,650,1280,720,al_map_rgb(70, 70, 70));
				al_draw_filled_rectangle(5, 655, 1275, 700, al_map_rgb(100, 100, 100));
				al_draw_filled_rectangle(0, 0, 1280, 60, al_map_rgb(70, 70, 70));
				al_draw_filled_rectangle(6, 6, 1274, 54, al_map_rgb(100, 100, 100));
				
				
				if (player.life >=1)
				{
					al_draw_bitmap(with_life,33,8,0);
				}
				else
				{
					al_draw_bitmap(lifeless, 33, 8, 0);
				}
				if (player.life >= 2)
				{
					al_draw_bitmap(with_life, 63, 8, 0);
				}
				else
				{
					al_draw_bitmap(lifeless, 63, 8, 0);
				}
				if (player.life == 3)
				{
					al_draw_bitmap(with_life, 93, 8, 0);
				}
				else
				{
					al_draw_bitmap(lifeless, 93, 8, 0);
				}
				for (i = 0; i < player.shots; i++)
				{
					if (!missile[i].status)
					{
						active_missiles++;
					}
				}
				active_enemies = 0;
				for (i = 0; i < enemiesNumber; i++)
				{
					if (enemies[i].status)
					{
						active_enemies++;
					}
				}
				al_draw_textf(fontButton,al_map_rgb(0,200,100),TAM_X-145,20,ALLEGRO_ALIGN_CENTER,"Enemies Alive - %hd", active_enemies);;
				al_draw_textf(fontButton, al_map_rgb(0, 200, 200), TAM_X / 2, 20, ALLEGRO_ALIGN_CENTER, "SCORE - %hd", player.score);
				if (active_missiles>0)
				{
					al_draw_bitmap(ammo, 13, 647, 0);
				}
				else
				{
					al_draw_bitmap(no_ammo, 13, 647, 0);
				}
				if (active_missiles>1)
				{
					al_draw_bitmap(ammo, 43, 647, 0);
				}
				else
				{
					al_draw_bitmap(no_ammo, 43, 647, 0);
				}
				if (active_missiles==3)
				{
					al_draw_bitmap(ammo, 73, 647, 0);
				}
				else
				{
					al_draw_bitmap(no_ammo, 73, 647, 0);
				}
				active_missiles = 0;
				draw = false;
				al_flip_display();
				al_clear_to_color(al_map_rgb(2, 2, 10));
			}
			if (player.life < 1)
			{
				Running = false;
				game_over = true;
	
				al_stop_sample_instance(inst_music);
			}
			
		}
		while (game_over)
		{
			al_wait_for_event(event_list, &ev);
			al_play_sample_instance(inst_mainmenumusic);
			if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				al_play_sample_instance(inst_buttonselected);
				game_over = false;
			}
			if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				background_atualizer(stars);
			}
			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				game_over = false;
				mainMenu = false;
			}
			background_draw(stars);
			if (player.score > highscore)
			{
				al_draw_textf(fontButton, al_map_rgb(175, 255, 120), 640, 400, ALLEGRO_ALIGN_CENTER, "New Highscore");
			}
			else
			al_draw_textf(fontButton, al_map_rgb(175, 255, 120), 640, 400, ALLEGRO_ALIGN_CENTER, "HighScore - %d",highscore);
			al_draw_text(fontTitle, al_map_rgb(255, 20, 20), 640,260, ALLEGRO_ALIGN_CENTER, "Game Over");
			al_draw_textf(fontButton, al_map_rgb(175, 255, 120), 640, 500, ALLEGRO_ALIGN_CENTER,"Your Score - %d",player.score);
			i++;
			if (i > 30)
			{
				al_draw_text(fontButton, al_map_rgb(175, 255, 110), 615, 648, ALLEGRO_ALIGN_CENTER, "PRESS ANY BUTTON FOR RETURN TO MAIN MENU");
			}
			if (i == 60)
			{
				i = 0;
			}
			al_flip_display();
			
			al_clear_to_color(al_map_rgb(2, 2, 10));
		}
		if (player.score > highscore)
		{
			highscore = player.score;
		}
	}
	arq.close();
	arq.open("HighScore", ios::out | ios::binary);
	arq << highscore;
	arq.close();
	al_destroy_bitmap(lifeless);
	al_destroy_bitmap(ammo);
	al_destroy_bitmap(no_ammo);
	al_destroy_bitmap(explosion);
	al_destroy_bitmap(with_life);
	al_destroy_bitmap(enemies1);
	al_destroy_bitmap(enemies2);
	al_destroy_bitmap(PlayerImage1);
	al_destroy_bitmap(PlayerImage2);
	al_destroy_bitmap(PlayerImage3);
	al_destroy_bitmap(PlayerImage4);
	al_destroy_bitmap(PlayerImage5);
	al_destroy_bitmap(PlayerImage6);
	al_destroy_bitmap(PlayerImage7);
	al_destroy_bitmap(PlayerImage8);
	al_destroy_bitmap(PlayerImage9);
	al_destroy_display(window);
	al_destroy_font(fontTitle);
	al_destroy_event_queue(event_list);
	al_destroy_timer(timer);
	al_destroy_font(score);
	al_destroy_font(fontButton);
	al_destroy_sample_instance(inst_mainmenumusic);
	al_destroy_sample(mainmenumusic);
	al_destroy_sample_instance(inst_music);
	al_destroy_sample(music);
	al_destroy_sample_instance(inst_buttonselected);
	al_destroy_sample(buttonselected);
}
//Funções
void initializer(Player& player, Stars stars[3][100], Enemies* enemies, Powers* enemies_missile, short int Enemies_Number, Powers* missile, short int shotsNumber)
{ 
	int i,j;
	//Player
	player.x = 40;
	player.y = TAM_Y / 2;
	player.edge_x = 20;
	player.edge_y = 9;
	player.shots = 3;
	player.life = 3;
	player.power = 0;
	player.points = 0;
	player.score = 0;
	player.velocity = 4;
	//Inimigos
	for (i = 0; i < 100; i++)
	{
		enemies[i].x = TAM_X + 20;
		enemies[i].edge_x = 60;
		enemies[i].edge_y = 13;
		enemies[i].life = 1;
		enemies[i].status = false;
		enemies[i].velocity = 3;
		enemies[i].current_velocity = 3;
		enemies[i].velocity = 3;
	}
	Enemies_Number = 9;
	//Misseis Inimigos
	for (i = 0; i < 100; i++)
	{
		enemies_missile[i].damage = 1;
		enemies_missile[i].edge_x = 4;
		enemies_missile[i].edge_y = 4;
		enemies_missile[i].status = false;
		enemies_missile[i].velocity = 7;
	}
	//Misseis
	for (i = 0; i < 10; i++)
	{
		missile[i].damage = 1;
		missile[i].edge_x = 4;
		missile[i].edge_y = 4;
		missile[i].status = false;
		missile[i].velocity = 10;
	}
	//Background
	for (j = 0; j < 100; j++)
	{
		stars[0][j].velocity = 8;
	}
	for (j = 0; j < 100; j++)
	{
		stars[1][j].velocity = 4;
	}
	for (j = 0; j < 100; j++)
	{
		stars[2][j].velocity = 2;
	}
	for (i = 0; i < 3; i++)
	{
		for(j = 0; j < 100; j++)
		{
			stars[i][j].x = 5 + rand() % (TAM_X - 10);
			stars[i][j].y = 5 + rand() % (TAM_Y - 10);
			stars[i][j].color_r = rand() % 255;
			stars[i][j].color_g = rand() % 255;
			stars[i][j].color_b = rand() % 255;
		}
	}
}
void DrawPlayer(Player& player,short int curFrame)
{
	switch (curFrame)
		{
		case 0:
			al_draw_bitmap(PlayerImage1, player.x - 32, player.y - 32, 0);
			break;
		case 1:
			al_draw_bitmap(PlayerImage2, player.x - 32, player.y - 32, 0);
			break;
		case 2:
			al_draw_bitmap(PlayerImage3, player.x - 32, player.y - 32, 0);
			break;
		case 3:
			al_draw_bitmap(PlayerImage4, player.x - 32, player.y - 32, 0);
			break;
		case 4:
			al_draw_bitmap(PlayerImage5, player.x - 32, player.y - 32, 0);
			break;
		case 5:
			al_draw_bitmap(PlayerImage6, player.x - 32, player.y - 32, 0);
			break;
		case 6:
			al_draw_bitmap(PlayerImage7, player.x - 32, player.y - 32, 0);
			break;
		case 7:
			al_draw_bitmap(PlayerImage8, player.x - 32, player.y - 32, 0);
			break;
		case 8:
			al_draw_bitmap(PlayerImage9, player.x - 32, player.y - 32, 0);
			break;
	}
}
void MoveUpPlayer(Player& player)
{
	player.y -= player.velocity;
	if (player.y < 80)
	{
		player.y = 80;
	}
}
void MoveDownPlayer(Player& player)
{
	player.y += player.velocity;
	if (player.y > TAM_Y-90)
	{
		player.y = TAM_Y-90;
	}
}
void MoveLeftPlayer(Player& player) 
{
	player.x -= player.velocity;
	if(player.x-player.edge_x < 12)
	{
		player.x = player.edge_x+12;
	}
}
void MoveRightPlayer(Player& player)
{
	player.x += player.velocity;
	if (player.x >TAM_X/3-20)
	{
		player.x = (TAM_X / 3) - 20;
	}
}
void background_atualizer(Stars stars[3][100])
{
	int i,j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 100; j++)
		{
			stars[i][j].x -= stars[i][j].velocity;
			if (stars[i][j].x < 0)
			{
				stars[i][j].x = TAM_X;
			}
		}
	}
}
void background_draw(Stars stars[3][100])
{
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 100; j++)
		{
			al_draw_filled_rectangle(stars[i][j].x, stars[i][j].y, stars[i][j].x+2, stars[i][j].y+2, al_map_rgb(stars[i][j].color_r, stars[i][j].color_g, stars[i][j].color_b));
		}
	}
}
void shootMissile(Player player, Powers shots[], short int shots_number)
{
	int i;
	for ( i = 0; i < player.shots; i++)
	{
		if (!shots[i].status)
		{
			shots[i].status= true;
			shots[i].x = player.x + 20;
			shots[i].y = player.y;
			return;
		}
	}
}
void atualizeShoot(Player &player,Powers shots[], short int shots_number,Enemies enemies[],short int enemiesNumber)
{
	int i,j;
	for (i = 0; i < shots_number; i++)
	{
		if (shots[i].status == true)
		{
			shots[i].x += shots[i].velocity;
			if (shots[i].x>TAM_X)
			{
				shots[i].status = false;
			}
			//Colisão com inimigos
			for (j = 0; j < enemiesNumber; j++)
			{
				if (enemies[j].status&&(((shots[i].x+shots[i].edge_x> enemies[j].x +5 )&&
					(shots[i].x - shots[i].edge_x < enemies[j].x + enemies[j].edge_x))&&
					((shots[i].y + shots[i].edge_y > enemies[j].y - enemies[j].edge_y)&&
						(shots[i].y - shots[i].edge_y < enemies[j].y + enemies[j].edge_y))))
				{
					shots[i].status = false;
					enemies[j].life -= shots[i].damage;
					if (enemies[j].life<=0)
					{
						enemies[j].status = false;
						player.points++;
						player.score += 10;
						enemies[j].current_velocity = enemies[j].velocity;
					}
				}
			}
		}
	}
}
void draw_shoot(Powers shots[], short int shots_number)
{
	int i;
	for (i = 0; i < shots_number; i++)
	{
		if (shots[i].status == true)
		{	
			al_draw_filled_rounded_rectangle(shots[i].x + 10, shots[i].y - 2, shots[i].x - 2, shots[i].y + 2,-10,-2,al_map_rgb(200, 255, 200));
			al_draw_filled_rectangle(shots[i].x + 8, shots[i].y - 1, shots[i].x - 1, shots[i].y + 1, al_map_rgb(0, 200, 0));
			
		}
	}
}
void spawn_enemies(Player& player, Enemies enemies[], short int& enemiesNumber)
{
	int i,chance=500;
	if (player.points%enemiesNumber*2==0&&player.points)
	{
		player.points = 0;
		enemiesNumber++ ;
	}
	for (i = 0; i < enemiesNumber; i++)
	{
		if (enemiesNumber == 25)
		{
			enemiesNumber = 10;
			enemies[i].life ++;
			if(enemies[i].velocity<9){enemies[i].velocity++;}
			if(player.life<3)
				player.life++;
		}
		if (!enemies[i].status)
		{
			if (rand()%chance==0)
			{
				enemies[i].x = TAM_X;
				enemies[i].y = 93 +(rand() %7 ) * 90;
				enemies[i].status = true;
				enemies[i].type = rand() % 2;
				break;
			}
		}
	}
}
void draw_enemies(Enemies enemies[], short int enemies_number)
{
	short int i=0,last_type=3;
	for (i = 0; i < enemies_number; i++)
	{
		if (enemies[i].status&&enemies[i].current_velocity==0)
		{

			if (enemies[i].type == 0)
			{
				al_draw_bitmap(enemies1, enemies[i].x, enemies[i].y - 44, 0);
			}
			else
			{
				al_draw_bitmap(enemies2, enemies[i].x, enemies[i].y - 44, 0);
			}
		}
		else
		{
			if (enemies[i].status)
			{
				if (enemies[i].type == 0)
				{
					al_draw_bitmap(active_enemies1, enemies[i].x, enemies[i].y - 44, 0);
				}
				else
				{
					al_draw_bitmap(active_enemies2, enemies[i].x, enemies[i].y - 44, 0);
				}
			}
		}
	}
}
void atualize_enemies(Enemies enemies[], short int enemiesNumber)
{
	int i, j,k, collision=0,velocity=0;
	for (i = 0; i < enemiesNumber; i++)
	{
		if (enemies[i].status)
		{
			velocity = enemies[i].velocity;
				for (j = 0; j < enemiesNumber; j++)
				{
					if (j==i)
					{
						j++;
					}
					if (enemies[j].status&& j < enemiesNumber)
					{
						if(enemies[i].x + enemies[i].edge_x*2-7> enemies[j].x && enemies[i].y + enemies[i].edge_y> enemies[j].y)
						{
							if (enemies[i].x < enemies[j].x+enemies[j].edge_x*2-7 && enemies[i].y < enemies[j].y+enemies[j].edge_x)
							{
								if (enemies[i].x > enemies[j].x)
								{
									enemies[i].current_velocity = 0;
									
								}
								else
								{
									enemies[j].current_velocity = 0;
									
								}
							}
						}
					}
				}
			if (enemies[i].x > (TAM_X / 3) + 20 )
			{
				enemies[i].x -= enemies[i].current_velocity;
				
			}
			else
			{
				enemies[i].current_velocity = 0;
			}
		}
	}
}
void enemie_shoot(Player player, Enemies enemies[], short int enemiesNumber,Powers enemies_missile[])
{
	short int i, chance = 500, enemies_ahead[100], j;
	for (i = 0; i < enemiesNumber; i++)
	{
		enemies_ahead[i] = 0;
		if (enemies[i].status)
		{
			for (j = 0; j < enemiesNumber; j++)
			{
				if (i == j)
				{
					j++;
				}
				if (enemies[j].status && j < enemiesNumber)
				{
					if (enemies[i].y == enemies[j].y)
					{
						if (enemies[i].x > enemies[j].x)
						{
							enemies_ahead[i]++;
						}
					}
				}
			}
		}
	}
	chance -= 50 * player.score/enemiesNumber/10;
	for (j = 0; j < enemiesNumber; j++)
	{
		i= rand() % enemiesNumber;
		if (enemies_ahead[i] == 0)
		{
			if (enemies[i].status)
			{
				if (!enemies_missile[i].status)
				{
					if (rand() % chance == 0)
					{
						enemies_missile[i].status = true;
						enemies_missile[i].x = enemies[i].x;
						enemies_missile[i].y = enemies[i].y;
					}

				}
			}
		}
	}
	
}
void draw_enemie_shoot(Powers enemies_missile[], short int num_enemies)
{
	short int i;
	for ( i = 0; i < num_enemies; i++)
	{
		
		if (enemies_missile[i].status)
		{

			al_draw_filled_rectangle(enemies_missile[i].x-2, enemies_missile[i].y - 2, enemies_missile[i].x + 2,enemies_missile[i].y + 2,al_map_rgb(255,200,200));
			al_draw_filled_rectangle(enemies_missile[i].x - 1, enemies_missile[i].y - 1, enemies_missile[i].x + 1, enemies_missile[i].y + 1, al_map_rgb(255, 200, 200));
		}
		
	}
}
void atualize_enemie_shoot(Player &player,Powers enemies_missile[], short int num_enemies)
{
	short int i;
	for (i = 0; i < num_enemies; i++)
	{

		if (enemies_missile[i].status)
		{
			enemies_missile[i].x -= enemies_missile[i].velocity;
			if (enemies_missile[i].x + enemies_missile[i].edge_x > player.x - player.edge_x &&
				enemies_missile[i].x - enemies_missile[i].edge_x < player.x + player.edge_x &&
				enemies_missile[i].y + enemies_missile[i].edge_y > player.y - player.edge_y &&
				enemies_missile[i].y - enemies_missile[i].edge_y < player.y + player.edge_y)
			{
				enemies_missile[i].status = false;
				player.life -= 1;
			}
		}
		if (enemies_missile[i].x<0)
		{
			enemies_missile[i].status = false;
		}
	}
}
void enemies_shot_collision_with_player_shot(Powers enemies_missile[], Powers shots[], short int enemies_number, short int shotsNumber)
{
	int i, j;
	for (i = 0; i < enemies_number; i++)
	{
		if (enemies_missile[i].status)
		{
			for ( j = 0; j < shotsNumber; j++)
			{
				if (shots[j].status)
				{
					if (enemies_missile[i].x + enemies_missile[i].edge_x > shots[j].x - shots[j].edge_x &&
						enemies_missile[i].x - enemies_missile[i].edge_x < shots[j].x + shots[j].edge_x &&
						enemies_missile[i].y + enemies_missile[i].edge_y > shots[j].y - shots[j].edge_y &&
						enemies_missile[i].y - enemies_missile[i].edge_y < shots[j].y + shots[j].edge_y)
					{
						enemies_missile[i].status = false;
						shots[j].status = false;
					}
				}
			}
		}
	}
}