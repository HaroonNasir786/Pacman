#include "Pacman.h"

#include <sstream>
#include <time.h>


Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.2f), _cPacmanFrameTime(250), _cMunchie2FrameTime(500), _cCherryFrameTime(500)

{
	{
		srand(time(NULL));
		int i;

		for (i = 0; i < MUNCHIECOUNT; i++)
		{
			_munchie2[i] = new munchie2();
			_munchie2[i]->currentFrameTime = 0;

			_munchie2[i]->frame = rand() % 500 + 50;
			_munchie2[i]->frameCount =  rand() % 1;
		}
         

		for (int i = 0; i < cherryCount; i++)
		{
			_cherry[i] = new cherry();
			_cherry[i]->currentFrameTime = 0;

			_cherry[i]->frame = rand() % 500 + 50;
			_cherry[i]->frameCount = rand() % 1;
		}

		for (int i = 0; i < GHOSTCOUNT; i++)
		{
			_ghosts[i] = new MovingEnemy();
			_ghosts[i]->direction = 0;
			_ghosts[i]->speed = 0.2f;
		}
		
		_pacman = new Player();
		_pacman->dead = false;
		_pacman->direction = 0;
		_pacman->frame = 0;
		_pacman->currentFrameTime = 0;
		_pacman->speedMultiplier = 1.0f;
		_paused = false;
		_pKeyDown = false;
		_startgame = true;
		_SPACEkeyDown = false;
		_maze = new maze();
		_pop = new SoundEffect();
		//Initialise important Game aspects
		Audio::Initialise();
		Graphics::Initialise(argc, argv, this, 448, 496, false, 25, 25, "Pacman", 60);
		Input::Initialise();
		// Start the Game Loop - This calls Update and Draw in game loop
		Graphics::StartGameLoop();
	}
}

Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _pacman;
	delete _pop;
	int i;
	

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchie2[i]->texture;
		delete _munchie2[i]->InvertedTexture;
	}

	for (int i = 0; i < cherryCount; i++)
	{
		delete _cherry[i]->texture;
		delete _cherry[i]->InvertedTexture;
	}
}

void Pacman::LoadContent()
{
	// Load Pacman

	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/pacman.png", true);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 16, 16);


	// Load Munchie2
	int i;

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie2[i]->texture = new Texture2D();
		_munchie2[i]->texture->Load("Textures/Munchie.tga", true);
		_munchie2[i]->Rect = new Rect(0.0f, 0.0f, 12, 12);
		_munchie2[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight())); 
	}
	// Load Cherry
	for (i = 0; i < cherryCount; i++)
	{
		_cherry[i]->texture = new Texture2D();
		_cherry[i]->texture->Load("Textures/cherry.png", true);
		_cherry[i]->Rect = new Rect(0.0f, 0.0f, 32, 32);
		_cherry[i]->position = new Vector2(100.0f, 100.0f);
	}
	
	// Load Ghosts
	for (i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->texture = new Texture2D();
		_ghosts[i]->texture->Load("Textures/GhostBlue.png", true);
		_ghosts[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 20.0f, 20.0f );
	}

	// Load Maze
	_maze->texture = new Texture2D();
	_maze->texture->Load("Textures/maze.png", true);
	_maze->position = new Vector2(0.0f, 0.0f);
	_maze->sourceRect = new Rect(0.0f, 0.0f, 448, 496);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);


	// Set Menu Paramters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(),
		Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 5.0f,
		Graphics::GetViewportHeight() / 2.0f);

	//load pop sound 
	_pop->Load("Sounds/pop.wav");

} 

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouse)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Checks if D key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::D))

	{
		_pacman->position->X += _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier; //Moves Pacman across X axis
		_pacman->direction = 4; //face right

		UpdatePacman(elapsedTime, _pacman->direction);
	}

	else if (keyboardState->IsKeyDown(Input::Keys::A))

	{
		_pacman->position->X -= _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;  //Moves Pacman back to X axis
		_pacman->direction = 5; // face left

		UpdatePacman(elapsedTime, _pacman->direction);
	}

	else if (keyboardState->IsKeyDown(Input::Keys::S))

	{
		_pacman->position->Y += _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;  //Moves Pacman down Y axis
		_pacman->direction = 7; // face down

		UpdatePacman(elapsedTime, _pacman->direction);
	}

	else if (keyboardState->IsKeyDown(Input::Keys::W))

	{
		_pacman->position->Y -= _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;  //Moves Pacman up Y axis
		_pacman->direction = 6; // face up

		UpdatePacman(elapsedTime, _pacman->direction);
	}

	else if (keyboardState->IsKeyDown(Input::Keys::C)) //TO GENERATE RANDOM CHERRY

	{

		for (int i = 0; i < cherryCount; i++) {


			_cherry[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		}
	}

	// Speed Multiplier
	if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		// Apply multiplier
		_pacman->speedMultiplier = 3.0f;
	}
	else
	{
		// Reset multiplier
		_pacman->speedMultiplier = 1.0f;
	}

	Input::MouseState* mouseState = Input::Mouse::GetState();

	for (int i = 0; i < cherryCount; i++)

	if (mouseState->LeftButton == Input::ButtonState::PRESSED)

	{
		_cherry[i]->position->X = mouseState->X;
		_cherry[i]->position->Y = mouseState->Y;

	}

}


