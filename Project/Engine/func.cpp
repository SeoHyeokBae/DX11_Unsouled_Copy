#include "pch.h"

#include "CTaskMgr.h"


void GamePlayStatic::SpawnGameObject(CGameObject* _Target, int _LayerIdx)
{
	FTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param_1 = (DWORD_PTR)_LayerIdx;
	task.Param_2 = (DWORD_PTR)_Target;
	CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DestroyGameObject(CGameObject* _Target)
{
	FTask task = {};
	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param_1 = (DWORD_PTR)_Target;
	CTaskMgr::GetInst()->AddTask(task);
}
