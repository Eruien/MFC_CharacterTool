#pragma once

// ExportForm 폼 보기

class ExportForm : public CFormView
{
	DECLARE_DYNCREATE(ExportForm)
public:
	static ExportForm* CreateOne(CWnd* parent);
protected:
	ExportForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~ExportForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ExportForm };
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
	CListBox m_CharacterList;
	CListBox m_AnimationList;
	CListBox m_ItemList;
	CListBox m_CharacterFormList;
	CListBox m_AnimationFormList;
	CListBox m_ItemFormList;
	CString m_CharacterFormName;
	CString m_AnimationFormName;
	CString m_ItemFormName;
	CString m_DeleteAniName;
	CString m_DeleteItemName;
	CString m_FormNameBox;
public:
	void LoadCharacterFile();
	void LoadAnimationFile();
	void LoadItemFile();
public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLbnSelchangeCharacterlist();
	afx_msg void OnLbnSelchangeAnimationlist();
	afx_msg void OnLbnSelchangeItemlist();
	afx_msg void OnBnClickedCharacterselect();
	afx_msg void OnBnClickedAddanimation();
	afx_msg void OnBnClickedSubtractanimation();
	afx_msg void OnBnClickedAdditem();
	afx_msg void OnBnClickedSubtractitem();
	afx_msg void OnBnClickedCreateform();
	afx_msg void OnBnClickedLoadform();
};


