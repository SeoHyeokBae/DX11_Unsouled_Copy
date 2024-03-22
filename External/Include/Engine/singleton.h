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
		// atexit=> return�� �Է°� void������ Destroy �Լ� ĳ����
		// ���α׷� ����� atexit ���� 
		typedef void(*FUNC_TYPE)(void);
		atexit((FUNC_TYPE)(&CSingleton::Destroy));
	}
	~CSingleton()
	{

	}
};

template<typename T>
T* CSingleton<T>::m_This = nullptr;