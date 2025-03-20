# CharacterTool
* MFC(Microsoft Foundation Class)를 이용해서 만든 캐릭터 툴
* Dockable Pane이라는 MFC에서 제공하는 UI 요소를 이용해 제작
* Dockable Pane은 패널을 드래그 하여 특정 위치에 도킹이 가능
* 캐릭터 툴은 각종 정보를 담고 있는 Form과 Form의 부모가 되어 Form을 옮겨줄 Dokable Pane으로 구성
* 다운로드 : <https://naver.me/Fy24JUn2>
  
<img src="Image/CharacterTool.gif" width="600" height="350"/>

***

* 작업 기간 : 2023. 11. 06 ~ 2024. 01. 07 (2개월)
* 인력 구성 : 1명
* 사용언어 및 개발환경 : MFC, DirectX11, C++

# Model View
* 캐릭터를 자작으로 만든 FBX Loader로 불필요한 정보를 제외하고 바이너리 파일로 변환
* 바이너리 파일에서 정점과 본 정보를 불러와 파이프라인을 거쳐 화면에 보여줌

***

* 캐릭터 불러오기
<img src="Image/CharacterLoad.gif" width="600" height="350"/>

* 애니메이션 불러오기
<img src="Image/AnimationLoad.gif" width="600" height="350"/>

<details>
<summary>Model View Form 헤더파일</summary>

```cpp
// 바이너리 캐릭터 데이터를 화면에 불러오는 역할
class CharacterToolForm : public CFormView
{
	DECLARE_DYNCREATE(CharacterToolForm)
public:
	static CharacterToolForm* CreateOne(CWnd* parent);
protected:
	CharacterToolForm();           
	virtual ~CharacterToolForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CharacterToolForm };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnCharacterLoad();
	afx_msg void OnBnAnimationLoad();
	afx_msg void OnBnColor();
	afx_msg void OnBnFont();
	afx_msg void OnLbnSelectAnimation();
	afx_msg void OnLbnSelectCharacter();
public:
	CListBox m_CharacterList;
	CListBox m_AnimationList;
	CString m_SelectFile;
public:
	void LoadAnimationFile();
	void LoadCharacterFile();
public:
	static std::vector<std::vector<std::wstring>> g_AllList;
	static std::vector<float> g_matItemNumber;
}; 
```

</details>

<details>
<summary>Model View Form 소스파일</summary>

```cpp
// 폼 생성 후에 Pane을 부모로 설정하기 위해 Pane을 부모 인자로 받음
CharacterToolForm* CharacterToolForm::CreateOne(CWnd* parent)
{
	CharacterToolForm* pForm = new CharacterToolForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), parent, 0, NULL);

	return pForm;
}

// 데이터 교환 함수
void CharacterToolForm::DoDataExchange(CDataExchange* pDX)
{
	// 데이터 교환을 위해 사용하는 기본 함수
	CFormView::DoDataExchange(pDX);
	// 폼에 있는 IDC_LIST를 각각 변수와 연결(애니메이션 리스트, 캐릭터 리스트)
	DDX_Control(pDX, IDC_LIST1, m_AnimationList);
	DDX_Text(pDX, IDC_EDIT4, m_SelectFile);
	DDX_Control(pDX, IDC_LIST2, m_CharacterList);
}

// 초기화 작업 배열의 사이즈를 정하고 캐릭터랑 애니메이션의 기본 경로 설정
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

// 리스트에 저장되어 있는 캐릭터 파일을 로드
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

// 리스트에 저장되어 있는 애니메이션 파일 로드
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

// 다른 디렉토리에서 애니메이션 파일을 로드 배열에 추가
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

// 다른 디렉토리에서 캐릭터 파일을 로드 배열에 추가
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

// 애니메이션 선택시 재생
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

// 캐릭터 선택시 모델을 불러옴
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
```

</details>

<details>
<summary>Model View Pane 헤더파일</summary>

```cpp
// 폼의 부모가 되서 여기저기 부착되는 역할
class CharacterToolPane : public CDockablePane
{
	DECLARE_DYNAMIC(CharacterToolPane)
public:
	CharacterToolForm* m_wndForm;
public:
	CharacterToolPane();
	virtual ~CharacterToolPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
}; 
```

