#pragma once
#include "Engine/Window.h"
#include "Engine/Sprite.h"
#include "Engine/Text.h"
#include "Engine/Button.h"
#include "Engine/Timer.h"
#include "Engine/Audio.h"
#include "Engine/GameManager.h"

#define SCRWIDTH 800
#define SCRHEIGHT 600

class Game
{
private:
	Window* window;
	bool running;

	GameManager GAME_STATE;
	
	Sprite* background;
	Sprite* ball;
	Sprite* leftWall;
	Sprite* rightWall;

	int score1;
	Text* score1Text;
	std::string player1Score;
	int score2;
	Text* score2Text;
	std::string player2Score;

	SoundEffect* hit;

public:
	Game();
	~Game();

	void handleInput();
	void run();
	void setup();
	void update();
};

