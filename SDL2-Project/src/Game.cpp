#include "Game.h"

Game::Game()
	:window(nullptr), running(true)
{
	GAME_STATE = GameManager::GAME;
}

Game::~Game()
{
	// Always delete the window last so that you delete all the resources first(textures, audio..) and then close all the SDL subsystems
	delete window;
}

void Game::setup()
{
	srand(time(0));

	// Initialize the window
	window = new Window(SCRWIDTH, SCRHEIGHT, "SDL2-Engine");
	window->setFrameRateLimit(60);
	window->setDrawBlendMode(SDL_BLENDMODE_BLEND);

	// Initialize the sprites
	Sprite::UseRenderer(window->getRenderer());
	background = new Sprite("res/images/textures/background.jpg");
	background->setPos(0, 0);

	ball = new Sprite("res/images/textures/soccer-ball.png");
	ball->setSize(30, 30);
	ball->setPos((SCRWIDTH / 2) - (ball->getWidth()/2), (SCRHEIGHT / 2) - (ball->getHeight()/2));
	// Generate random(from two choices) velocity
	if (SDL_GetTicks() % 2 == 0)
		ball->setVelocity(8, 8);
	else
		ball->setVelocity(-8, -8);

	leftWall = new Sprite("res/images/textures/wall.png");
	leftWall->setSize(10, 200);
	leftWall->setPos(40, 150);
	leftWall->setVelocity(0, 8);

	rightWall = new Sprite("res/images/textures/wall.png");
	rightWall->setSize(10, 200);
	rightWall->setPos(SCRWIDTH - 50, 150);
	rightWall->setVelocity(0, 8);

	// Initialize the UI
	Text::UseRenderer(window->getRenderer());
	score1 = 0;
	score2 = 0;
	player1Score = "0";
	player2Score = "0";

	score1Text = new Text("res/fonts/Pointless.ttf", 24, player1Score.c_str(), SCRWIDTH / 4, 80, 50, 50, 255, 255, 255, 150);
	score2Text = new Text("res/fonts/Pointless.ttf", 24, player2Score.c_str(), SCRWIDTH/2 + SCRWIDTH / 4, 80, 50, 50, 255, 255, 255, 150);

	hit = new SoundEffect("res/audio/player-hurt.wav");
}

void Game::update()
{
	while (running)
	{
		// Clear
		window->clear(0, 0, 0, 255);

		// Handle user events
		handleInput();

		// Draw
		background->draw();

		// Draw the middle centre line
		window->drawLine(SCRWIDTH / 2, 0, SCRWIDTH / 2, SCRHEIGHT, 255, 255, 255, 100);
		leftWall->draw();
		rightWall->draw();

		// Draw the UI
		
		score1Text->draw();
		score2Text->draw();

		ball->draw();
		ball->setPos(ball->getXPos() + ball->getVelX(), ball->getYPos() + ball->getVelY());
		
		// Check for collision with ball and walls
		if (leftWall->isCollided(ball))
		{
			hit->play(-1, 0);
			score1++;
			player1Score = std::to_string(score1);
			score1Text->setText(player1Score.c_str());
			ball->setVelocity(abs(ball->getVelX()), ball->getVelY());
		}
		else if (rightWall->isCollided(ball))
		{
			hit->play(-1, 0);
			score2++;
			player2Score = std::to_string(score2);
			score2Text->setText(player2Score.c_str());
			ball->setVelocity(-abs(ball->getVelX()), ball->getVelY());
		}

		// Check for bounds
		if (ball->getXPos() < 0)
			running = false;
		else if ((ball->getXPos() + ball->getWidth()) >= SCRWIDTH)
			running = false;

		if (ball->getYPos() < 0)
			ball->setVelocity(ball->getVelX(), abs(ball->getVelY()));
		else if ((ball->getYPos() + ball->getHeight()) >= SCRHEIGHT)
			ball->setVelocity(ball->getVelX(), -abs(ball->getVelY()));

		// Swap Buffers
		window->display();
	}
}
void Game::handleInput()
{
	// Process user inputs
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			running = false;

		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				running = false;
				break;
			}
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_W] && leftWall->getYPos() > 0)
		leftWall->setPos(leftWall->getXPos(), leftWall->getYPos() - leftWall->getVelY());
	else if (keyState[SDL_SCANCODE_S] && (leftWall->getYPos() + leftWall->getHeight()) <= SCRHEIGHT)
		leftWall->setPos(leftWall->getXPos(), leftWall->getYPos() + leftWall->getVelY());

	if (keyState[SDL_SCANCODE_UP] && rightWall->getYPos() > 0)
		rightWall->setPos(rightWall->getXPos(), rightWall->getYPos() - rightWall->getVelY());
	else if (keyState[SDL_SCANCODE_DOWN] && (rightWall->getYPos() + rightWall->getHeight()) <= SCRHEIGHT)
		rightWall->setPos(rightWall->getXPos(), rightWall->getYPos() + rightWall->getVelY());


}
void Game::run()
{
	setup();
	update();
}
