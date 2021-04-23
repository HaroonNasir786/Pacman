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
			_munchie2[i]->frameCount = rand() % 1;
		}

		int i;

		for (i = 0; i < cherryCount; i++)
		{
			_cherry[i] = new cherry();
			_cherry[i]->currentFrameTime = 0;

			_cherry[i]->frame = rand() % 500 + 50;
			_cherry[i]->frameCount = rand() % 1;
		}

		_pacman = new Player();
		_pacman->direction = 0;
		_pacman->frame = 0;
		_pacman->currentFrameTime = 0;
		_pacman->speedMultiplier = 1.0f;
		_paused = false;
		_pKeyDown = false;
		_startgame = true;
		_SPACEkeyDown = false;
		_cherry = new cherry();
		_cherry->frame = 0;
		_cherry->currentFrameTime = 0;
		_cherry->frameCount = 0;
		//Initialise important Game aspects
		Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25,
			"Pacman", 60);
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
	int i;

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchie2[i]->texture;
		delete _munchie2[i]->InvertedTexture;
	}
	delete _cherry->texture;
	delete _cherry->InvertedTexture;
}

void Pacman::LoadContent()
{
	// Load Pacman

	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);


	// Load Munchie2
	int i;

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie2[i]->texture = new Texture2D();
		_munchie2[i]->texture->Load("Textures/SpriteSheetMunches.png", true);
		_munchie2[i]->InvertedTexture = new Texture2D();
		_munchie2[i]->InvertedTexture->Load("Textures/SpriteSheetMunches.png", true);
		_munchie2[i]->Rect = new Rect(0.0f, 0.0f, 12, 12);
		_munchie2[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight())); 
	}
	// Load Cherry
	_cherry->texture = new Texture2D();
	_cherry->texture->Load("Textures/chherriesbig.png", true);
	_cherry->InvertedTexture = new Texture2D();
	_cherry->InvertedTexture->Load("Textures/chherriesbigSS.png", true);
	_cherry->Rect = new Rect(0.0f, 0.0f, 24, 24);
	_cherry->position = new Vector2(700.0f, 700.0f);
	
	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);


	// Set Menu Paramters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(),
		Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f,
		Graphics::GetViewportHeight() / 2.0f);
} 

void Pacman::Input(int elapsedTime, Input::KeyboardState* state)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Checks if D key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::D))

	{
		_pacman->position->X += _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier; //Moves Pacman across X axis
		_pacman->direction = 0; //face right

		UpdatePacman(elapsedTime, _pacman->direction);
	}

	else if (keyboardState->IsKeyDown(Input::Keys::A))

	{
		_pacman->position->X -= _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;  //Moves Pacman back to X axis
		_pacman->direction = 2; // face left

		UpdatePacman(elapsedTime, _pacman->direction);
	}

	else if (keyboardState->IsKeyDown(Input::Keys::S))

	{
		_pacman->position->Y += _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;  //Moves Pacman down Y axis
		_pacman->direction = 1; // face down

		UpdatePacman(elapsedTime, _pacman->direction);
	}

	else if (keyboardState->IsKeyDown(Input::Keys::W))

	{
		_pacman->position->Y -= _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;  //Moves Pacman up Y axis
		_pacman->direction = 3; // face up

		UpdatePacman(elapsedTime, _pacman->direction);
	}

	else if (keyboardState->IsKeyDown(Input::Keys::R)) //TO GENERATE RANDOM CHERRY
	
	{
		_cherry[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
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

	if (mouseState->LeftButton == Input::ButtonState::PRESSED)

	{
		_cherry->position->X = mouseState->X;
		_cherry->position->Y = mouseState->Y;

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
	int i;

	for (i = 0; i < MUNCHIECOUNT; i++)
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
}

void Pacman::UpdateCherry(int elapsedTime)
{
	_cherry[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	_cherry->currentFrameTime += elapsedTime;
	{
		if (_cherry->currentFrameTime > _cCherryFrameTime)


		{
			_cherry->frameCount++;

			if (_cherry->frameCount >= 2)
				_cherry->frameCount = 0;
			_cherry->currentFrameTime = 0;
		}


		_cherry->Rect->X = _cherry->Rect->Width * _cherry->frameCount;

	}
}

void Pacman::Update(int elapsedTime)

{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	
	if (!_paused && !_startgame)
	{
		Input(elapsedTime, keyboardState);

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
	

}


void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	
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
			SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
			SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

			int i;

			for (i = 0; i < MUNCHIECOUNT; i++)
			{
				SpriteBatch::Draw(_munchie2[i]->texture, _munchie2[i]->position, _munchie2[i]->Rect); // Draws munchie
			}

			SpriteBatch::Draw(_cherry->texture, _cherry->position, _cherry->Rect); // Draws cherry

			for (i = 0; i < MUNCHIECOUNT; i++)
			{
				//munchie 2
				if (_munchie2[i]->frameCount == 0)

				{
					SpriteBatch::Draw(_munchie2[i]->InvertedTexture, _munchie2[i]->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

				}
				else
				{
					SpriteBatch::Draw(_munchie2[i]->texture, _munchie2[i]->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);



					if (_munchie2[i]->frameCount >= 60)
						_munchie2[i]->frameCount = 0;
				}
			}
			//cherry
			if (_cherry->frameCount == 0)

			{
				SpriteBatch::Draw(_cherry->InvertedTexture, _cherry->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

			}
			else
			{
				SpriteBatch::Draw(_cherry->texture, _cherry->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);



				if (_cherry->frameCount >= 60)
					_cherry->frameCount = 0;
			}
		}

	}
	SpriteBatch::EndDraw(); // Ends Drawing
}