void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// pause screen 

	if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown) // pause the game
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::P))
		_pKeyDown = false;
}

void Pacman::Menu(Input::KeyboardState* state, Input::Keys SPACEKey)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (keyboardState->IsKeyDown(Input::Keys::SPACE) && !_SPACEkeyDown) // start the game
	{
		_SPACEkeyDown = true;
		_startgame = !_startgame;
	}
	if (keyboardState->IsKeyUp(Input::Keys::SPACE))
		_SPACEkeyDown = false;

}

void Pacman::CheckViewportCollision()
{
	if (_pacman->position->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth())//checks if pacman hits right wall
	{
		_pacman->position->X = 0;
	}

	if (_pacman->position->X + _pacman->sourceRect->Width < _pacman->sourceRect->Width)//checks if pacman hits left wall  
	{
		_pacman->position->X = Graphics::GetViewportWidth() - _pacman->sourceRect->Width;
	}

	if (_pacman->position->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight())//checks if pacman hits bottom wall  
	{
		_pacman->position->Y = 0;
	}

	if (_pacman->position->Y + _pacman->sourceRect->Height < _pacman->sourceRect->Height)//checks if pacman hits top wall  
	{
		_pacman->position->Y = Graphics::GetViewportHeight() - _pacman->sourceRect->Height;
	}
}

void Pacman::UpdatePacman(int elapsedTime, int direction)
{
	_pacman->currentFrameTime += elapsedTime;

	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction; // helps to move different direction on Y axis

	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;

		if (_pacman->frame >= 2)
			_pacman->frame = 0;

		_pacman->currentFrameTime = 0;
	}
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;      // helps to move different direction on X axis


}

void Pacman::UpdateMunchie(munchie2* _munchie2,int elapsedTime)
{
	


		_munchie2->currentFrameTime += elapsedTime;
		{
			if (_munchie2->currentFrameTime > _cMunchie2FrameTime)


			{
				_munchie2->frameCount++;

				if (_munchie2->frameCount >= 2)
					_munchie2->frameCount = 0;
				_munchie2->currentFrameTime = 0;
			}


			_munchie2->Rect->X = _munchie2->Rect->Width * _munchie2->frameCount;

		}
		
	
}

void Pacman::UpdateCherry(int elapsedTime)

{
	for (int i = 0; i < cherryCount; i++)
	{
		_cherry[i]->currentFrameTime += elapsedTime;
		{
			if (_cherry[i]->currentFrameTime > _cCherryFrameTime)


			{
				_cherry[i]->frameCount++;

				if (_cherry[i]->frameCount >= 2)
					_cherry[i]->frameCount = 0;
				_cherry[i]->currentFrameTime = 0;
			}


			_cherry[i]->Rect->X = _cherry[i]->Rect->Width * _cherry[i]->frameCount;

		}
	}
}

void Pacman::CheckGhostCollisions()
{
	// Local Variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		// Populate variables with Ghost data
		bottom2 =
			_ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height;
		left2 = _ghosts[i]->position->X;
		right2 =
			_ghosts[i]->position->X + _ghosts[i]->sourceRect->Width;
		top2 = _ghosts[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2)
			&& (left1 < right2))
		{
			_pacman->dead = true;
			i = GHOSTCOUNT;
		}
	}
}

void Pacman::UpdateGhost(MovingEnemy* _ghost, int elapsedTime)
{

		if (_ghost->direction == 0) //Moves Right 
		{
			_ghost->position->X += _ghost->speed * elapsedTime;
		}
		else if (_ghost->direction == 1) //Moves Left 
		{
			_ghost->position->X -= _ghost->speed * elapsedTime;
		}

		if (_ghost->position->X + _ghost->sourceRect->Width >=
			Graphics::GetViewportWidth()) //Hits Right edge 
		{
			_ghost->direction = 1; //Change direction 
		}
		else if (_ghost->position->X <= 0) //Hits left edge 
		{
			_ghost->direction = 0; //Change direction 
		}
	
}


bool Pacman::collisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int left1 = x1;
	int left2 = x2;
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	int top1 = y1;
	int top2 = y2;
	int bottom1 = y1 + height1;
	int bottom2 = y2 + height2;


	if (left1 < left2 + width2 &&
		left1 + width1 > left2 &&
		top1 < top2 + height2 &&
		top1 + height1 > top2) {

		return true;

	}

	return false; 

}

