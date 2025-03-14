#include "PlayerRun.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool PlayerRun::Init()
{
    return true;
}

void PlayerRun::Process()
{
    if (m_pOwner->IsAttack)
    {
        m_pOwner->SetTransition(Event::CLICKATTACKBUTTON);
        return;
    }

    if (!m_pOwner->IsRun)
    {
        m_pOwner->SetTransition(Event::WALKSPEED);
        return;
    }

    if (!m_pOwner->IsWalk)
    {
        m_pOwner->SetTransition(Event::IDLESPEED);
        return;
    }
  
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Run_Rifle_44.fbx");
}

void PlayerRun::Release()
{

}

PlayerRun::PlayerRun(LPlayer* parent) : PlayerState(parent)
{

}
PlayerRun::~PlayerRun() {}