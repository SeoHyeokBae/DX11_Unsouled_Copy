#pragma once
#include "singleton.h"

class CLevel;

class CLevelMgr
	: public CSingleton<CLevelMgr>
{
	SINGLE(CLevelMgr);

private:
	CLevel*			m_CurLevel;

public:
	void init();
	void tick();

public:
	void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextStartState);	// Task 등록
	void ChangeLevelState(LEVEL_STATE _State);

private:
	void ChangeLevel_Task(CLevel* _NextLevel, LEVEL_STATE _NextLevelState); // TaskMgr 안에서 다음프레임 적용(실행)

public:
	CLevel* GetCurrentLevel() { return m_CurLevel; }

	friend class CTaskMgr;
};

