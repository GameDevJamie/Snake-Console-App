#include "CTimer.h"

#include <Windows.h>

//CONSTRUCTOR//
//----------------------------------
CTimer::CTimer()
{
	//Initalise base variable values
	m_SecondsPerCount = 0.0f;
	m_DeltaTime = 1.0f;

	m_BaseTime = 0;
	m_PausedTime = 0;
	m_PrevTime = 0;
	m_CurTime = 0;

	m_AppIsStopped = false;

	//Calculate the counts per second
	__int64 countsPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	m_SecondsPerCount = 1.0f / static_cast<float>(countsPerSecond);
}
//----------------------------------

//DESTRUCTOR//
//---------------------------------
CTimer::~CTimer()
{

}
//---------------------------------

void CTimer::Reset()
{
	// Get the current time
	__int64 curTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

	// Set variables to either be 0 or curTime
	m_BaseTime = curTime;
	m_PrevTime = curTime;
	m_StopTime = 0;
	m_AppIsStopped = false;
}

void CTimer::Start()
{
	// Get the time when timer is started
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	// Determine time passes between stopping and starting
	if (m_AppIsStopped)
	{
		m_PausedTime += (startTime - m_StopTime);

		// Set variables
		m_PrevTime = startTime;
		m_StopTime = 0;
		m_AppIsStopped = false;
	}
}

void CTimer::Stop()
{
	// Ensure app is not already stopped
	if (!m_AppIsStopped)
	{
		// Get current time
		__int64 curTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

		// Set variables
		m_StopTime = curTime;
		m_AppIsStopped = true;
	}
}

//Calculate time passed since last called. (Should be called every frame)
void CTimer::Tick()
{
	//If app is stopped, keep delta time at 0
	if (m_AppIsStopped)
	{
		m_DeltaTime = 0.0f;
		return;
	}

	//Get current time
	__int64 curTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

	//Set variables
	m_CurTime = curTime;

	//Calculate time passed (delta time)
	m_DeltaTime = (m_CurTime - m_PrevTime) * m_SecondsPerCount;

	//Prepare to begin counting next frame - set previuos time as current time
	m_PrevTime = m_CurTime;

	//Sometimes when CPU swaps application onto different processor or if it goes into sleep / power save
	//mode, delta time can be left with a negative number.
	//If this has happened, simply set delta time to 0
	if (m_DeltaTime < 0.0f)
	{
		m_DeltaTime = 0.0f;
	}
}