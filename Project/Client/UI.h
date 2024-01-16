#pragma once

class UI
{
private:
	string	m_strName;
	string	m_strID;

public:
	void SetName(const string& _name) { m_strName = _name; }
	const string& GetName() { return m_strName; }

public:
	UI();
	~UI();
};

