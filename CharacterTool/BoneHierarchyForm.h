#pragma once
#include "LStd.h"


// BoneHierarchyForm 폼 보기

class BoneHierarchyForm : public CFormView
{
	DECLARE_DYNCREATE(BoneHierarchyForm)
public:
	static BoneHierarchyForm* CreateOne(CWnd* parent);
protected:
	BoneHierarchyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
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


