#include "pch.h"
#include "Sample.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LCharacterIO.h"
#include "LAnimationIO.h"
#include "CharacterToolForm.h"
#include "BoneHierarchyForm.h"

bool Sample::Init()
{
	// 1인칭 카메라 테스트용 회전
	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 167.0f, 218.0f, 312.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	m_DebugCamera->Init();
	LGlobal::g_pMainCamera = m_DebugCamera.get();
	m_ModelCamera = std::make_shared<LModelCamera>();
	m_ModelCamera->SetTargetPos(TVector3(0.0f, 200.0f, 0.0f));
	m_ModelCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	
	m_SkyBox = std::make_shared<LSkyBox>();
	m_SkyBox->Set();
	m_SkyBox->Create(L"../../res/hlsl/SkyBox.hlsl", L"../../res/sky/grassenvmap1024.dds");

	LGlobal::g_MFCModel = new LSkinningModel;
	LGlobal::g_MFCModel->CreateBoneBuffer();
	D3DXMatrixRotationY(&LGlobal::g_MFCModel->m_matControl, 3.14159);

	LGlobal::g_MFCItem = new LModel;
	LGlobal::g_MFCItem->CreateBoneBuffer();
	
	for (int i = 0; i < CharacterToolForm::g_AllList[0].size(); i++)
	{
		LCharacterIO::GetInstance().CharacterRead(CharacterToolForm::g_AllList[0][i]);
	}

	for (int i = 0; i < CharacterToolForm::g_AllList[1].size(); i++)
	{
		LAnimationIO::GetInstance().AnimationRead(CharacterToolForm::g_AllList[1][i]);
	}

	for (int i = 0; i < CharacterToolForm::g_AllList[2].size(); i++)
	{
		LCharacterIO::GetInstance().ItemRead(CharacterToolForm::g_AllList[2][i]);
	}

	return true;
}

bool Sample::Frame()
{
	std::wstring objPos = L"ObjectPos: ";
	objPos += std::to_wstring(int(LGlobal::g_MFCModel->m_matControl._41));
	objPos += L"," + std::to_wstring(int(LGlobal::g_MFCModel->m_matControl._42));
	objPos += L"," + std::to_wstring(int(LGlobal::g_MFCModel->m_matControl._43));
	//LWrite::GetInstance().AddText(objPos, 0.0f, 50.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	std::wstring cameraPos = L"CameraPos: ";
	cameraPos += std::to_wstring(int(LGlobal::g_pMainCamera->m_vCameraPos.x));
	cameraPos += L"," + std::to_wstring(int(LGlobal::g_pMainCamera->m_vCameraPos.y));
	cameraPos += L"," + std::to_wstring(int(LGlobal::g_pMainCamera->m_vCameraPos.z));
	//LWrite::GetInstance().AddText(cameraPos, 0.0f, 100.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	std::wstring cameraRot = L"CameraRot: ";
	cameraRot += std::to_wstring(int(LGlobal::g_pMainCamera->m_fCameraPitch));
	cameraRot += L"," + std::to_wstring(int(LGlobal::g_pMainCamera->m_fCameraYaw));
	cameraRot += L"," + std::to_wstring(int(LGlobal::g_pMainCamera->m_fCameraRoll));
	//LWrite::GetInstance().AddText(cameraRot, 0.0f, 150.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	m_ModelCamera->SetTargetPos(TVector3(LGlobal::g_MFCModel->m_matControl._41, LGlobal::g_MFCModel->m_matControl._42 + 200.0f, LGlobal::g_MFCModel->m_matControl._43));

	LGlobal::g_MFCModel->Frame();
	LGlobal::g_MFCItem->Frame();

	if (LGlobal::g_MFCModel->m_pActionModel != nullptr && LGlobal::g_MFCItem->m_pModel != nullptr)
	{
		LGlobal::g_MFCItem->m_pModel->m_matSocket = LGlobal::g_MFCModel->m_pActionModel->m_NameMatrixMap[int(LGlobal::g_MFCModel->m_fCurrentAnimTime)][LGlobal::g_MFCItem->m_ParentBoneName];

		LGlobal::g_MFCItem->m_matControl = 
			LGlobal::g_MFCItem->m_pModel->m_matScale * LGlobal::g_MFCItem->m_pModel->m_matRotation * LGlobal::g_MFCItem->m_pModel->m_matTranslation * LGlobal::g_MFCItem->m_pModel->m_matSocket
			* LGlobal::g_MFCModel->m_matControl;
	}

	return true;
}

bool Sample::Render()
{
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilStateDisable.Get(), 1);
	m_SkyBox->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	m_SkyBox->Render();
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
	LGlobal::g_MFCModel->Render();
	LGlobal::g_MFCItem->Render();
	

	return true;
}

bool Sample::Release()
{
	return true;
}

Sample::~Sample() {}
