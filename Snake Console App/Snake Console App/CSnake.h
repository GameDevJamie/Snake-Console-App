#pragma once

#include "SCoords.h"
#include <vector>
#include <deque>

enum class EDirection { LEFT, RIGHT, UP, DOWN };

class CSnake
{
	private:
		std::vector<SCoords> m_Snake;

		std::deque<EDirection> m_Path;

		EDirection m_Direction;

	public:
		//CONSTRUCTOR/DESTRUCTOR//
		//---------------------

		CSnake(SCoords snakeHead);
		~CSnake();

		//---------------------


		//MUTATORS//
		//----------------------

		void SetDirection(const EDirection& newDirection)
		{
			bool allow = false;

			switch (newDirection)
			{
				case EDirection::UP:
					if (m_Direction != EDirection::DOWN) allow = true;
					break;

				case EDirection::DOWN:
					if (m_Direction != EDirection::UP) allow = true;
					break;

				case EDirection::LEFT:
					if (m_Direction != EDirection::RIGHT) allow = true;
					break;

				case EDirection::RIGHT:
					if (m_Direction != EDirection::LEFT) allow = true;
					break;
			}

			if (allow) m_Direction = newDirection;
		}

		void SetPath(std::deque<EDirection> newPath) 
		{ 
			m_Path = newPath; 
		}

		void DeletePath() { m_Path.clear(); }
		//----------------------


		//ACCESSORS//
		//-------------------------

		//Get the co-ordinates of part of the snake (0 = head)
		SCoords GetSnakePart(int index)
		{
			if (index < 0) return m_Snake[0];
			if (index >= m_Snake.size()) return m_Snake[m_Snake.size() - 1];

			return m_Snake[index];
		}

		//Return the size of the snake (including head and tail)
		int GetSnakeSize() { return m_Snake.size(); }

		//Has the snake already got a path set
		bool HasPath() { return !m_Path.empty(); }
		//-------------------------

		//Move the snake based on its current direction
		bool Update();

		//Increases the size of the snake by 1
		void Grow();

		//Check if snake has hit it self
		bool HasHitSelf();
};