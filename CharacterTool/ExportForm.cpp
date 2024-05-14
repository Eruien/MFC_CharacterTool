// ExportForm.cpp: 구현 파일
//

#include "pch.h"
#include "CharacterTool.h"
#include "ExportForm.h"
#include "LExportIO.h"
#include "LGlobal.h"
#include "CharacterToolForm.h"
// ExportForm

IMPLEMENT_DYNCREATE(ExportForm, CFormView)

ExportForm* ExportForm::CreateOne(CWnd* parent)
{
	ExportForm* pForm = new ExportForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), parent, 0, NULL);

	return pForm;
}

ExportForm::ExportForm()
	: CFormView(IDD_ExportForm)
	, m_FormNameBox(_T(""))
{

}

ExportForm::~ExportForm()
{
}

void ExportForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CharacterList, m_CharacterList);
	DDX_Control(pDX, IDC_AnimationList, m_AnimationList);
	DDX_Control(pDX, IDC_ItemList, m_ItemList);
	DDX_Control(pDX, IDC_CharacterFormList, m_CharacterFormList);
	DDX_Control(pDX, IDC_AnimationFormList, m_AnimationFormList);
	DDX_Control(pDX, IDC_ItemFormList, m_ItemFormList);
	DDX_Text(pDX, IDC_FormNameBox, m_FormNameBox);
}

BEGIN_MESSAGE_MAP(ExportForm, CFormView)
	ON_LBN_SELCHANGE(IDC_CharacterList, &ExportForm::OnLbnSelchangeCharacterlist)
	ON_LBN_SELCHANGE(IDC_AnimationList, &ExportForm::OnLbnSelchangeAnimationlist)
	ON_LBN_SELCHANGE(IDC_ItemList, &ExportForm::OnLbnSelchangeItemlist)
	ON_BN_CLICKED(IDC_CharacterSelect, &ExportForm::OnBnClickedCharacterselect)
	ON_BN_CLICKED(IDC_AddAnimation, &ExportForm::OnBnClickedAddanimation)
	ON_BN_CLICKED(IDC_SubtractAnimation, &ExportForm::OnBnClickedSubtractanimation)
	ON_BN_CLICKED(IDC_AddItem, &ExportForm::OnBnClickedAdditem)
	ON_BN_CLICKED(IDC_SubtractItem, &ExportForm::OnBnClickedSubtractitem)
	ON_BN_CLICKED(IDC_CreateForm, &ExportForm::OnBnClickedCreateform)
	ON_BN_CLICKED(IDC_LoadForm, &ExportForm::OnBnClickedLoadform)
END_MESSAGE_MAP()


// ExportForm 진단

#ifdef _DEBUG
void ExportForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ExportForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ExportForm 메시지 처리기

void ExportForm::LoadCharacterFile()
{
	TCHAR path[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, path);
	_tcscat_s(path, _T("\\..\\..\\res\\UserFile\\Character\\*.*"));
	HANDLE hSearch = NULL;

	WIN32_FIND_DATA  data;
	hSearch = FindFirstFile(path, &data);

	int iCnt = 0;
	BOOL bFind = TRUE;
	while (bFind)
	{
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

		}
		else
		{
			m_CharacterList.AddString(data.cFileName);
		}
		bFind = FindNextFile(hSearch, &data);
	}
	FindClose(hSearch);
}

void ExportForm::LoadAnimationFile()
{
	TCHAR path[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, path);
	_tcscat_s(path, _T("\\..\\..\\res\\UserFile\\Animation\\*.*"));
	HANDLE hSearch = NULL;

	WIN32_FIND_DATA  data;
	hSearch = FindFirstFile(path, &data);

	int iCnt = 0;
	BOOL bFind = TRUE;
	while (bFind)
	{
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

		}
		else
		{
			m_AnimationList.AddString(data.cFileName);
		}
		bFind = FindNextFile(hSearch, &data);
	}
	FindClose(hSearch);
}

void ExportForm::LoadItemFile()
{
	TCHAR path[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, path);
	_tcscat_s(path, _T("\\..\\..\\res\\UserFile\\Item\\*.*"));
	HANDLE hSearch = NULL;

	WIN32_FIND_DATA  data;
	hSearch = FindFirstFile(path, &data);

	int iCnt = 0;
	BOOL bFind = TRUE;
	while (bFind)
	{
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
		}
		else
		{
			m_ItemList.AddString(data.cFileName);
		}
		bFind = FindNextFile(hSearch, &data);
	}
	FindClose(hSearch);
}

void ExportForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	LoadCharacterFile();
	LoadAnimationFile();
	LoadItemFile();
	UpdateData(FALSE);
}

void ExportForm::OnLbnSelchangeCharacterlist()
{
	int iIndex = m_CharacterList.GetCurSel();
	if (iIndex == LB_ERR)
	{
		return;
	}
	m_CharacterList.GetText(iIndex, m_CharacterFormName);

	UpdateData(FALSE);
}


void ExportForm::OnLbnSelchangeAnimationlist()
{
	int iIndex = m_AnimationList.GetCurSel();
	if (iIndex == LB_ERR)
	{
		return;
	}
	m_AnimationList.GetText(iIndex, m_AnimationFormName);

	UpdateData(FALSE);
}


void ExportForm::OnLbnSelchangeItemlist()
{
	int iIndex = m_ItemList.GetCurSel();
	if (iIndex == LB_ERR)
	{
		return;
	}
	m_ItemList.GetText(iIndex, m_ItemFormName);

	UpdateData(FALSE);
}

void ExportForm::OnBnClickedCharacterselect()
{
	m_CharacterFormList.ResetContent();
	m_CharacterFormList.AddString(m_CharacterFormName);
}

void ExportForm::OnBnClickedAddanimation()
{
	m_AnimationFormList.AddString(m_AnimationFormName);
}

void ExportForm::OnBnClickedSubtractanimation()
{
	int iIndex = m_AnimationFormList.GetCurSel();
	m_AnimationFormList.GetText(iIndex, m_DeleteAniName);
	m_AnimationFormList.DeleteString(iIndex);
	UpdateData(FALSE);
}

void ExportForm::OnBnClickedAdditem()
{
	m_ItemFormList.AddString(m_ItemFormName);
}

void ExportForm::OnBnClickedSubtractitem()
{
	int iIndex = m_ItemFormList.GetCurSel();
	m_ItemFormList.GetText(iIndex, m_DeleteItemName);
	m_ItemFormList.DeleteString(iIndex);
	UpdateData(FALSE);
}

void ExportForm::OnBnClickedCreateform()
{
	LExportIO::GetInstance().Reset();
	m_CharacterFormList.GetText(0, m_CharacterFormName);
	std::wstring characterFormPath = L"../../res/UserFile/Character/";
	characterFormPath += m_CharacterFormName;
	LExportIO::GetInstance().SetCharacterFbxPath(characterFormPath, LGlobal::g_MFCModel->m_matControl);

	char Drive[MAX_PATH];
	char Dir[MAX_PATH];
	char FName[MAX_PATH];
	char Ext[MAX_PATH];

	_splitpath_s(CW2A(m_CharacterFormName), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);
	std::wstring filename = mtw(FName);
	std::wstring defaultFormName = L"../../res/UserFile/Animation/" + filename + L"Default" + L".bin";
	LExportIO::GetInstance().SetDefaultPoseAnimationPath(defaultFormName);
	
	int iCount = m_AnimationFormList.GetCount();

	for (int i = 0; i < iCount; i++)
	{
		CString data;
		m_AnimationFormList.GetText(i, data);
		std::wstring filePath = L"../../res/UserFile/Animation/";
		filePath += data.GetString();
		LExportIO::GetInstance().SetAnimationPath(filePath);
	}

	LExportIO::GetInstance().ResetiPos();

	int itemCount = m_ItemFormList.GetCount();

	for (int i = 0; i < itemCount; i++)
	{
		CString data;
		m_ItemFormList.GetText(i, data);
		std::wstring filePath = L"../../res/UserFile/Item/";
		filePath += data.GetString();
		LExportIO::GetInstance().SetItem(filePath, LGlobal::g_MFCItem->m_ParentBoneName,
			LGlobal::g_MFCItem->m_pModel->m_matScale,
			LGlobal::g_MFCItem->m_pModel->m_matRotation,
			LGlobal::g_MFCItem->m_pModel->m_matTranslation);
	}
	
	std::wstring formBox = m_FormNameBox;
	LExportIO::GetInstance().ExportWrite(formBox, iCount, itemCount);
}

