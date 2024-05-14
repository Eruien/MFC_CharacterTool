// CharacterToolForm.cpp: 구현 파일
//

#include "pch.h"
#include "CharacterTool.h"
#include "CharacterToolForm.h"
#include "LGlobal.h"
#include "LAnimationIO.h"
#include "LCharacterIO.h"

std::vector<std::vector<std::wstring>> CharacterToolForm::g_AllList;
std::vector<float> CharacterToolForm::g_matItemNumber;

// CharacterToolForm

IMPLEMENT_DYNCREATE(CharacterToolForm, CFormView)

CharacterToolForm* CharacterToolForm::CreateOne(CWnd* parent)
{
	CharacterToolForm* pForm = new CharacterToolForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), parent, 0, NULL);

	return pForm;
}

CharacterToolForm::CharacterToolForm()
	: CFormView(IDD_CharacterToolForm)
{

}

CharacterToolForm::~CharacterToolForm()
{
}

void CharacterToolForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_AnimationList);
	DDX_Text(pDX, IDC_EDIT4, m_SelectFile);
	DDX_Control(pDX, IDC_LIST2, m_CharacterList);
}

BEGIN_MESSAGE_MAP(CharacterToolForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CharacterToolForm::OnBnCharacterLoad)
	ON_BN_CLICKED(IDC_BUTTON2, &CharacterToolForm::OnBnAnimationLoad)
	ON_BN_CLICKED(IDC_BUTTON4, &CharacterToolForm::OnBnColor)
	ON_BN_CLICKED(IDC_BUTTON5, &CharacterToolForm::OnBnFont)
	ON_LBN_SELCHANGE(IDC_LIST1, &CharacterToolForm::OnLbnSelectAnimation)
	ON_LBN_SELCHANGE(IDC_LIST2, &CharacterToolForm::OnLbnSelectCharacter)
END_MESSAGE_MAP()


// CharacterToolForm 진단

#ifdef _DEBUG
void CharacterToolForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CharacterToolForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CharacterToolForm 메시지 처리기

void CharacterToolForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	g_AllList.resize(3);
	g_matItemNumber.resize(9);

	LoadCharacterFile();
	LoadAnimationFile();

	int iCount = m_AnimationList.GetCount();

	for (int i = 0; i < iCount; i++)
	{
		CString data;
		m_AnimationList.GetText(i, data);
		std::wstring filePath = L"../../res/UserFile/Animation/";
		filePath += data.GetString();
		g_AllList[1].push_back(filePath);
	}

	iCount = m_CharacterList.GetCount();

	for (int i = 0; i < iCount; i++)
	{
		CString data;
		m_CharacterList.GetText(i, data);
		std::wstring filePath = L"../../res/UserFile/Character/";
		filePath += data.GetString();
		g_AllList[0].push_back(filePath);
	}

	UpdateData(FALSE);
}

void CharacterToolForm::OnBnCharacterLoad()
{
	CFileDialog dlg(TRUE, L"bmp", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"All Files(*.*)|*.*| bmp Files(*.bmp)|*.bmp|", this);

	if (dlg.DoModal())
	{
		CString selfFilePath = dlg.GetPathName();
		CString selfFileName = dlg.GetFileName();
		CString selfFileExt = dlg.GetFileExt();

		char Drive[MAX_PATH];
		char Dir[MAX_PATH];
		char FName[MAX_PATH];
		char Ext[MAX_PATH];

		_splitpath_s(CW2A(selfFilePath), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);

		std::wstring characterLoadPath = mtw(Drive) + mtw(Dir) + mtw(FName);

		if (characterLoadPath.empty()) return;

		characterLoadPath += L".fbx";
		std::replace(characterLoadPath.begin(), characterLoadPath.end(), '\\', '/');

		LCharacterIO::GetInstance().CharacterWrite(characterLoadPath);
		m_CharacterList.ResetContent();
		LoadCharacterFile();
		AfxMessageBox(selfFileName);
	}
}

