#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file

#include "S2D/S2D.h"
#include <iostream>
#define MUNCHIECOUNT 50
#define cherryCount 5
#define GHOSTCOUNT 1




// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;


// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.

struct maze
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
};
struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
};

struct MovingEnemy2
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
};

struct cherry
{
	int currentFrameTime;
	int frame;
	int frameCount;
	Rect* Rect;
	Texture2D* texture;
	Texture2D* InvertedTexture;
	Vector2* position;
};
struct munchie2
{
	int currentFrameTime;
	int frame;
	int frameCount;
	Rect* Rect;
	Texture2D* texture;
	Texture2D* InvertedTexture;
	Vector2* position;
};

struct Player
{
	bool dead;
	float speedMultiplier;
	int currentFrameTime;
	int direction;
	int frame;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
};

class Pacman : public Game
{
private:
Player* _pacman;
munchie2* _munchie2 [MUNCHIECOUNT];
cherry* _cherry [cherryCount];
MovingEnemy* _ghosts[GHOSTCOUNT];
maze* _maze;
SoundEffect* _pop;



	//Input methods
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouse);
	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void Menu(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();
	
	//Update methods
	void UpdatePacman(int elapsedTime, int directions);
	void UpdateMunchie(munchie2*,int elapsedTime);
	void UpdateCherry(int elapsedTime);
	void CheckGhostCollisions();
	void UpdateGhost(MovingEnemy*, int elapsedTime);
    bool collisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
	bool Play(SoundEffect*);
	
	
	

	// Data to represent Pacman

	// Data to represent Munchie
	
	
	//munchie2

	int _frame2Count;

	//Data to represent Cherry

	int _cherryframeCount;
	
	// Position for String
	Vector2* _stringPosition;

	//Constant data for Game Variables 
	const float _cPacmanSpeed;

	// Data for Menu
	
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);
	
	bool _paused;
	bool _startgame;

	bool _pKeyDown; // for the pause button
	bool _SPACEkeyDown; // for the start game

	const int _cPacmanFrameTime; // for  consistency 

	//new munchie
	const int _cMunchie2FrameTime;
	
	////cherry
	const int _cCherryFrameTime;

	int score = 0;
	int cherryscore = 0;

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};