void ExportForm::OnBnClickedLoadform()
{
	CFileDialog dlg(TRUE, L"bmp", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"All Files(*.*)|*.*| bmp Files(*.bmp)|*.bmp|", this);

	if (dlg.DoModal())
	{
		std::wstring selfFilePath = dlg.GetPathName();
		CString selfFileName = dlg.GetFileName();
		CString selfFileExt = dlg.GetFileExt();

		m_CharacterFormList.ResetContent();
		m_AnimationFormList.ResetContent();
		m_ItemFormList.ResetContent();
		LExportIO::GetInstance().ExportRead(std::wstring(selfFileName));
		char Drive[MAX_PATH];
		char Dir[MAX_PATH];
		char FName[MAX_PATH];
		char Ext[MAX_PATH];

		_splitpath_s(LExportIO::GetInstance().m_ExportForm.characterFbxPath, Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);
		std::wstring characterPath = mtw(FName) + mtw(Ext);
		m_CharacterFormList.AddString(characterPath.c_str());

		int animationListSize = LExportIO::GetInstance().m_AnimationList.size();

		for (int i = 0; i < animationListSize; i++)
		{
			char Drive[MAX_PATH];
			char Dir[MAX_PATH];
			char FName[MAX_PATH];
			char Ext[MAX_PATH];

			_splitpath_s(wtm(LExportIO::GetInstance().m_AnimationList[i]).c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);
			std::wstring animationPath = mtw(FName) + mtw(Ext);
			m_AnimationFormList.AddString(animationPath.c_str());
		}

		int itemListSize = LExportIO::GetInstance().m_ItemList.size();

		for (int i = 0; i < itemListSize; i++)
		{
			char Drive[MAX_PATH];
			char Dir[MAX_PATH];
			char FName[MAX_PATH];
			char Ext[MAX_PATH];

			_splitpath_s(wtm(LExportIO::GetInstance().m_ItemList[i]).c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);
			std::wstring itemPath = mtw(FName) + mtw(Ext);
			m_ItemFormList.AddString(itemPath.c_str());
		}

		std::wstring characterName = mtw(FName) + L".fbx";
		LGlobal::g_MFCModel->m_pModel = LFbxMgr::GetInstance().GetPtr(characterName);

		Drive[MAX_PATH];
		Dir[MAX_PATH];
		FName[MAX_PATH];
		Ext[MAX_PATH];

		_splitpath_s(LExportIO::GetInstance().m_ExportForm.defaultposeAnimation, Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);

		std::wstring defaultAnimationName = mtw(FName) + L".fbx";
		LGlobal::g_MFCModel->m_pActionModel = LFbxMgr::GetInstance().GetPtr(defaultAnimationName);

		CString getItem;
		m_ItemFormList.GetText(0, getItem);

		_splitpath_s(CT2A(getItem), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);
		std::wstring itemPath = mtw(FName) + L".fbx";
		LGlobal::g_MFCItem->m_pModel = LFbxMgr::GetInstance().GetPtr(itemPath);
		LGlobal::g_MFCItem->m_ParentBoneName = LExportIO::GetInstance().m_ItemParentName[0];
		LGlobal::g_MFCItem->m_pModel->m_matScale = LExportIO::GetInstance().m_ItemScale[0];
		LGlobal::g_MFCItem->m_pModel->m_matRotation = LExportIO::GetInstance().m_ItemRotation[0];
		LGlobal::g_MFCItem->m_pModel->m_matTranslation = LExportIO::GetInstance().m_ItemTranslation[0];
		LGlobal::g_MFCItem->m_pModel->m_matSocket = LGlobal::g_MFCModel->m_pModel->m_NameMatrixMap[0][LGlobal::g_MFCItem->m_ParentBoneName];
		LGlobal::g_MFCItem->m_matControl = LGlobal::g_MFCItem->m_pModel->m_matScale * LGlobal::g_MFCItem->m_pModel->m_matRotation * LGlobal::g_MFCItem->m_pModel->m_matSocket *
			LGlobal::g_MFCItem->m_pModel->m_matTranslation;

		for (int i = 0; i < 9; i++)
		{
			CharacterToolForm::g_matItemNumber[i] = LExportIO::GetInstance().m_ExportForm.matNumber[i];
		}
	}
	
}

BOOL ExportForm::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		GetDlgItemText(IDC_FormNameBox, m_FormNameBox);
		return TRUE;
	}

	return CFormView::PreTranslateMessage(pMsg);
}
