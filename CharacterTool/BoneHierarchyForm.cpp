// BoneHierarchyForm.cpp: 구현 파일
//

#include "pch.h"
#include "CharacterTool.h"
#include "BoneHierarchyForm.h"
#include "LCharacterIO.h"
#include "LGlobal.h"
#include "LInput.h"
#include "CharacterToolForm.h"
#include <iomanip>
#include <algorithm>

// BoneHierarchyForm

IMPLEMENT_DYNCREATE(BoneHierarchyForm, CFormView)

BoneHierarchyForm* BoneHierarchyForm::CreateOne(CWnd* parent)
{
	BoneHierarchyForm* pForm = new BoneHierarchyForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), parent, 0, NULL);

	return pForm;
}

BoneHierarchyForm::BoneHierarchyForm()
	: CFormView(IDD_BoneHierarchyForm)
{

}

BoneHierarchyForm::~BoneHierarchyForm()
{
}

void BoneHierarchyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_CharacterList);
	DDX_Control(pDX, IDC_LIST2, m_Hierarchy);
	DDX_Control(pDX, IDC_LIST3, m_ItemList);
}

BEGIN_MESSAGE_MAP(BoneHierarchyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &BoneHierarchyForm::OnBnFileLoad)
	ON_LBN_SELCHANGE(IDC_LIST1, &BoneHierarchyForm::OnLbnSelectCharacter)
	ON_LBN_SELCHANGE(IDC_LIST2, &BoneHierarchyForm::OnLbnSelectBone)
	ON_BN_CLICKED(IDC_BUTTON2, &BoneHierarchyForm::OnBnItemLoad)
	ON_LBN_SELCHANGE(IDC_LIST3, &BoneHierarchyForm::OnLbnSelectItem)
	ON_BN_CLICKED(IDC_LoadFormMatrix, &BoneHierarchyForm::OnBnClickedLoadformmatrix)
END_MESSAGE_MAP()


// BoneHierarchyForm 진단

#ifdef _DEBUG
void BoneHierarchyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void BoneHierarchyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// BoneHierarchyForm 메시지 처리기

struct Fraction
{
	int numerator = 0;
	int denominator = 0;
};

int GCD(int x, int y) { if (y == 0) { return x; } else { return GCD(y, x % y); } }

Fraction DecimalToFraction(double decimal, int precision = 1000000) {
	int denominator = precision;
	int numerator = static_cast<int>(round(decimal * precision));
	int gcd = GCD(numerator, denominator);  // 최대공약수 계산

	return { numerator / gcd, denominator / gcd };
}

void BoneHierarchyForm::LoadCharacterFile()
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

void BoneHierarchyForm::LoadItemFile()
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

void BoneHierarchyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	LoadCharacterFile();
	LoadItemFile();
	
	CharacterToolForm::g_AllList.resize(3);
	int iCount = m_ItemList.GetCount();

	for (int i = 0; i < iCount; i++)
	{
		CString data;
		m_ItemList.GetText(i, data);
		std::wstring filePath = L"../../res/UserFile/Item/";
		filePath += data.GetString();
		CharacterToolForm::g_AllList[2].push_back(filePath);
	}
	
	UpdateData(FALSE);
}

void BoneHierarchyForm::OnBnFileLoad()
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

void BoneHierarchyForm::OnBnItemLoad()
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

		std::wstring itemLoadPath = mtw(Drive) + mtw(Dir) + mtw(FName);

		if (itemLoadPath.empty()) return;

		itemLoadPath += L".fbx";
		std::replace(itemLoadPath.begin(), itemLoadPath.end(), '\\', '/');

		LCharacterIO::GetInstance().ItemWrite(itemLoadPath);
		m_ItemList.ResetContent();
		LoadItemFile();
		AfxMessageBox(selfFileName);
	}
}

void BoneHierarchyForm::OnBnClickedLoadformmatrix()
{
	CString text;
	text.Format(_T("%.1f"), CharacterToolForm::g_matItemNumber[0]);
	SetDlgItemText(IDC_POSX, text);
	text.Format(_T("%.1f"), CharacterToolForm::g_matItemNumber[1]);
	SetDlgItemText(IDC_POSY, text);
	text.Format(_T("%.1f"), CharacterToolForm::g_matItemNumber[2]);
	SetDlgItemText(IDC_POSZ, text);

	text.Format(_T("%.1f"), CharacterToolForm::g_matItemNumber[3]);
	SetDlgItemText(IDC_RotX, text);
	text.Format(_T("%.1f"), CharacterToolForm::g_matItemNumber[4]);
	SetDlgItemText(IDC_RotY, text);
	text.Format(_T("%.1f"), CharacterToolForm::g_matItemNumber[5]);
	SetDlgItemText(IDC_RotZ, text);

	text.Format(_T("%.1f"), CharacterToolForm::g_matItemNumber[6]);
	SetDlgItemText(IDC_ScaleX, text);
	text.Format(_T("%.1f"), CharacterToolForm::g_matItemNumber[7]);
	SetDlgItemText(IDC_ScaleY, text);
	text.Format(_T("%.1f"), CharacterToolForm::g_matItemNumber[8]);
	SetDlgItemText(IDC_ScaleZ, text);
}

