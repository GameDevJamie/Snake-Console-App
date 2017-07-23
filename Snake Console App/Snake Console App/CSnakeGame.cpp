#include "CSnakeGame.h"
#include "Math.h"
#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>

CSnakeGame::CSnakeGame()
{
	m_IsRunning = false;
	m_IsPaused = false;
	m_IsGameOver = false;
	m_AiMode = false;

	m_SnakeHasFruit = false;

	//Points
	m_CurScore = 0;
	m_MaxBonusTime = 50;
	m_CurBonusTime = m_MaxBonusTime;

	m_NextUpdate = 1.0f;

	m_Handle = GetStdHandle(STD_OUTPUT_HANDLE);
}

CSnakeGame::~CSnakeGame()
{
	delete(m_Snake);
}


//Start/Restart the game 
bool CSnakeGame::Start(std::string levelName, const SCoords& gridStartPos)
{
	if (!this->LoadLevel(levelName)) return false;
	
	this->SpawnFruit();

	this->UpdateGrid();
	
	m_GridStartPos = gridStartPos;
	m_IsRunning = true;

	return true;
}

//Update the display
bool CSnakeGame::Draw(bool clearScreen)
{
	if (m_IsGameOver) return false;

	if (!clearScreen)
	{		
		COORD p;
		p.X = m_GridStartPos.x;
		p.Y = m_GridStartPos.y;
		SetConsoleCursorPosition(m_Handle, p);
	}
	else
	{
		//Clear old display
		system("cls");
	}
	

	for (int y = 0; y < m_GridY; y++)
	{
		for (int x = 0; x < m_GridX; x++)
		{
			std::string letter = " ";

			switch (m_Grid[y][x])
			{
			case EGridSquare::EMPTY:
				break;

			case EGridSquare::WALL:
				SetConsoleTextAttribute(m_Handle, 15);
				letter = "*";
				break;

			case EGridSquare::SNAKEHEAD:
				SetConsoleTextAttribute(m_Handle, 10);
				letter = "O";
				break;

			case EGridSquare::SNAKETAIL:
				SetConsoleTextAttribute(m_Handle, 10);
				letter = "o";
				break;

			case EGridSquare::FRUIT:
				SetConsoleTextAttribute(m_Handle, 14);
				letter = "X";
				break;
			}

			std::cout << letter;
		}

		COORD p;
		p.X = m_GridStartPos.x;
		p.Y = m_GridStartPos.y + (y + 1);
		SetConsoleCursorPosition(m_Handle, p);
	}

	return true;
}

//Update the game (Player Movement, Logic etc)
bool CSnakeGame::Update()
{
	if (m_AiMode)
	{
		//Calculate Path to Fruit
		//Store Path as array of EDirection (RIGHT, RIGHT, UP, LEFT, DOWN etc)
		//If path to fruit is found
			//Calculate path to tail from goal (To make sure we can escape)

		//If no path to fruit found or path to tail found
			//Calculate path away from fruit (i.e. stall until path to fruit is found)

		if (!m_Snake->HasPath())
		{
			m_SnakeHasFruit = false;
			this->CalculatePathForSnake(m_Fruit);
			m_SnakeUpdateCheck = 0;
		}
		else if(!m_SnakeHasFruit)
		{
			m_SnakeUpdateCheck++;
			
			if (m_SnakeUpdateCheck >= 1)
			{
				this->CalculatePathForSnake(m_Fruit);
				m_SnakeUpdateCheck = 0;
			}
		}
	}

	if (m_IsGameOver) return false;

	//Update Bonus Timer
	if (m_CurBonusTime > 0) m_CurBonusTime--;

	m_Snake->Update();

	//If Snake hits wall or Self, game over
	SCoords snakeHead = m_Snake->GetSnakePart(0);
	if (m_Grid[snakeHead.y][snakeHead.x] == EGridSquare::WALL || m_Snake->HasHitSelf())
	{
		m_IsGameOver = true;
		return false;
	}


	//If Snake eats fruit, spawn new fruit and increase tail
	if (snakeHead.x == m_Fruit.x && snakeHead.y == m_Fruit.y)
	{
		m_Snake->Grow();
		this->SpawnFruit();

		m_CurScore += (10 * (m_CurBonusTime + 1));
		m_CurBonusTime = m_MaxBonusTime;
	}

	this->UpdateGrid();

	return true;
}