bool Pacman::Play(SoundEffect*)
{
	if (!Audio::IsInitialised())
	{
		std::cout << "Audio is nor initialised" << std::endl;
	}

	if (!_pop->IsLoaded())
	{
		std::cout << "_pop member sound effect has not loaded" << std::endl;
	}


	return true;
}

void Pacman::Update(int elapsedTime)

{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();
	
   if (!_paused && !_startgame)
	{
		Input(elapsedTime, keyboardState, mouseState);

		int i;
		for (i = 0; i < MUNCHIECOUNT; i++)
		{
			UpdateMunchie(_munchie2[i], elapsedTime);
		}

		UpdateCherry(elapsedTime);
	}

	// pause screen 

	CheckPaused(keyboardState, Input::Keys:: P );


	Menu(keyboardState, Input::Keys::SPACE);
      
    
      // check if the pacman is trying to disapper 
	
	 CheckViewportCollision();

	 // Gets the current state of the keyboard

	 //remaining code


	 if (!_paused)
	 {
		 Input(elapsedTime, keyboardState, mouseState);
		 UpdatePacman(elapsedTime, _pacman->direction);
		 UpdateGhost(_ghosts[0], elapsedTime);
		 CheckGhostCollisions();
		 
		 for (int i = 0; i < MUNCHIECOUNT; i++)
		 {
			 // Check collision with munchie
			 if (collisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _munchie2[i]->position->X, _munchie2[i]->position->Y, _munchie2[i]->Rect->Width, _munchie2[i]->Rect->Height))
			 {
				 //Collided!
				 Audio::Play(_pop);

				 _munchie2[i]->position->X = (-10.0f, -10.0f);
				 _munchie2[i]->position->Y = (-10.0f, -10.0f);

				 score++;
				 
			 }

		 }

		 for (int i = 0; i < cherryCount; i++)
		 {
			 // Check collision with munchie
			 if (collisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _cherry[i]->position->X, _cherry[i]->position->Y, _cherry[i]->Rect->Width, _cherry[i]->Rect->Height))
			 {
				 //Collided!
				 Audio::Play(_pop);
				
				 _cherry[i]->position->X = (-10.0f, -10.0f);
				 _cherry[i]->position->Y = (-10.0f, -10.0f);

			 }

		 }

	 }



}


void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << " Score " << score;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_maze->texture, _maze->position, _maze->sourceRect);
	
	if (_startgame)
	{
		std::stringstream menuStream;
		menuStream << "Press SpaceBar To Start The Game";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition,
			Color::Red);
	}
	// Draws String
	
	else if (!_startgame)
	{
		if (_paused)
		{
			std::stringstream menuStream;
			menuStream << "PAUSED!";

			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition,
				Color::Red);
		}

		else if (!_paused)
		{
			if (!_pacman->dead)
			{
				SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
			}
			
			SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

			int i;

			for (i = 0; i < MUNCHIECOUNT; i++)
			{
				SpriteBatch::Draw(_munchie2[i]->texture, _munchie2[i]->position, _munchie2[i]->Rect); // Draws munchie
			}

	        for (int i = 0; i < cherryCount; i++)
			{
				SpriteBatch::Draw(_cherry[i]->texture, _cherry[i]->position, _cherry[i]->Rect); // Draws cherry
			}

			//for (i = 0; i < MUNCHIECOUNT; i++)
			//{
			//	//munchie 2
			//	if (_munchie2[i]->frameCount == 0)

			//	{
			//		SpriteBatch::Draw(_munchie2[i]->InvertedTexture, _munchie2[i]->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

			//	}
			//	else
			//	{
			//		SpriteBatch::Draw(_munchie2[i]->texture, _munchie2[i]->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);



			//		if (_munchie2[i]->frameCount >= 60)
			//			_munchie2[i]->frameCount = 0;
			//	}
			//}
			//cherry
			/*for (int i = 0; i < cherryCount; i++)
			{
				if (_cherry[i]->frameCount == 0)

				{
					SpriteBatch::Draw(_cherry[i]->InvertedTexture, _cherry[i]->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

				}
				else
				{
					SpriteBatch::Draw(_cherry[i]->texture, _cherry[i]->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);



					if (_cherry[i]->frameCount >= 60)
						_cherry[i]->frameCount = 0;
				}

			}*/
			
			for (int i = 0; i < GHOSTCOUNT; i++)
			{
				
					//draw ghosts
					SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect);

			}

		}

	}

	if (_pacman->dead)
	{
		std::stringstream menuStream;
		menuStream << "Game over!";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition,
			Color::Red);
	}

	if (score == MUNCHIECOUNT)
	{
		std::stringstream menuStream;
		menuStream << "You Win!";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition,
			Color::Red);
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}