#include "CSnake.h"

CSnake::CSnake(SCoords snakeHead)
{
	m_Snake.push_back(snakeHead);
	m_Snake.push_back(SCoords(snakeHead.x - 1, snakeHead.y));

	m_Direction = EDirection::RIGHT;
}

CSnake::~CSnake()
{

}


//Move the snake based on its current direction
bool CSnake::Update()
{
	if (!m_Path.empty())
	{
		EDirection newDirection = m_Path.front();
		m_Path.pop_front();

		m_Direction = newDirection;
	}

	//Reposition Snake
	//.........................
	auto tmpIt = m_Snake.end() - 2;
	for (auto it = m_Snake.end() - 1; it != m_Snake.begin(); it--)
	{
		(*it).x = (*tmpIt).x;
		(*it).y = (*tmpIt).y;

		if (tmpIt == m_Snake.begin()) break;

		tmpIt--;
	}

	switch (m_Direction)
	{
		case EDirection::UP:
			m_Snake[0].y--;
			break;

		case EDirection::DOWN:
			m_Snake[0].y++;
			break;

		case EDirection::LEFT:
			m_Snake[0].x--;
			break;

		case EDirection::RIGHT:
			m_Snake[0].x++;
			break;
	}
	//.........................

	return true;
}


//Increases the size of the snake by 1
//If there is no snake, creates its head
void CSnake::Grow()
{
	//Get last Coords of end tail
	auto it = m_Snake.end() - 1;
	int x = (*it).x;
	int y = (*it).y;

	m_Snake.push_back(SCoords(x, y));
}

//Check if snake has hit it self
bool CSnake::HasHitSelf()
{
	SCoords snakeHead = m_Snake[0];
	auto it = m_Snake.begin() + 1;

	while (it != m_Snake.end())
	{
		int x = (*it).x;
		int y = (*it).y;

		if (snakeHead.x == x && snakeHead.y == y) return true;

		it++;
	}

	return false;
}