void BoneHierarchyForm::OnLbnSelectCharacter()
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
	std::wstring defaultAniFileName = characterFileName + L"Default";
	characterFileName += L".fbx";
	defaultAniFileName += L".fbx";
	LGlobal::g_MFCModel->m_pModel = LFbxMgr::GetInstance().GetPtr(characterFileName);
	LGlobal::g_MFCModel->m_pActionModel = LFbxMgr::GetInstance().GetPtr(defaultAniFileName);

	m_Hierarchy.ResetContent();
	for (int i = 0; i < LGlobal::g_MFCModel->m_pModel->m_pFbxNodeNameList.size(); i++)
	{
		CString cstr(LGlobal::g_MFCModel->m_pModel->m_pFbxNodeNameList[i].c_str());
		m_Hierarchy.AddString(cstr);
	}

	UpdateData(FALSE);
}

void BoneHierarchyForm::OnLbnSelectBone()
{
	if (LGlobal::g_MFCItem->m_pModel == nullptr) return;
	CString data;
	int iIndex = m_Hierarchy.GetCurSel();
	if (iIndex == LB_ERR)
	{
		return;
	}
	m_Hierarchy.GetText(iIndex, data);

	std::wstring boneName(data.GetString());
	LGlobal::g_MFCItem->m_ParentBoneName = boneName;
}

void BoneHierarchyForm::OnLbnSelectItem()
{
	CString data;
	int iIndex = m_ItemList.GetCurSel();
	if (iIndex == LB_ERR) 
	{
		return;
	}
	m_ItemList.GetText(iIndex, data);

	char Drive[MAX_PATH];
	char Dir[MAX_PATH];
	char FName[MAX_PATH];
	char Ext[MAX_PATH];

	_splitpath_s(CW2A(data), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);

	std::wstring characterFileName = mtw(FName);
	characterFileName += L".fbx";
	
	LGlobal::g_MFCItem->m_pModel = LFbxMgr::GetInstance().GetPtr(characterFileName);
	UpdateData(FALSE);
}

void BoneHierarchyForm::ChangePosX()
{
	CString text;
	GetDlgItemText(IDC_POSX, text);
	float x = _ttof(text);
	LGlobal::g_MFCItem->m_pModel->m_matTranslation._41 = x;
	CharacterToolForm::g_matItemNumber[0] = x;
}

void BoneHierarchyForm::ChangePosY()
{
	CString text;
	GetDlgItemText(IDC_POSY, text);
	float y = _ttof(text);
	LGlobal::g_MFCItem->m_pModel->m_matTranslation._42 = y;
	CharacterToolForm::g_matItemNumber[1] = y;
}

void BoneHierarchyForm::ChangePosZ()
{
	CString text;
	GetDlgItemText(IDC_POSZ, text);
	float z = _ttof(text);
	LGlobal::g_MFCItem->m_pModel->m_matTranslation._43 = z;
	CharacterToolForm::g_matItemNumber[2] = z;
}

void BoneHierarchyForm::ChangeRotX()
{
	CString text;
	GetDlgItemText(IDC_RotX, text);
	m_RotX = _ttof(text);
	CharacterToolForm::g_matItemNumber[3] = m_RotX;
	D3DXMatrixRotationX(&matRotX, DegreeToRadian(m_RotX));
	TMatrix rot = matRotX * matRotY * matRotZ;
	LGlobal::g_MFCItem->m_pModel->m_matRotation = rot;

}

void BoneHierarchyForm::ChangeRotY()
{
	CString text;
	GetDlgItemText(IDC_RotY, text);
	m_RotY = _ttof(text);
	CharacterToolForm::g_matItemNumber[4] = m_RotY;
	D3DXMatrixRotationY(&matRotY, DegreeToRadian(m_RotY));
	TMatrix rot = matRotX * matRotY * matRotZ;
	LGlobal::g_MFCItem->m_pModel->m_matRotation = rot;
}

