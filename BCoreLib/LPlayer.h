#pragma once
#include "LModel.h"
#include "LInput.h"
#include "LFSMMgr.h"

class LPlayer;

class PlayerState
{
public:
	LPlayer* m_pOwner = nullptr;
public:
	virtual bool Init() { return true; }
	virtual	void Process() = 0;
public:
	PlayerState(LPlayer* parent) : m_pOwner(parent) {}
	virtual ~PlayerState() {}
};

class LPlayer : public LSkinningModel
{
public:
	State m_CurrentState = State::NONE;
	LFiniteStateMachine* m_pFsm = nullptr;
	PlayerState* m_pAction = nullptr;
	std::map<State, std::unique_ptr<PlayerState>> m_pActionList;
public:
	bool IsWalk = false;
	bool IsRun = false;
	bool IsAttack = false;
	bool IsMove = true;
	float m_Speed = 0.0f;
public:
	void FSM(FSMType fsmType);
	void SetTransition(Event inputEvent);
	State GetState();
	void Move();
public:
	virtual	void Process();
	bool Frame() override;
};

