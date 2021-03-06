
// searchFileView.cpp: CsearchFileView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "searchFile.h"
#endif

#include "searchFileDoc.h"
#include "searchFileView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsearchFileView

IMPLEMENT_DYNCREATE(CsearchFileView, CView)

BEGIN_MESSAGE_MAP(CsearchFileView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_WM_SIZE()
END_MESSAGE_MAP()

// CsearchFileView 构造/析构

CsearchFileView::CsearchFileView() noexcept
{
	// TODO: 在此处添加构造代码

}

CsearchFileView::~CsearchFileView()
{
}

BOOL CsearchFileView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CsearchFileView 绘图

void CsearchFileView::OnDraw(CDC* /*pDC*/)
{
	CsearchFileDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CsearchFileView 打印

BOOL CsearchFileView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CsearchFileView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CsearchFileView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CsearchFileView 诊断

#ifdef _DEBUG
void CsearchFileView::AssertValid() const
{
	CView::AssertValid();
}

void CsearchFileView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CsearchFileDoc* CsearchFileView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CsearchFileDoc)));
	return (CsearchFileDoc*)m_pDocument;
}
#endif //_DEBUG


// CsearchFileView 消息处理程序



