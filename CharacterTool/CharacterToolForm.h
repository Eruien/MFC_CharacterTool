#pragma once
#include <vector>
#include <string>
// CharacterToolForm 폼 보기

class CharacterToolForm : public CFormView
{
	DECLARE_DYNCREATE(CharacterToolForm)
public:
	static CharacterToolForm* CreateOne(CWnd* parent);
protected:
	CharacterToolForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
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