</details>

<details>
<summary>Model View Pane 소스파일</summary>

```cpp
// 폼을 생성하고 Pane을 부모로 설정
int CharacterToolPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 기본 토대가 되는 생성 실패시 -1 반환
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Bone의 폼 생성 Bone의 Pane을 부모로 설정
	m_wndForm = CharacterToolForm::CreateOne(this);

	return 0;
}

// 폼과 Pane의 사이즈 설정
void CharacterToolPane::OnSize(UINT nType, int cx, int cy)
{
	// Pane 사이즈 설정
	CDockablePane::OnSize(nType, cx, cy);

	if (m_wndForm)
	{
		// 폼의 크기를 Pane의 크기에 맞게 변경
		m_wndForm->SetWindowPos(
			NULL, 0, 0, cx, cy,
			SWP_NOZORDER);
	}
}

// 마우스를 클릭할 때 실행
int CharacterToolPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// 부모 프레임을 가져옴
	CFrameWnd* pParentFrame = GetParentFrame();

	// 부모 프레임이나 자식 프레임을 클릭할 때 실행
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		// 위에서 언급된 이외에 마우스 클릭 시 실행
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

	return MA_NOACTIVATE;
}
```

</details>

# Item Load
* 바이너리로 된 아이템 파일을 불러옴
<img src="Image/ItemLoad.gif" width="600" height="350"/>

# Transform
* 엔진에 있는 transform 기능을 흉내를 내서 제작
* 물체의 이동, 회전, 스케일 변환이 가능

***

* 이동
<img src="Image/TransformPos.gif" width="600" height="350"/>

* 회전
<img src="Image/TransformRotation.gif" width="600" height="350"/>

* 스케일
<img src="Image/TransformScale.gif" width="600" height="350"/>

# Socket
* 캐릭터의 본을 부모로 해서 캐릭터 본의 위치를 아이템이 따라다니게 제작
<img src="Image/Bone.gif" width="600" height="350"/>

<details>
<summary>Socket Form 헤더파일</summary>
	
```cpp
// 본 계층 창 내부에 들어갈 정보들(캐릭터, 캐릭터의 본, 아이템, 아이템 Transform)
class BoneHierarchyForm : public CFormView
{
	DECLARE_DYNCREATE(BoneHierarchyForm)
public:
	static BoneHierarchyForm* CreateOne(CWnd* parent);
protected:
	BoneHierarchyForm();           
	virtual ~BoneHierarchyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BoneHierarchyForm };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	TMatrix matRotX;
	TMatrix matRotY;
	TMatrix matRotZ;
	float m_RotX = 0.0f;
	float m_RotY = 0.0f;
	float m_RotZ = 0.0f;
	float m_ScaleX = 1.0f;
	float m_ScaleY = 1.0f;
	float m_ScaleZ = 1.0f;
	CListBox m_CharacterList;
	CListBox m_Hierarchy;
	CListBox m_ItemList;
public:
	void LoadCharacterFile();
	void LoadItemFile();
public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnFileLoad();
	afx_msg void OnBnItemLoad();
	afx_msg void OnBnClickedLoadformmatrix();
	afx_msg void OnLbnSelectCharacter();
	afx_msg void OnLbnSelectBone();
	afx_msg void OnLbnSelectItem();
	void ChangePosX();
	void ChangePosY();
	void ChangePosZ();
	void ChangeRotX();
	void ChangeRotY();
	void ChangeRotZ();
	void ChangeScaleX();
	void ChangeScaleY();
	void ChangeScaleZ();
};
```

</details>

<details>
<summary>Socket Form 소스파일</summary>
	