//Allow the player to input commands to the snake//
void CSnakeGame::EnablePlayerInput()
{
	//Move Snake
	if (_kbhit())
	{
		switch (_getch())
		{
			case 'l':
			case 'L':
				m_AiMode = !m_AiMode;

				if (!m_AiMode)
				{
					m_Snake->DeletePath();
				}
				break;

			case 'w':
			case 'W':
				if(!m_AiMode) this->SetSnakeDirection(EDirection::UP);
				break;

			case 's':
			case 'S':
				if (!m_AiMode) this->SetSnakeDirection(EDirection::DOWN);
				break;

			case 'a':
			case 'A':
				if (!m_AiMode) this->SetSnakeDirection(EDirection::LEFT);
				break;

			case 'd':
			case 'D':
				if (!m_AiMode) this->SetSnakeDirection(EDirection::RIGHT);
				break;

			case 0x08:
				m_IsGameOver = true;
				break;

			case '1':
				m_NextUpdate = 1.0f;
				break;

			case '2':
				m_NextUpdate = 0.5f;
				break;

			case '3':
				m_NextUpdate = 0.3f;
				break;

			case '4':
				m_NextUpdate = 0.25f;
				break;

			case '5':
				m_NextUpdate = 0.2f;
				break;

			case '6':
				m_NextUpdate = 0.17f;
				break;

			case '7':
				m_NextUpdate = 0.14f;
				break;

			case '8':
				m_NextUpdate = 0.1f;
				break;

			case '9':
				m_NextUpdate = 0.01f;
				break;
		}
	}
}



//HELPER FUNCTIONS//
//-------------------------------------
bool CSnakeGame::LoadLevel(std::string levelName)
{
	std::ifstream infile;

	infile.open(levelName);

	if (!infile.is_open()) return false;

	int x, y;
	infile >> x >> y;

	m_GridX = x;
	m_GridY = y;

	x = 0;
	y = 0;

	m_Grid.push_back(std::vector<EGridSquare>());


	while (!infile.eof())
	{
		char letter;
		infile >> letter;

		if (y != m_GridY)
		{
			switch (letter)
			{
			case '*':
				m_Grid[y].push_back(EGridSquare::WALL);
				break;

			case 'O':
				m_Grid[y].push_back(EGridSquare::SNAKEHEAD);
				break;

			case 'o':
				m_Grid[y].push_back(EGridSquare::SNAKETAIL);
				break;

			case '.':
				m_Grid[y].push_back(EGridSquare::EMPTY);
				break;
			}
		}

		x++;

		if (x == m_GridX)
		{
			x = 0;
			y++;
			if (y != m_GridY) m_Grid.push_back(std::vector<EGridSquare>());
		}
	}

	infile.close();

	m_Snake = new CSnake(SCoords(m_GridX / 2, m_GridY / 2));

	return true;
}

//Keep all walls, remove snake and fruit//
void CSnakeGame::ResetGrid()
{
	for (int x = 0; x < m_GridX; x++)
	{
		for (int y = 0; y < m_GridY; y++)
		{
			if(m_Grid[y][x] != EGridSquare::WALL) m_Grid[y][x] = EGridSquare::EMPTY;
		}
	}
}

//Update the Grid with the new positions of the snake and fruit
void CSnakeGame::UpdateGrid()
{
	this->ResetGrid();

	SCoords snakeHead = m_Snake->GetSnakePart(0);
	int x = snakeHead.x;
	int y = snakeHead.y;
	m_Grid[y][x] = EGridSquare::SNAKEHEAD;

	for (int i = 1; i < m_Snake->GetSnakeSize(); i++)
	{
		SCoords snakeTail = m_Snake->GetSnakePart(i);

		x = snakeTail.x;
		y = snakeTail.y;
		m_Grid[y][x] = EGridSquare::SNAKETAIL;
	}

	m_Grid[m_Fruit.y][m_Fruit.x] = EGridSquare::FRUIT;
}


//Randomly spawn a piece of fruit on the map (Overwrites the current fruit on the grid)
void CSnakeGame::SpawnFruit()
{
	//Update Empty Square List
	m_EmptySquares.clear();

	for (int y = 0; y < m_GridY; y++)
	{
		for (int x = 0; x < m_GridX; x++)
		{
			if (m_Grid[y][x] == EGridSquare::EMPTY) m_EmptySquares.push_back(SCoords(x, y));
		}
	}

	int randIndex = Random(0, m_EmptySquares.size() - 1);

	SCoords emptySquare = m_EmptySquares[randIndex];

	m_Fruit.x = emptySquare.x;
	m_Fruit.y = emptySquare.y;

	m_Grid[m_Fruit.y][m_Fruit.x] = EGridSquare::FRUIT;
}


//PATHFINDING//
//------------------------------------------
void CSnakeGame::ClearLists()
{
	auto it = m_OpenList.begin();
	while (it != m_OpenList.end())
	{
		delete(*it);
		it++;
	}
	
	it = m_ClosedList.begin();
	while (it != m_ClosedList.end())
	{
		delete(*it);
		it++;
	}

	m_OpenList.clear();
	m_ClosedList.clear();
}

