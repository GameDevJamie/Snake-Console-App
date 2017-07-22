#pragma once

#include "CSnake.h"
#include "SNode.h"
#include <vector>
#include <Windows.h>

class CSnakeGame
{
	private:
		enum class EGridSquare{EMPTY, SNAKEHEAD, SNAKETAIL, FRUIT, WALL};
		
		//Grid/Map
		HANDLE m_Handle;		//The Console Handle
		SCoords m_GridStartPos;	//Where to start drawing the grid
		int m_GridX;
		int m_GridY;
		std::vector<std::vector<EGridSquare>> m_Grid;
		std::vector<SCoords> m_EmptySquares;

		CSnake* m_Snake;
		int m_SnakeUpdateCheck;

		SCoords m_Fruit;

		float m_NextUpdate;	//When to next update the game

		//Score/Points//
		//------------------
		
		int m_CurScore;
		int m_CurBonusTime;
		int m_MaxBonusTime;

		//------------------

		bool m_AiMode;		//If ON, Snake controls self
		bool m_IsRunning;	//Is the game currently running
		bool m_IsPaused;	//Is the game currently paused
		bool m_IsGameOver;	//Is the game over

		//Pathfinding//
		SCoords m_GoalNode;
		std::deque<SNode*> m_OpenList;
		std::deque<SNode*> m_ClosedList;
		bool m_SnakeHasFruit;	//True when snake has path to fruit

	private:
		//Load a map from the given text file
		bool LoadLevel(std::string levelName);

		//Keep all walls, remove snake and fruit//
		void ResetGrid();

		//Update the Grid with the new positions of the snake and fruit
		void UpdateGrid();

		//Randomly spawn a piece of fruit on the map (Overwrites the current fruit on the grid)
		void SpawnFruit();

		//PATHFINDING//
		//--------------------------
		void ClearLists();
		void AddToOpenList(SNode* currentNode, EDirection newDirection);
		void SortOpenList(SNode* node);
		void QuickSort(int left, int right);
		bool IsInLists(SNode* node);
		void CalculatePathForSnake(const SCoords& goal);
		//--------------------------

	public:
		//CONSTRUCTOR/DESTRUCTOR//
		//------------------------------

		CSnakeGame();
		~CSnakeGame();

		//------------------------------


		//MUTATORS//
		//----------------------------

		//Pause the game
		void Pause() { m_IsPaused = true; }

		//Unpause the game
		void UnPause() { m_IsPaused = false; }

		//Quickly end the game
		void EndGame() { m_IsGameOver = true; }

		void SetAiMode(bool aiMode) 
		{
			m_AiMode = aiMode; 
			m_Snake->DeletePath();
		}

		//Set how long the player has to get bonus points
		void SetMaxBonusTime(int time) { m_MaxBonusTime = time; }

		void SetSnakeDirection(EDirection direction) { m_Snake->SetDirection(direction); }

		void ResetSnakePath() { m_Snake->DeletePath(); }

		void SetNextUpdateTime(const float& updateTime) { m_NextUpdate = updateTime; }
		//----------------------------


		//ACCESSORS//
		//---------------------------

		bool IsAiModeActive() { return m_AiMode; }

		bool IsRunning() { return m_IsRunning; }

		bool IsPaused() { return m_IsPaused; }

		bool IsGameOver() { return m_IsGameOver; }

		int GetScore() { return m_CurScore; }

		int GetBonusTime() { return m_CurBonusTime; }

		float GetNextUpdateTime() { return m_NextUpdate; }

		SCoords GetGridSize() { return SCoords(m_GridX, m_GridY); }

		//---------------------------


		//Start/Restart the game using the given map file
		bool Start(std::string levelName, const SCoords& gridStartPos = SCoords(0, 0));

		//Update the display
		//Returns false if game is over
		//@param: clearScreen - If true, will clear the console screen and draw the whole grid (WARNING: CAN CAUSE FLICKERING)
		bool Draw(bool clearScreen = false);

		//Update the game (Player Movement, Logic etc)
		//Returns false if game is over
		bool Update();

		//Allow the player to input commands to the snake//
		void EnablePlayerInput();
};