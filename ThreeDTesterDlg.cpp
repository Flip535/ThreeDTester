
// ThreeDTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThreeDTester.h"
#include "ThreeDTesterDlg.h"

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CThreeDTesterDlg dialog




CThreeDTesterDlg::CThreeDTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThreeDTesterDlg::IDD, pParent)
	,m_render(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThreeDTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XROT, m_xrot);
	DDX_Control(pDX, IDC_YROT, m_yrot);
	DDX_Control(pDX, IDC_ZROT, m_zrot);
	DDX_Control(pDX, IDC_Z, m_Z);
	DDX_Control(pDX, IDC_PERSPECTIVE, m_perspective);
	DDX_Control(pDX, IDC_X, m_x);
	DDX_Control(pDX, IDC_Y, m_y);
	DDX_Control(pDX, IDC_W, m_w);
	DDX_Control(pDX, IDC_H, m_h);
}

BEGIN_MESSAGE_MAP(CThreeDTesterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, &CThreeDTesterDlg::OnBnClickedButton2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_XROTSPIN, &CThreeDTesterDlg::OnDeltaposXrotspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_YROTSPIN, &CThreeDTesterDlg::OnDeltaposYrotspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ZROTSPIN, &CThreeDTesterDlg::OnDeltaposZrotspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ZSPIN, &CThreeDTesterDlg::OnDeltaposZspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PERSPECTIVESPIN, &CThreeDTesterDlg::OnDeltaposPerspectivespin)
END_MESSAGE_MAP()


// CThreeDTesterDlg message handlers

BOOL CThreeDTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetDlgItemInt(IDC_X, 36);
	SetDlgItemInt(IDC_Y, 150);
	SetDlgItemInt(IDC_W, 666);
	SetDlgItemInt(IDC_H, 36);
	SetDlgItemInt(IDC_Z, 0);

	
	SetDlgItemInt(IDC_XROT,60);
	SetDlgItemInt(IDC_YROT, 0);
	SetDlgItemInt(IDC_ZROT, 0);
	SetDlgItemInt(IDC_PERSPECTIVE,45);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void rotateThreeDPoint(int* x, int* y, int* Z, double xrot, double yrot, double zrot, int cameraX, int cameraY, int cameraZ)
{
	int xnew = cos(yrot)*(sin(zrot)*(*y-cameraY)+cos(zrot)*(*x-cameraX))-sin(yrot)*(*Z-cameraZ);
	int ynew = sin(xrot)*(cos(yrot)*(*Z-cameraZ)+sin(yrot)*(sin(zrot)*(*y-cameraY)+cos(zrot)*(*x-cameraX)))+cos(xrot)*(cos(zrot)*(*y-cameraY)-sin(zrot)*(*x-cameraX));
	int znew = cos(xrot)*(cos(yrot)*(*Z-cameraZ)+sin(yrot)*(sin(zrot)*(*y-cameraY)+cos(zrot)*(*x-cameraX)))-sin(xrot)*(cos(zrot)*(*y-cameraY)-sin(zrot)*(*x-cameraX));
	*x = xnew;
	*y = ynew;
	*Z = znew;
}

CPoint projectThreeDPoint(int x, int y, int z, int viewerX, int viewerY, int viewerZ, int perspective)
{
	CPoint p(0,0);
	double per =1.0;
	if(perspective!=0)
		per = ((double)(50000/perspective)/((double)(50000/perspective)-(double)z));
	p.x = ((double)x-(double)viewerX)*per;
	p.y = ((double)y-(double)viewerY)*per;
	return p;
}

CPoint rotateAndProjectPoint( int x, int y, int z, double xrot, double yrot, double zrot, int cameraX, int cameraY, int cameraZ, int viewerX, int viewerY, int viewerZ, int perspective)
{
	rotateThreeDPoint(&x,&y,&z,xrot,yrot,0,cameraX,cameraY,cameraZ);
	CPoint p = projectThreeDPoint(x,y,z,viewerX,viewerY,viewerZ,perspective);
	//now rotate the 2d projection (i know its wrong, but its what powerpoint does)
	CPoint p2(0,0);
	p2.x = p.x*cos(-zrot)-p.y*sin(-zrot);
	p2.y = p.y*cos(-zrot)+p.x*sin(-zrot);
	return p2;
}