void CSnakeGame::AddToOpenList(SNode* currentNode, EDirection newDirection)
{
	int newX = currentNode->coords.x;
	int newY = currentNode->coords.y;

	switch (newDirection)
	{
		case EDirection::UP:
			newY -= 1;
			break;

		case EDirection::DOWN:
			newY += 1;
			break;

		case EDirection::LEFT:
			newX -= 1;
			break;

		case EDirection::RIGHT:
			newX += 1;
			break;
	}

	if (newX < 0 || newX >= m_GridX || newY < 0 || newY >= m_GridY) return;

	if (m_Grid[newY][newX] == EGridSquare::EMPTY || (newX == m_GoalNode.x && newY == m_GoalNode.y))
	{
		int heuristicVal = std::abs((newX + newY) - (m_GoalNode.x + m_GoalNode.y));

		SNode* newNode = new SNode(SCoords(newX, newY), currentNode->cost + 1);
		newNode->parent = currentNode;
		newNode->direction = newDirection;
		newNode->total = newNode->cost + heuristicVal;

		if (this->IsInLists(newNode)) return;

		this->SortOpenList(newNode);
	}
}

void CSnakeGame::SortOpenList(SNode* node)
{
	//INSERTION SORT
	//................................
	auto it = m_OpenList.begin();
	while (it != m_OpenList.end())
	{
		if ((*it)->total > node->total)
		{
			m_OpenList.insert(it, node);
			return;
		}
	
		it++;
	}
	
	//Node has current largest total, so add to end of list
	m_OpenList.push_back(node);
	//................................
}

void CSnakeGame::QuickSort(int left, int right)
{
	//PARTITION
	int i = left, j = right;
	int pivot = m_OpenList[(left + right) / 2]->total;

	while (i <= j) 
	{
		while (m_OpenList[i]->total < pivot) i++;
		while (m_OpenList[j]->total > pivot) j--;

		if (i <= j)
		{
			SNode* tmp = m_OpenList[i];
			m_OpenList[i] = m_OpenList[j];
			m_OpenList[j] = tmp;
			i++;
			j--;
		}
	};

	if (left < j) QuickSort(left, j);
	if (i < right) QuickSort(i, right);
}

//Check if node already exists in the open/closed list.
//If so and the newNode is cheaper to get to, remove old one and add new one
//Returns true if node was found
bool CSnakeGame::IsInLists(SNode* node)
{
	bool nodeExists = false;
	int newX = node->coords.x;
	int newY = node->coords.y;

	auto it = m_OpenList.begin();
	while (it != m_OpenList.end())
	{
		int oldX = (*it)->coords.x;
		int oldY = (*it)->coords.y;

		if (oldX == newX && oldY == newY)
		{
			if (node->cost > (*it)->cost)
			{
				delete(node);
				return true;
			}

			m_OpenList.erase(it);

			nodeExists = true;
			break;
		}

		it++;
	}

	if (!nodeExists)
	{
		auto it = m_ClosedList.begin();
		while (it != m_ClosedList.end())
		{
			int oldX = (*it)->coords.x;
			int oldY = (*it)->coords.y;

			if (oldX == newX && oldY == newY)
			{
				if (node->cost > (*it)->cost)
				{
					delete(node);
					return true;
				}

				m_ClosedList.erase(it);

				nodeExists = true;
				break;
			}

			it++;
		}
	}

	if (!nodeExists) return false;

	this->SortOpenList(node);

	return true;
}


void CSnakeGame::CalculatePathForSnake(const SCoords& goal)
{
	//Clear the lists//
	this->ClearLists();

	m_GoalNode = goal;
	SCoords start = m_Snake->GetSnakePart(0);

	SNode* currentNode = NULL;
	m_OpenList.push_front(new SNode(start, 0));

	while (!m_OpenList.empty())
	{
		currentNode = m_OpenList.front();
		m_OpenList.pop_front();

		if (currentNode->coords.x == goal.x && currentNode->coords.y == goal.y)
		{
			m_SnakeHasFruit = true;
			break;
		}

		this->AddToOpenList(currentNode, EDirection::UP);
		this->AddToOpenList(currentNode, EDirection::RIGHT);
		this->AddToOpenList(currentNode, EDirection::DOWN);
		this->AddToOpenList(currentNode, EDirection::LEFT);

		m_ClosedList.push_back(currentNode);
	}

	//Give snake the new directions//
	std::deque<EDirection> path;
	while (currentNode->parent != NULL)
	{
		path.push_front(currentNode->direction);
		currentNode = currentNode->parent;
	}

	m_Snake->SetPath(path);
}
//------------------------------------------

//-------------------------------------