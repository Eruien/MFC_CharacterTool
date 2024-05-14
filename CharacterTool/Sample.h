#pragma once
#include "LCore.h"
#include "LFbxMgr.h"
#include "LModel.h"
#include "LModelCamera.h"
#include "LSkyBox.h"

class Sample : public LCore
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	std::shared_ptr<LModelCamera> m_ModelCamera = nullptr;
	std::shared_ptr<LSkyBox> m_SkyBox = nullptr;
	std::shared_ptr<LSkinningModel> m_PlayerModel = nullptr;

	// character
	LFbxObj* fbxObj = nullptr;
	LFbxObj* gunObj = nullptr;
	LFbxObj* idle = nullptr;
	LFbxObj* walk = nullptr;
	LFbxObj* run = nullptr;
	LFbxObj* reload = nullptr;
	LFbxObj* fire = nullptr;
	TMatrix socket;
	TMatrix matTemp;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	virtual ~Sample();
};