void rotateAndProjectPoints(CPtrArray& arrPoints, int z, double xrot, double yrot, double zrot, int cameraX, int cameraY, int cameraZ, int viewerX, int viewerY, int viewerZ, int perspective)
{

	for(int i=0; i<arrPoints.GetCount(); i++)
	{
		CPoint* p = (CPoint*)arrPoints[i];
		*p = rotateAndProjectPoint(p->x,p->y,z,xrot,yrot,zrot,cameraX,cameraY,cameraZ,viewerX,viewerY,viewerZ,perspective);
	}
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CThreeDTesterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
		if(m_render)
		{
			CPaintDC pDC(this);
		
			CWnd* desktopWnd = CWnd::FromHandle(::GetDesktopWindow());
			CDC* desktopDC = desktopWnd->GetDC();
			int dpi = desktopDC->GetDeviceCaps(LOGPIXELSX);
			double multiple = (double)dpi/(double)72;
			desktopWnd->ReleaseDC(desktopDC);

			//get all the values from the boxes.
			int x = GetDlgItemInt(IDC_X);
			int y = GetDlgItemInt(IDC_Y);
			int w = GetDlgItemInt(IDC_W);
			int h = GetDlgItemInt(IDC_H);

			//convert to radians
			double xrot = (double)GetDlgItemInt(IDC_YROT);
			double yrot = (double)(int)GetDlgItemInt(IDC_XROT);
			double zrot = (double)(int)GetDlgItemInt(IDC_ZROT);

			xrot *= (double)0.0174532925;
			yrot *= (double)0.0174532925;
			zrot *= (double)0.0174532925;


			int perspective = GetDlgItemInt(IDC_PERSPECTIVE);
			int Z = (double)GetDlgItemInt(IDC_Z);//must be in pt, not pixels.

			// the 3D position of a point C representing the camera.
			int cameraX = 0;
			int cameraY = 0;
			int cameraZ = 0;
			//the viewer's position relative to the 2D display surface
			int viewerX = 0;
			int viewerY = 0;
			int viewerZ = 1;

			//these are the normalized rectangle points. 
			//that is, the center of the rectangle is (0,0)
			//they need to be normalized because rotation is about the (0,0) point.
			CPoint topLeft(-w/2,-h/2);
			CPoint topRight(+w/2,-h/2);
			CPoint bottomRight(+w/2,+h/2);
			CPoint bottomLeft(-w/2,+h/2);

			//stick them in an array
			CPtrArray arrPoints;
			arrPoints.Add(&topLeft);
			arrPoints.Add(&topRight);
			arrPoints.Add(&bottomRight);
			arrPoints.Add(&bottomLeft);

			//rotate the entire array of points.
			rotateAndProjectPoints(arrPoints,Z,xrot,yrot,zrot,cameraX,cameraY,cameraZ,viewerX,viewerY,viewerZ,perspective);

			int minX = 0;
			int minY = 0;
			for(int i=0; i<arrPoints.GetCount(); i++)
			{
				CPoint* p = (CPoint*)arrPoints[i];
				p->x = x+w/2+p->x;
				p->y = y+h/2+p->y;

				if(i==0)
				{
					minX = ((CPoint*)arrPoints[0])->x;
					minY = ((CPoint*)arrPoints[0])->y;
				}
				else
				{
					minX = min(minX,p->x);
					minY = min(minY,p->y);
				}
			}

			int m_nOffsetX = x-minX+3;
			int m_nOffsetY = y-minY+3;

			for(int i=0; i<arrPoints.GetCount(); i++)
			{
				CPoint* p = (CPoint*)arrPoints[i];
				p->x = p->x * multiple;
				p->y = p->y * multiple;
			}

			pDC.MoveTo(topLeft.x,topLeft.y);
			pDC.LineTo(topRight.x,topRight.y);
			pDC.LineTo(bottomRight.x,bottomRight.y);
			pDC.LineTo(bottomLeft.x,bottomLeft.y);
			pDC.LineTo(topLeft.x,topLeft.y);

			//Draw the original rectangle outline last.
			pDC.MoveTo(x*multiple,y*multiple);
			pDC.LineTo(x*multiple+w*multiple,y*multiple);
			pDC.LineTo(x*multiple+w*multiple,y*multiple+h*multiple);
			pDC.LineTo(x*multiple,y*multiple+h*multiple);
			pDC.LineTo(x*multiple,y*multiple);

		}
	}
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CThreeDTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CThreeDTesterDlg::OnBnClickedButton2()
{
	m_render = TRUE;
	this->Invalidate();
}

void CThreeDTesterDlg::OnDeltaposXrotspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	this->Invalidate();
	*pResult = 0;
}

void CThreeDTesterDlg::OnDeltaposYrotspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	this->Invalidate();
}

void CThreeDTesterDlg::OnDeltaposZrotspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	this->Invalidate();
	*pResult = 0;
}

void CThreeDTesterDlg::OnDeltaposZspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	this->Invalidate();
	*pResult = 0;
}

void CThreeDTesterDlg::OnDeltaposPerspectivespin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	this->Invalidate();
	*pResult = 0;
}