void BoneHierarchyForm::ChangeRotZ()
{
	CString text;
	GetDlgItemText(IDC_RotZ, text);
	m_RotZ = _ttof(text);
	CharacterToolForm::g_matItemNumber[5] = m_RotZ;
	D3DXMatrixRotationZ(&matRotZ, DegreeToRadian(m_RotZ));
	TMatrix rot = matRotX * matRotY * matRotZ;
	LGlobal::g_MFCItem->m_pModel->m_matRotation = rot;
}

void BoneHierarchyForm::ChangeScaleX()
{

	TMatrix matScaleX;

	if (m_ScaleX < 1.0f)
	{
		Fraction fraction = DecimalToFraction(m_ScaleX);
		matScaleX._11 =  float(fraction.denominator) / float(fraction.numerator);
		LGlobal::g_MFCItem->m_pModel->m_matScale = LGlobal::g_MFCItem->m_pModel->m_matScale * matScaleX;
	}
	else
	{
		float revScaleX =  1 / m_ScaleX;
		matScaleX._11 = revScaleX;
		LGlobal::g_MFCItem->m_pModel->m_matScale = LGlobal::g_MFCItem->m_pModel->m_matScale * matScaleX;
	}

	CString text;
	GetDlgItemText(IDC_ScaleX, text);
	m_ScaleX = _ttof(text);

	if (m_ScaleX <= 0)
	{
		m_ScaleX = 1.0f;
	}

	matScaleX._11 = m_ScaleX;
	
	LGlobal::g_MFCItem->m_pModel->m_matScale = LGlobal::g_MFCItem->m_pModel->m_matScale * matScaleX;

	CharacterToolForm::g_matItemNumber[6] = m_ScaleX;
}

void BoneHierarchyForm::ChangeScaleY()
{
	TMatrix matScaleY;

	if (m_ScaleY < 1.0f)
	{
		Fraction fraction = DecimalToFraction(m_ScaleY);
		matScaleY._22 = float(fraction.denominator) / float(fraction.numerator);
		LGlobal::g_MFCItem->m_pModel->m_matScale = LGlobal::g_MFCItem->m_pModel->m_matScale * matScaleY;
	}
	else
	{
		float revScaleY = 1 / m_ScaleY;
		matScaleY._22 = revScaleY;
		LGlobal::g_MFCItem->m_pModel->m_matScale = LGlobal::g_MFCItem->m_pModel->m_matScale * matScaleY;
	}

	CString text;
	GetDlgItemText(IDC_ScaleY, text);
	m_ScaleY = _ttof(text);

	if (m_ScaleY <= 0)
	{
		m_ScaleY = 1.0f;
	}

	matScaleY._22 = m_ScaleY;

	LGlobal::g_MFCItem->m_pModel->m_matScale = LGlobal::g_MFCItem->m_pModel->m_matScale * matScaleY;

	CharacterToolForm::g_matItemNumber[7] = m_ScaleY;
}

void BoneHierarchyForm::ChangeScaleZ()
{
	TMatrix matScaleZ;

	if (m_ScaleZ < 1.0f)
	{
		Fraction fraction = DecimalToFraction(m_ScaleZ);
		matScaleZ._33 = float(fraction.denominator) / float(fraction.numerator);
		LGlobal::g_MFCItem->m_pModel->m_matScale = LGlobal::g_MFCItem->m_pModel->m_matScale * matScaleZ;
	}
	else
	{
		float revScaleY = 1 / m_ScaleZ;
		matScaleZ._33 = revScaleY;
		LGlobal::g_MFCItem->m_pModel->m_matScale = LGlobal::g_MFCItem->m_pModel->m_matScale * matScaleZ;
	}

	CString text;
	GetDlgItemText(IDC_ScaleZ, text);
	m_ScaleZ = _ttof(text);

	if (m_ScaleZ <= 0)
	{
		m_ScaleZ = 1.0f;
	}

	matScaleZ._33 = m_ScaleZ;

	LGlobal::g_MFCItem->m_pModel->m_matScale = LGlobal::g_MFCItem->m_pModel->m_matScale * matScaleZ;

	CharacterToolForm::g_matItemNumber[8] = m_ScaleZ;
}

BOOL BoneHierarchyForm::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (LGlobal::g_MFCItem->m_pModel == nullptr) return false;
		ChangeScaleX();
		ChangeScaleY();
		ChangeScaleZ();
		ChangeRotX();
		ChangeRotY();
		ChangeRotZ();
		ChangePosX();
		ChangePosY();
		ChangePosZ();
		return TRUE;  
	}

	return CFormView::PreTranslateMessage(pMsg);
}

