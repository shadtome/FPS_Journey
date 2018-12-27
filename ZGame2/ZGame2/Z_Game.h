#ifndef ZGAME_H
#define ZGAME_H

enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_LOSE
};


class Game
{
public:
	//Game states
	GameState State;
	// keep track of movement from the player and actions
	bool Keys[1024];
	
	unsigned int Width, Height;

	//Constructor/Destructor
	Game(unsigned int width, unsigned int height);
	-Game();

	//Initalization of game state (load all shaders/textures/levels ect
	void Init();

	//GameLoop
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
};


























#endif
