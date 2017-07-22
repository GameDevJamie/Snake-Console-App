#include "CSnakeGame.h"
#include "CTimer.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <ctime>

using std::cout;
using std::endl;

enum class EGameState{TITLE, LEVELSELECT, CONTROLS, RUNNING, RUNNINGALL};

void LoadTitleScreen();
void LoadLevelSelect();
void LoadControls();

void RunLevel(std::string);
void RunMultiple(std::vector<std::string>);

int main()
{
	srand(time(NULL));

	HWND hWnd = GetConsoleWindow();
	RECT r;
	GetWindowRect(hWnd, &r);
	
	MoveWindow(hWnd, r.left, r.top, 1280, 960, TRUE);

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15);

	EGameState gameState = EGameState::TITLE;

	std::string singleLevel;

	std::vector<std::string> multipleLevels;
	multipleLevels.push_back("Level1.txt");
	multipleLevels.push_back("Level2.txt");
	multipleLevels.push_back("Level3.txt");
	multipleLevels.push_back("Level4.txt");

	LoadTitleScreen();

	while (true)
	{
		switch (gameState)
		{
			case EGameState::TITLE:
				while (std::cin.get() != '\n') {}

				LoadLevelSelect();
				gameState = EGameState::LEVELSELECT;
				break;

			case EGameState::LEVELSELECT:
				if (_kbhit())
				{
					switch (_getch())
					{
						case '1':
							singleLevel = "Level1.txt";
							gameState = EGameState::RUNNING;
							break;

						case '2':
							singleLevel = "Level2.txt";
							gameState = EGameState::RUNNING;
							break;

						case '3':
							singleLevel = "Level3.txt";
							gameState = EGameState::RUNNING;
							break;

						case '4':
							singleLevel = "Level4.txt";
							gameState = EGameState::RUNNING;
							break;

						case '5':
							gameState = EGameState::RUNNINGALL;
							break;

						case '6':
							LoadControls();
							gameState = EGameState::CONTROLS;
							break;
					}
				}
				break;

			case EGameState::CONTROLS:
				if (_kbhit() && _getch() == 0x08)
				{
					LoadLevelSelect();
					gameState = EGameState::LEVELSELECT;
				}
				break;

			case EGameState::RUNNING:
				RunLevel(singleLevel);
				LoadLevelSelect();
				gameState = EGameState::LEVELSELECT;
				break;

			case EGameState::RUNNINGALL:
				RunMultiple(multipleLevels);
				LoadLevelSelect();
				gameState = EGameState::LEVELSELECT;
				break;
		}
	}
}

void LoadTitleScreen()
{
	system("cls");
	cout << endl << endl << endl << endl << endl;
	cout << "              ************************************************************************************************" << endl;
	cout << "                                                                                                              " << endl;
	cout << "                          ********    * *         *            *            *       *     ********            " << endl;
	cout << "                          *           *  *        *           * *           *     *       *                   " << endl;
	cout << "                          *           *   *       *          *   *          *   *         *                   " << endl;
	cout << "                          *           *    *      *         *     *         * *           *                   " << endl;
	cout << "                          ********    *     *     *        *       *        * *           ********            " << endl;
	cout << "                                 *    *      *    *       * ******* *       *   *         *                   " << endl;
	cout << "                                 *    *       *   *      *           *      *     *       *                   " << endl;
	cout << "                                 *    *        *  *     *             *     *      *      *                   " << endl;
	cout << "                          ********    *         * *    *               *    *        *    ********            " << endl;
	cout << "                                                                                                              " << endl;
	cout << "              ************************************************************************************************" << endl;
	cout << endl;
	cout << "                                                      PRESS ENTER TO START                                    " << endl;
}

void LoadLevelSelect()
{
	system("cls");

	cout << "1: Level 1" << endl;
	cout << "2: Level 2" << endl;
	cout << "3: Level 3" << endl;
	cout << "4: Level 4" << endl;
	cout << "5: All Levels (AI)" << endl;
	cout << endl;
	cout << "6: Controls" << endl;
}

