#include "LInput.h"
#include "LGlobal.h"

TVector3 LInput::GetWorldPos(float windowWidth, float windowHeight, float cameraPosX, float cameraPosY)
{
    float HalfWindowX = windowWidth * 0.5f;
    float HalfWindowY = windowHeight * 0.5f;
    TVector3 mouseLTPos = { cameraPosX - HalfWindowX,
                          cameraPosY + HalfWindowY, 1.0f };
    mouseLTPos.x = mouseLTPos.x + m_MousePos.x;
    mouseLTPos.y = mouseLTPos.y - m_MousePos.y;

    return mouseLTPos;
}

bool LInput::Init()
{
	return true;
}

bool LInput::Frame()
{
    // 클라이언트 중앙 포지션 계산 
    RECT clientRect;
    GetClientRect(LGlobal::g_hWnd, &clientRect);

    int clientCenterX = (clientRect.right - clientRect.left) / 2;
    int clientCenterY = (clientRect.bottom - clientRect.top) / 2;
    POINT point = { clientCenterX, clientCenterY };
    ClientToScreen(LGlobal::g_hWnd, &point);

    ::GetCursorPos(&m_MousePos);
    ::ScreenToClient(LGlobal::g_hWnd, &m_MousePos);

    m_vOffset.x = m_MousePos.x - m_BeforeMousePos.x;
    m_vOffset.y = m_MousePos.y - m_BeforeMousePos.y;

    if (ISEndPoint)
    {
        m_vOffset.x = 0.0f;
        m_vOffset.y = 0.0f;
        ISEndPoint = false;
    }

    for (int ikey = 0; ikey < 256; ikey++)
    {
        SHORT s = GetAsyncKeyState(ikey);
        if (s & 0x8000)
        {
            if (m_dwKeyState[ikey] == DWORD(KeyState::KEY_FREE))
            {
                m_dwKeyState[ikey] = DWORD(KeyState::KEY_PUSH);
            }
            else
            {
                m_dwKeyState[ikey] = DWORD(KeyState::KEY_HOLD);
            }
        }
        else
        {
            if (m_dwKeyState[ikey] == DWORD(KeyState::KEY_HOLD)
            ||  m_dwKeyState[ikey] == DWORD(KeyState::KEY_PUSH))
            {
                m_dwKeyState[ikey] = DWORD(KeyState::KEY_UP);
            }
            else
            {
                m_dwKeyState[ikey] = DWORD(KeyState::KEY_FREE);
            }
        }
    }
  
    if (m_dwKeyState[VK_F2] == DWORD(KeyState::KEY_PUSH))
    {
        ShowCursor(IsHideCursor);
        IsHideCursor = !IsHideCursor;
    }

    if (IsHideCursor)
    {
        if (m_MousePos.x <= clientRect.left || m_MousePos.x >= clientRect.right ||
            m_MousePos.y <= clientRect.top || m_MousePos.y >= clientRect.bottom)
        {
            SetCursorPos(point.x, point.y);
            ISEndPoint = true;
        }
    }

    m_BeforeMousePos = m_MousePos;

    //m_BeforeMousePos = m_MousePos;
    //RECT clientRect;
    //GetClientRect(LGlobal::g_hWnd, &clientRect);

    //// 마우스의 현재 스크린 위치를 얻고, 그 위치를 클라이언트 좌표로 변환합니다.
    //POINT m_MousePos;
    //GetCursorPos(&m_MousePos);
    //ScreenToClient(LGlobal::g_hWnd, &m_MousePos);

    //// 마우스 위치가 클라이언트 영역 내에 있는지 확인합니다.
    //if (m_MousePos.x >= clientRect.left && m_MousePos.x <= clientRect.right &&
    //    m_MousePos.y >= clientRect.top && m_MousePos.y <= clientRect.bottom)
    //{
    //    if (m_dwKeyState[VK_LBUTTON] == DWORD(KeyState::KEY_PUSH))
    //    {
    //        m_StartClick = true;
    //    }
    //}

    //if (m_MousePos.x >= clientRect.left && m_MousePos.x <= clientRect.right &&
    //    m_MousePos.y >= clientRect.top && m_MousePos.y <= clientRect.bottom && m_StartClick)
    //{
    //    // 마우스가 클라이언트 영역 내에 있을 때만 오프셋을 계산합니다.
    //    m_vOffset.x = m_MousePos.x - m_BeforeMousePos.x;
    //    m_vOffset.y = m_MousePos.y - m_BeforeMousePos.y;
    //    m_BeforeMousePos = m_MousePos;  // 이전 마우스 위치를 업데이트합니다.
    //}
   
	return true;
}

bool LInput::Render()
{
#ifdef _DEBUG
    /*std::wstring mousePos = std::to_wstring(m_MousePos.x);
    mousePos += L",";
    mousePos += std::to_wstring(m_MousePos.y);
    mousePos += L"\n";
    L_DebugString(mousePos.c_str());*/
#endif
	return true;
}

bool LInput::Release()
{
	return true;
}