void CharacterToolForm::OnBnAnimationLoad()
{
	CFileDialog dlg(TRUE, L"bmp", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"All Files(*.*)|*.*| bmp Files(*.bmp)|*.bmp|", this);

	if (dlg.DoModal())
	{
		CString selfFilePath = dlg.GetPathName();
		CString selfFileName = dlg.GetFileName();
		CString selfFileExt = dlg.GetFileExt();

		char Drive[MAX_PATH];
		char Dir[MAX_PATH];
		char FName[MAX_PATH];
		char Ext[MAX_PATH];

		_splitpath_s(CW2A(selfFilePath), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);

		std::wstring AnimationLoadPath = mtw(Drive) + mtw(Dir) + mtw(FName);

		if (AnimationLoadPath.empty()) return;

		AnimationLoadPath += L".fbx";
		std::replace(AnimationLoadPath.begin(), AnimationLoadPath.end(), '\\', '/');
		
		LAnimationIO::GetInstance().AnimationWrite(AnimationLoadPath);
		m_AnimationList.ResetContent();
		LoadAnimationFile();
		AfxMessageBox(selfFileName);
	}
}

void CharacterToolForm::OnBnColor()
{
	CColorDialog dlg(RGB(0, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal())
	{
		COLORREF color = dlg.GetColor();
		CString data;
		data.Format(L"%u, %u, %u", GetRValue(color),
			GetGValue(color),
			GetBValue(color));
		AfxMessageBox(data);
	}
}

void CharacterToolForm::OnBnFont()
{
	CFontDialog dlg;
	if (dlg.DoModal())
	{
		CString data = dlg.GetFaceName();
		AfxMessageBox(data);
	}
}

void CharacterToolForm::LoadAnimationFile()
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
			//m_ObjectList.InsertItem(iCnt, data.cFileName, 0);
			//SYSTEMTIME st;
			////SystemTimeToFileTime(&st, &data.ftCreationTime);
			//FileTimeToSystemTime(&data.ftCreationTime, &st);

			//CString date;
			//date.AppendFormat(_T("%ld년%ld월%ld일"), st.wYear,
			//	st.wMonth, st.wDay);
			//m_ObjList.SetItemText(iCnt++, 1, date);
		}
		bFind = FindNextFile(hSearch, &data);
	}
	FindClose(hSearch);
}

void CharacterToolForm::LoadCharacterFile()
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
			//m_ObjectList.InsertItem(iCnt, data.cFileName, 0);
			//SYSTEMTIME st;
			////SystemTimeToFileTime(&st, &data.ftCreationTime);
			//FileTimeToSystemTime(&data.ftCreationTime, &st);

			//CString date;
			//date.AppendFormat(_T("%ld년%ld월%ld일"), st.wYear,
			//	st.wMonth, st.wDay);
			//m_ObjList.SetItemText(iCnt++, 1, date);
		}
		bFind = FindNextFile(hSearch, &data);
	}
	FindClose(hSearch);
}

void CharacterToolForm::OnLbnSelectAnimation()
{
	if (LGlobal::g_MFCModel->m_pModel == nullptr) return;

	CString data;
	int iIndex = m_AnimationList.GetCurSel();
	if (iIndex == LB_ERR)
	{
		return;
	}
	m_AnimationList.GetText(iIndex, data);
	m_SelectFile = data;

	char Drive[MAX_PATH];
	char Dir[MAX_PATH];
	char FName[MAX_PATH];
	char Ext[MAX_PATH];

	_splitpath_s(CW2A(data), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);

	std::wstring actionFileName = mtw(FName);
	actionFileName += L".fbx";
	LGlobal::g_MFCModel->m_pActionModel = LFbxMgr::GetInstance().GetPtr(actionFileName);
	UpdateData(FALSE);
}

void CharacterToolForm::OnLbnSelectCharacter()
{
	CString data;
	int iIndex = m_CharacterList.GetCurSel();
	if (iIndex == LB_ERR)
	{
		return;
	}
	m_CharacterList.GetText(iIndex, data);

	char Drive[MAX_PATH];
	char Dir[MAX_PATH];
	char FName[MAX_PATH];
	char Ext[MAX_PATH];

	_splitpath_s(CW2A(data), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);

	std::wstring characterFileName = mtw(FName);
	characterFileName += L".fbx";
	LGlobal::g_MFCModel->m_pModel = LFbxMgr::GetInstance().GetPtr(characterFileName);
	UpdateData(FALSE);
}
