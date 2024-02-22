#pragma once

enum class TASK_TYPE
{
	// Param1 : Layer Index, Param2 : Object Adress
	CREATE_OBJECT,

	// Param1 : Object Adress
	DELETE_OBJECT,

	// Param1 : Level, Param2 : LEVEL_STATE
	CHANGE_LEVELSTATE,

	// Param1 : LEVEL_TYPE
	CHANGE_LEVEL,

	// Param1 : Parent Object, Param2 : Child Object
	ADD_CHILD,

	// Param1 : Parent Object, Param2 : Child Object
	DISCONNECT_PARENT,
};

struct tTask
{
	TASK_TYPE Type;
	UINT_PTR  Param_1;
	UINT_PTR  Param_2;
};


class CTaskMgr
	: public CSingleton<CTaskMgr>
{
	SINGLE(CTaskMgr);

private:
	vector<tTask>	m_vecTask;

public:
	void tick();

	void AddTask(const tTask& _Task)
	{
		m_vecTask.push_back(_Task);
	}
};

