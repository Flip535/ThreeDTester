
// ThreeDTesterDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CThreeDTesterDlg dialog
class CThreeDTesterDlg : public CDialog
{
// Construction
public:
	CThreeDTesterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_THREEDTESTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_xrot;
	CEdit m_yrot;
	CEdit m_zrot;
	CEdit m_Z;
	CEdit m_perspective;
	CEdit m_x;
	CEdit m_y;
	CEdit m_w;
	CEdit m_h;

	BOOL m_render;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnDeltaposXrotspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposYrotspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposZrotspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposZspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposPerspectivespin(NMHDR *pNMHDR, LRESULT *pResult);
};
