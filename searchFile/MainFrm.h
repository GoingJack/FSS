
// MainFrm.h: CMainFrame 类的接口
//

#pragma once

//自定义消息
#define NM_A (WM_USER+100)
#define WM_USER_NOTIFYICON (WM_USER+101)

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar        m_wndStatusBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
private:
	CSplitterWnd m_spliter;
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//自定义消息处理函数
	afx_msg LRESULT OnMyChange(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnNotifyMsg(WPARAM wparam, LPARAM lparam);
public:
	//NOTIFYICONDATA m_notify;
	//afx_msg void On32774();
	//afx_msg void On32775();
	//afx_msg void On32776();
	afx_msg void OnClose();
};


