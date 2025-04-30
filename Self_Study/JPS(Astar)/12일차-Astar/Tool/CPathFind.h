﻿#pragma once
#include "afxdialogex.h"

#include "Include.h"

// CPathFind 대화 상자

class CPathFind : public CDialog
{
	DECLARE_DYNAMIC(CPathFind)

public:
	CPathFind(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CPathFind();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CPathFind };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnListBox();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnDropFiles(HDROP hDropInfo);

public:
	CListBox			m_ListBox;
	list<IMGPATH*>		m_PathInfoList;
	afx_msg void OnDestroy();
};
