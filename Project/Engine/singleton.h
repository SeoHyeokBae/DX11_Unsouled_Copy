#pragma once


template<typename T>
class CSingleton
{
private:
	static T* m_This;

public:
	static T* GetInst()
	{
		if (nullptr == m_This)
		{
			m_This = new T;
		}

		return m_This;
	}

	static void Destroy()
	{
		if (nullptr != m_This)
		{
			delete m_This;
			m_This = nullptr;
		}
	}

public:
	CSingleton()
	{
		// atexit=> return값 입력값 void형으로 Destroy 함수 캐스팅
		// 프로그램 종료시 atexit 실행 
		typedef void(*FUNC_TYPE)(void);
		atexit((FUNC_TYPE)(&CSingleton::Destroy));
	}
	~CSingleton()
	{

	}
};

template<typename T>
T* CSingleton<T>::m_This = nullptr;