#pragma once

#include <list>
#include <iostream>

// CSelfFind 对话框
//定义每次提取关键字前后200个字节
//文件的各种信息结构体
typedef struct
{
	//定义消息传递的变量
	CString my_tmp;//每行前面的序号
	CString my_name;//每个文件的名字(带后缀的文件名字)
	CString my_FilePath;//文件所在的路径
	CString my_FileTitle;//文件的名字(只是文件的名字不带后缀没有路径)
	CString my_Summary;//命中文件的摘要
	CString my_showeveryFile;//当前正在扫描的文件的名字
	CString strDir;//要搜索的目录
}ListInfo;
//控件随窗口变化的结构体
typedef struct Rect
{
public:
	int Id;
	double scale[4];


	Rect()
	{
		Id = -2;
		scale[0] = 0;
		scale[1] = 0;
		scale[2] = 0;
		scale[3] = 0;
	}


	Rect(const Rect& c)
	{
		*this = c;
	}
}control;
//筛选文件信息容器保存的结构体
struct Position
{
	int FileFlag;
	CString FileName;
	CString FileKey;
	CString FileSummary;
	CString FilePath;
};
class CSelfFind : public CFormView
{
	DECLARE_DYNCREATE(CSelfFind)

protected:
	CSelfFind();           // 动态创建所使用的受保护的构造函数
	virtual ~CSelfFind();

public:
	enum { IDD = IDD_SELFFIND };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	CString m_showeveryfile;
	void TraverseDir(CString strDir);//strDir为传入的文件夹路径，vecFiles为扫描到的文件集
	BOOL isSercetFile(const CString &filename, CString &Summery, CString &NAME, int *FileFlag);
	void a2tcmd(CString filename,CString filetxt);
	CString m_getFilepath;
	unsigned long  getDisknum();
	void getDiskString(char *Diskname);
	afx_msg void OnBnClickedAlldiskscan();
	void AllDiskScan();
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedFastscan();
	void getPath(char* path1, char* path2);
	void FastScan();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32771();
	afx_msg void On32772();
	afx_msg void On32773();
	long   m_nTime;//时间
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_time;
	afx_msg void OnBnClickedButton1();
	CWinThread*  h_fastscan;
	HANDLE m_hThreadEvent;
	afx_msg void OnBnClickedButton2();
	//Ontimer的几个数
	UINT mm;
	UINT ss;
	UINT mss;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_alldiskscan;
	CButton m_fastscan;
	CRect m_rect;//获取最开始的窗口大小
	control m_control[10];//最多10个控件，这里代码可移植性没那么强，但是作者能力比较低，直接用数字代表，也没有用宏定义

//筛选功能添加的：
	std::list<Position> ls[5];//ls[0]存的时全部扫描到的文件信息，ls[1]时关键字在前100字节，类似200，500，1000；
	std::list<Position>::iterator iElement;
	int GetKeyPosition(CString words, CString Key);
	void AddListInfo(Position *po);
	CComboBox m_combo;
	afx_msg void OnCbnSelchangeCombo2();
	void RefreshListInfo(CString FilePath);
//显示时间函数
	void ShowTime();
	int s = 0, m = 0;
};
typedef struct
{
	CSelfFind* sf1;
	char path[3];
	char SpecialPath[MAX_PATH];
}Transfer;
