﻿// CharacterToolPane.cpp: 구현 파일
//

#include "pch.h"
#include "CharacterTool.h"
#include "CharacterToolPane.h"


// CharacterToolPane

IMPLEMENT_DYNAMIC(CharacterToolPane, CDockablePane)

CharacterToolPane::CharacterToolPane()
{

}

CharacterToolPane::~CharacterToolPane()
{
}


BEGIN_MESSAGE_MAP(CharacterToolPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// CharacterToolPane 메시지 처리기




int CharacterToolPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_wndForm = CharacterToolForm::CreateOne(this);

	return 0;
}


void CharacterToolPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_wndForm)
	{
		m_wndForm->SetWindowPos(
			NULL, 0, 0, cx, cy,
			SWP_NOZORDER);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


int CharacterToolPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CFrameWnd* pParentFrame = GetParentFrame();

	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

	return MA_NOACTIVATE;
}
