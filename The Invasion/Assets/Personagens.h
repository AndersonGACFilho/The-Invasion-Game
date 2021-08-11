enum {UP,DOWN,LEFT,RIGHT};

typedef struct
{
	//posição
	short int x;
	short int y;
	short int edge_x;
	short int edge_y;
	//
	short int life;
	short int power;
	short int shots;
	short int points;
	short int score;
	short int velocity;
}Player;
typedef struct
{
	short int x;
	short int y;
	short int edge_x;
	short int edge_y;
	short int go_to_x;
	short int go_to_y;
	short int life;
	short int points_per_kill;
	bool status;
	short int velocity;
	short int current_velocity;
	short int type;
}Enimies;
typedef struct
{
	short int x;
	short int y;
	short int edge_x;
	short int edge_y;
	short int damage;
	bool status;
	short int velocity;
}Powers;
typedef struct
{
	short int x;
	short int y;
	short int velocity;
	short int color_r;
	short int color_g;
	short int color_b;
}Stars;