void LoadControls()
{
	system("cls");

	cout << "Controls:" << endl;
	cout << "-Press the W Key or the Up Arrow Key to go UP" << endl;
	cout << "-Press the S Key or the Down Arrow Key to go DOWN" << endl;
	cout << "-Press the A Key or the Left Arrow Key to go LEFT" << endl;
	cout << "-Press the D Key or the Right Arrow Key to go RIGHT" << endl;
	cout << endl;
	cout << "-Press the 1-9 Keys to change the speed of the snake (9 being the fastest)" << endl;
	cout << endl;
	cout << "-Press the Backspace Key to End the Game and then Press the Backspace Key again to Return to Level Select" << endl;
	cout << endl;
	cout << endl;
	cout << "Press BackSpace To Return To Level Select!" << endl;
	
}

void RunLevel(std::string levelName)
{
	system("cls");
	bool textDisplayed = false;
	
	CSnakeGame* snakeGame = new CSnakeGame;
	CTimer* timer = new CTimer();

	snakeGame->Start(levelName);
	timer->Start();

	float deltaTime = 0.0f;

	while (snakeGame->IsRunning())
	{
		if (!snakeGame->IsGameOver())
		{
			timer->Tick();
			deltaTime += timer->GetDeltaTime();

			snakeGame->EnablePlayerInput();

			if (deltaTime > snakeGame->GetNextUpdateTime())
			{
				snakeGame->Update();
				snakeGame->Draw();

				if (!snakeGame->IsGameOver())
				{
					//Display Text
					if (snakeGame->IsAiModeActive()) cout << "AI Mode: ON " << endl;
					else cout << "AI Mode: Off" << endl;
					cout << "Score: " << snakeGame->GetScore() << " " << endl;
					cout << "Bonus: " << snakeGame->GetBonusTime() << " " << endl;
				}
				deltaTime = 0.0f;
			}
		}
		else
		{	
			if (!textDisplayed)
			{
				cout << endl << endl;
				cout << "Press the Backspace Key to Return To Level Select" << endl;
				textDisplayed = true;
			}

			if (_kbhit() && _getch() == 0x08) break;
		}
	}

	delete(snakeGame);
	delete(timer);
}

void RunMultiple(std::vector<std::string> levelList)
{
	system("cls");
	CTimer* timer = new CTimer;
	std::vector<CSnakeGame*> games;

	int xStartPos = 0;
	int yStartPos = 0;
	bool moveRight = true;

	//Set Up Games
	//...................................
	for each(std::string level in levelList)
	{
		CSnakeGame* snakeGame = new CSnakeGame;
		snakeGame->Start(level, SCoords(xStartPos, yStartPos));

		SCoords gridSize = snakeGame->GetGridSize();

		if (moveRight)
		{
			xStartPos = gridSize.x + 2;
			moveRight = false;
		}
		else
		{
			xStartPos = 0;
			yStartPos += gridSize.y + 2;
			moveRight = true;
		}

		snakeGame->SetAiMode(true);

		games.push_back(snakeGame);
	}
	//....................................

	timer->Start();
	float deltaTime = 0.0f;

	while (true)
	{
		timer->Tick();
		deltaTime += timer->GetDeltaTime();

		if (_kbhit() && _getch() == 0x08)
		{
			//Delete Games//
			//................
			auto it = games.begin();
			while (it != games.end())
			{
				delete(*it);
				it++;
			}

			delete(timer);
			return;
			//................
		}

		//Update Each Game//
		//.........................
		auto it = games.begin();
		while (it != games.end())
		{
			if (deltaTime > 0.1f)
			{
				(*it)->Update();
				(*it)->Draw();
			}
			it++;
		}
		//.........................

		if(deltaTime > 0.1f) deltaTime = 0.0f;
	}


	//Delete
	auto it = games.begin();
	while (it != games.end())
	{
		delete(*it);
		it++;
	}

	delete(timer);
}