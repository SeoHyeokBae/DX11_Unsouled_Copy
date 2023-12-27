#include "pch.h"
#include "CGC.h"

#include "CEntity.h""

CGC::CGC()
{

}

CGC::~CGC()
{
	Delete_Vec(m_vecEntity);
}

// 나중에 필요한 위치에 사용
void CGC::tick()
{
	if (100 < m_vecEntity.size())
	{
		Delete_Vec(m_vecEntity);
	}
}