#include "PlayerIdle.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool PlayerIdle::Init()
{
    return true;
}

void PlayerIdle::Process()
{
    if (m_pOwner->IsAttack)
    {
        m_pOwner->SetTransition(Event::CLICKATTACKBUTTON);
        return;
    }

    if (m_pOwner->IsRun)
    {
        m_pOwner->SetTransition(Event::RUNSPEED);
        return;
    }

    if (m_pOwner->IsWalk)
    {
        m_pOwner->SetTransition(Event::WALKSPEED);
        return;
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Idle_Rifle_189.fbx");
}

void PlayerIdle::Release()
{

}

PlayerIdle::PlayerIdle(LPlayer *parent) : PlayerState(parent)
{

}
PlayerIdle::~PlayerIdle() {}