```cpp
// 폼 생성 후에 Pane을 부모로 설정하기 위해 Pane을 부모 인자로 받음
BoneHierarchyForm* BoneHierarchyForm::CreateOne(CWnd* parent)
{
	BoneHierarchyForm* pForm = new BoneHierarchyForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), parent, 0, NULL);

	return pForm;
}

// 데이터 교환 함수
void BoneHierarchyForm::DoDataExchange(CDataExchange* pDX)
{
	// 데이터 교환을 위해 사용하는 기본 함수
	CFormView::DoDataExchange(pDX);
	// 폼에 있는 IDC_LIST를 각각 변수와 연결(캐릭터 리스트, 본 계층, 아이템 리스트)
	DDX_Control(pDX, IDC_LIST1, m_CharacterList);
	DDX_Control(pDX, IDC_LIST2, m_Hierarchy);
	DDX_Control(pDX, IDC_LIST3, m_ItemList);
}

// 캐릭터 파일을 불러와 캐릭터 리스트에 로드
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
			// 캐릭터 리스트에 캐릭터 추가
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

// 아이템 파일을 불러와 아이템 리스트에 로드
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
			// 아이템 리스트에 아이템 추가
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

// 초기화 작업(배열 크기 정하고 파일 초기 경로 설정)
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

// 리스트에 저장되어 있는 캐릭터 파일을 로드
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

// 리스트에 저장되어 있는 아이템을 로드
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

// 사용자가 캐릭터를 선택했을 경우 캐릭터를 불러오는 함수
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

// 본을 선택했을 때 본 정보를 가지고 옴
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

// 아이템을 선택했을 때 아이템 모델을 불러옴
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

// Transform 포지션 변경
void BoneHierarchyForm::ChangePosX()
{
	CString text;
	GetDlgItemText(IDC_POSX, text);
	float x = _ttof(text);
	LGlobal::g_MFCItem->m_pModel->m_matTranslation._41 = x;
	CharacterToolForm::g_matItemNumber[0] = x;
}

// Transform 포지션 변경
void BoneHierarchyForm::ChangePosY()
{
	CString text;
	GetDlgItemText(IDC_POSY, text);
	float y = _ttof(text);
	LGlobal::g_MFCItem->m_pModel->m_matTranslation._42 = y;
	CharacterToolForm::g_matItemNumber[1] = y;
}

// Transform 포지션 변경
void BoneHierarchyForm::ChangePosZ()
{
	CString text;
	GetDlgItemText(IDC_POSZ, text);
	float z = _ttof(text);
	LGlobal::g_MFCItem->m_pModel->m_matTranslation._43 = z;
	CharacterToolForm::g_matItemNumber[2] = z;
}

// Transform 회전 변경
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

// Transform 회전 변경
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

// Transform 회전 변경
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

// Transform 스케일 변경
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

// Transform 스케일 변경
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

// Transform 스케일 변경
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
```

</details>

<details>
<summary>Socket Pane 헤더파일</summary>
	
```cpp
// 폼의 부모가 되서 여기저기 부착되는 역할
class BoneHierarchyPane : public CDockablePane
{
	DECLARE_DYNAMIC(BoneHierarchyPane)
public:
	BoneHierarchyForm* m_HierarchyForm;
public:
	BoneHierarchyPane();
	virtual ~BoneHierarchyPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};
```

</details>

<details>
<summary>Socket Pane 소스파일</summary>
	
```cpp
// 폼을 생성하고 Pane을 부모로 설정
int BoneHierarchyPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 기본 토대가 되는 생성 실패시 -1 반환
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Bone의 폼 생성 Bone의 Pane을 부모로 설정
	m_HierarchyForm = BoneHierarchyForm::CreateOne(this);
	
	return 0;
}

// 폼과 Pane의 사이즈 설정
void BoneHierarchyPane::OnSize(UINT nType, int cx, int cy)
{
	// Pane 사이즈 설정
	CDockablePane::OnSize(nType, cx, cy);

	if (m_HierarchyForm)
	{
		// 폼의 크기를 Pane의 크기에 맞게 변경
		m_HierarchyForm->SetWindowPos(
			NULL, 0, 0, cx, cy,
			SWP_NOZORDER);
	}
}

// 마우스를 클릭할 때 실행
int BoneHierarchyPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	// 부모 프레임을 가져옴
	CFrameWnd* pParentFrame = GetParentFrame();
	
	// 부모 프레임이나 자식 프레임을 클릭할 때 실행
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

	// 위에서 언급된 이외에 마우스 클릭 시 실행
	return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
```

</details>

# Character Form
* 현재 있는 캐릭터와 아이템의 종류 위치를 저장하고 로드

***

* 저장
<img src="Image/RighthandFormSave.gif" width="600" height="350"/>

* 로드
<img src="Image/RighthandFormLoad.gif" width="600" height="350"/>


