
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
	//ON_MESSAGE(WM_USER_NOTIFYICON, OnNotifyMsg)
	//ON_COMMAND(ID_32774, &CMainFrame::On32774)
	//ON_COMMAND(ID_32775, &CMainFrame::On32775)
	//ON_COMMAND(ID_32776, &CMainFrame::On32776)
	ON_WM_CLOSE()
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
	m_bAutoMenuEnable = FALSE;
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
	//设置托盘图标
	/*
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_notify.cbSize = sizeof(NOTIFYICONDATA);//结构体大小
	m_notify.hWnd = m_hWnd;//对应窗口
	m_notify.uID = IDR_MAINFRAME;//托盘id
	m_notify.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));//图标
	strcpy(m_notify.szTip, "SafeCheck");//鼠标移动到托盘时的提示字符
	m_notify.uCallbackMessage = WM_USER_NOTIFYICON;//处理消息
	m_notify.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; //有效标志
	Shell_NotifyIcon(NIM_ADD, &m_notify);//添加托盘
	*/
	//设置默认初始化最大化窗口
	//m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	//获取系统分辨率设置窗口大小
	int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	int cy = GetSystemMetrics(SM_CYFULLSCREEN);
	int nx = cx * 0.78;
	int ny = cy * 0.95;
	MoveWindow(0, 0, nx, ny);//设置窗口的x和y
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
	m_spliter.CreateView(0, 0, RUNTIME_CLASS(CSelectView), CSize(500, 50), pContext);
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

//托盘图标响应函数
/*
LRESULT  CMainFrame::OnNotifyMsg(WPARAM wparam, LPARAM lparam)
{
	//wParam接收的是图标的ID，而lParam接收的是鼠标的行为
	if (wparam != IDR_MAINFRAME) return  1;
	switch (lparam)
	{
	case  WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个关闭
	{
		CPoint pos;
		CMenu menu;
		////代码添加一个响应菜单
		//::GetCursorPos(&pos);
		//menu.CreatePopupMenu();//声明一个弹出式菜单
		//menu.AppendMenu(MF_STRING, NM_A, "关闭");
		//menu.TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, this);
		//menu.Detach();
		//menu.DestroyMenu();
		//从资源文件中添加一个响应菜单
		GetCursorPos(&pos);
		menu.LoadMenu(IDR_MENU2);
		SetForegroundWindow();//放置在前面
		CMenu* pmenu;    //定义右键菜单指针
		pmenu = menu.GetSubMenu(0);      //该函数取得被指定菜单激活的下拉式菜单或子菜单的句柄
		ASSERT(pmenu != NULL);
		pmenu->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN, pos.x, pos.y, this);   //在指定位置显示右键快捷菜单
		pmenu->Detach();
		pmenu->DestroyMenu();

		//CPoint point;
		//GetCursorPos(&point);

		//menu.LoadMenuA(IDR_MENU2);
		//CMenu* popup = menu.GetSubMenu(0);
		////弹出菜单显示
		//popup->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
		
	}
	break;
	case  WM_LBUTTONDBLCLK://双击左键的处理
	{
		if (IsWindowVisible())
		{
			ShowWindow(SW_HIDE);
		}
		else
		{
			ShowWindow(SW_SHOW);
		}
	}
	break;
	}
	return 0;
}

//用户登陆
void CMainFrame::On32774()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(NULL, _T("open"), "http://116.62.167.140/loginScanSelf", NULL, NULL, SW_SHOWNORMAL);
}


//管理员登陆
void CMainFrame::On32775()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(NULL, _T("open"), "http://116.62.167.140/", NULL, NULL, SW_SHOWNORMAL);
}


//退出
void CMainFrame::On32776()
{
	// TODO: 在此添加命令处理程序代码
	exit(0);
}
*/
//关闭按钮重写
void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CFrameWnd::OnClose();
	//ShowWindow(SW_HIDE);
	if (MessageBox("确定要退出吗?", "提示", MB_YESNO | MB_DEFBUTTON2) == IDYES)
	{
		CFrameWnd::OnClose();
	}
}
