
// MainFrm.cpp: CMainFrame 类的实现
//

#include "stdafx.h"
#include "searchFile.h"

#include "MainFrm.h"

//
#include "CSelectView.h"
#include "CDisplayView.h"
#include "CSelfFind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(NM_A, OnMyChange)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	//以下位用户自定义代码
	MoveWindow(0, 0, 1500, 1000);//设置窗口的x和y
	CenterWindow();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_spliter.CreateStatic(this, 2, 1);

	//return CFrameWnd::OnCreateClient(lpcs, pContext);
	m_spliter.CreateView(0, 0, RUNTIME_CLASS(CSelectView), CSize(500, 150), pContext);
	m_spliter.CreateView(1, 0, RUNTIME_CLASS(CDisplayView), CSize(500, 450), pContext);
	return TRUE;
}
LRESULT CMainFrame::OnMyChange(WPARAM wParam, LPARAM lParam)
{
	CCreateContext   Context;
	switch (wParam)
	{
	case NM_A:
	{
		//CUserDlg类需要包含头文件#include "UserDlg.h"
		Context.m_pNewViewClass = RUNTIME_CLASS(CSelfFind);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView *)m_spliter.GetPane(1, 0);
		m_spliter.DeleteView(1, 0);
		m_spliter.CreateView(1, 0, RUNTIME_CLASS(CSelfFind), CSize(1300, 400), &Context);
		CSelfFind *pNewView = (CSelfFind *)m_spliter.GetPane(1, 0);
		m_spliter.RecalcLayout();
		pNewView->OnInitialUpdate();
		m_spliter.SetActivePane(1, 0);
	}
	break;
	}
	return 0;
}