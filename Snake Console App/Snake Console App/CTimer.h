#pragma once

class CTimer
{
	private:
		float m_SecondsPerCount;	//How many counts are in a second - determined via QueryPerformance
		float m_DeltaTime;			//Time since last frame

		__int64 m_BaseTime;			//Time passed without pauseTime etc. being docked
		__int64 m_PausedTime;		//How much of BaseTime has been spent in pause (app not in focus)
		__int64 m_StopTime;			//Similar to previous
		__int64 m_PrevTime;
		__int64 m_CurTime;

		bool m_AppIsStopped;		//Stores whether the app is not in focus

	public:
		//CONSTRUCTOR/DESTRUCTOR//
		//------------------

		CTimer();
		~CTimer();

		//------------------
				
		//ACCESSORS//
		//---------------------

		//Get time passed since last frame
		float GetDeltaTime() { return m_DeltaTime; }

		//Get total time passed while timer is active
		float GetTotalTime()
		{
			if (m_AppIsStopped)
			{
				//If the app is stopped or out of focus, dont add time passed to the counter
				return static_cast<float>(((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
			}
			else
			{
				//Remove any tallied up paused time from the current and base time
				return static_cast<float>(((m_CurTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
			}
		}

		//---------------------
				
		//Resets the timer. Called before message loop
		void Reset();

		//Starts the timer. Called after unpausing app
		void Start();

		//Pauses the timer. Called when app loses focus
		void Stop();

		//Calculate time passed since last called. (Should be called every frame)
		void Tick();
};