#include "pch.h"
#include "CScriptMgr.h"

#include "CAfterImageScript.h"
#include "CBackgroundScript.h"
#include "CBossNiugAttColScript.h"
#include "CBossNiugScript.h"
#include "CCameraPlayScript.h"
#include "CChainSystemScript.h"
#include "CColTileScript.h"
#include "CEffectScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CNormalObjScript.h"
#include "CPlayerAttColScript.h"
#include "CPlayerHitBox.h"
#include "CPlayerScript.h"
#include "CPlayerTopBodyScript.h"
#include "CShadowScript.h"
#include "CTypeTileScript.h"
#include "CZombieScript.h"
#include "CZSortScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAfterImageScript");
	_vec.push_back(L"CBackgroundScript");
	_vec.push_back(L"CBossNiugAttColScript");
	_vec.push_back(L"CBossNiugScript");
	_vec.push_back(L"CCameraPlayScript");
	_vec.push_back(L"CChainSystemScript");
	_vec.push_back(L"CColTileScript");
	_vec.push_back(L"CEffectScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CNormalObjScript");
	_vec.push_back(L"CPlayerAttColScript");
	_vec.push_back(L"CPlayerHitBox");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayerTopBodyScript");
	_vec.push_back(L"CShadowScript");
	_vec.push_back(L"CTypeTileScript");
	_vec.push_back(L"CZombieScript");
	_vec.push_back(L"CZSortScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAfterImageScript" == _strScriptName)
		return new CAfterImageScript;
	if (L"CBackgroundScript" == _strScriptName)
		return new CBackgroundScript;
	if (L"CBossNiugAttColScript" == _strScriptName)
		return new CBossNiugAttColScript;
	if (L"CBossNiugScript" == _strScriptName)
		return new CBossNiugScript;
	if (L"CCameraPlayScript" == _strScriptName)
		return new CCameraPlayScript;
	if (L"CChainSystemScript" == _strScriptName)
		return new CChainSystemScript;
	if (L"CColTileScript" == _strScriptName)
		return new CColTileScript;
	if (L"CEffectScript" == _strScriptName)
		return new CEffectScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CNormalObjScript" == _strScriptName)
		return new CNormalObjScript;
	if (L"CPlayerAttColScript" == _strScriptName)
		return new CPlayerAttColScript;
	if (L"CPlayerHitBox" == _strScriptName)
		return new CPlayerHitBox;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPlayerTopBodyScript" == _strScriptName)
		return new CPlayerTopBodyScript;
	if (L"CShadowScript" == _strScriptName)
		return new CShadowScript;
	if (L"CTypeTileScript" == _strScriptName)
		return new CTypeTileScript;
	if (L"CZombieScript" == _strScriptName)
		return new CZombieScript;
	if (L"CZSortScript" == _strScriptName)
		return new CZSortScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::AFTERIMAGESCRIPT:
		return new CAfterImageScript;
		break;
	case (UINT)SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return new CBackgroundScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSNIUGATTCOLSCRIPT:
		return new CBossNiugAttColScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSNIUGSCRIPT:
		return new CBossNiugScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAPLAYSCRIPT:
		return new CCameraPlayScript;
		break;
	case (UINT)SCRIPT_TYPE::CHAINSYSTEMSCRIPT:
		return new CChainSystemScript;
		break;
	case (UINT)SCRIPT_TYPE::COLTILESCRIPT:
		return new CColTileScript;
		break;
	case (UINT)SCRIPT_TYPE::EFFECTSCRIPT:
		return new CEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::NORMALOBJSCRIPT:
		return new CNormalObjScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERATTCOLSCRIPT:
		return new CPlayerAttColScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERHITBOX:
		return new CPlayerHitBox;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERTOPBODYSCRIPT:
		return new CPlayerTopBodyScript;
		break;
	case (UINT)SCRIPT_TYPE::SHADOWSCRIPT:
		return new CShadowScript;
		break;
	case (UINT)SCRIPT_TYPE::TYPETILESCRIPT:
		return new CTypeTileScript;
		break;
	case (UINT)SCRIPT_TYPE::ZOMBIESCRIPT:
		return new CZombieScript;
		break;
	case (UINT)SCRIPT_TYPE::ZSORTSCRIPT:
		return new CZSortScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::AFTERIMAGESCRIPT:
		return L"CAfterImageScript";
		break;

	case SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return L"CBackgroundScript";
		break;

	case SCRIPT_TYPE::BOSSNIUGATTCOLSCRIPT:
		return L"CBossNiugAttColScript";
		break;

	case SCRIPT_TYPE::BOSSNIUGSCRIPT:
		return L"CBossNiugScript";
		break;

	case SCRIPT_TYPE::CAMERAPLAYSCRIPT:
		return L"CCameraPlayScript";
		break;

	case SCRIPT_TYPE::CHAINSYSTEMSCRIPT:
		return L"CChainSystemScript";
		break;

	case SCRIPT_TYPE::COLTILESCRIPT:
		return L"CColTileScript";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return L"CEffectScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::NORMALOBJSCRIPT:
		return L"CNormalObjScript";
		break;

	case SCRIPT_TYPE::PLAYERATTCOLSCRIPT:
		return L"CPlayerAttColScript";
		break;

	case SCRIPT_TYPE::PLAYERHITBOX:
		return L"CPlayerHitBox";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PLAYERTOPBODYSCRIPT:
		return L"CPlayerTopBodyScript";
		break;

	case SCRIPT_TYPE::SHADOWSCRIPT:
		return L"CShadowScript";
		break;

	case SCRIPT_TYPE::TYPETILESCRIPT:
		return L"CTypeTileScript";
		break;

	case SCRIPT_TYPE::ZOMBIESCRIPT:
		return L"CZombieScript";
		break;

	case SCRIPT_TYPE::ZSORTSCRIPT:
		return L"CZSortScript";
		break;

	}
	return nullptr;
}