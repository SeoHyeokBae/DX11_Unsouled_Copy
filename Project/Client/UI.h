#pragma once

#include "imgui.h"

class UI
{
private:
	string			m_strName;
	const string	m_strID;

	bool			m_bActive;

public:
	void SetName(const string& _name) { m_strName = _name; }
	const string& GetName() { return m_strName; }
	const string& GetID() { return m_strID; }

	void Activate() { m_bActive = true; }
	void Deactivate() { m_bActive = false; }

	bool IsActivate() { return m_bActive; }

public:
	virtual void tick();
	virtual void render();
	virtual void render_update() = 0;


public:
	UI(const string& _strName, const string& _strID);
	~UI();
};

