#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	AFTERIMAGESCRIPT,
	BACKGROUNDSCRIPT,
	BOSSNIUGATTCOLSCRIPT,
	BOSSNIUGSCRIPT,
	CAMERAPLAYSCRIPT,
	CHAINSYSTEMSCRIPT,
	COLTILESCRIPT,
	EFFECTSCRIPT,
	HITCOLLIDERSCRIPT,
	MISSILESCRIPT,
	MONSTERSCRIPT,
	NORMALOBJSCRIPT,
	PLAYERATTACKSCRIPT,
	PLAYERATTCOLSCRIPT,
	PLAYERHITBOX,
	PLAYERSCRIPT,
	PLAYERTOPBODYSCRIPT,
	SHADOWSCRIPT,
	TYPETILESCRIPT,
	ZOMBIESCRIPT,
	ZSORTSCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
