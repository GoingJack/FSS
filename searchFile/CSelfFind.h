#pragma once


// CSelfFind 对话框
//定义每次提取关键字前后200个字节
#define FINDINTVALUE 100
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
	void TraverseDir(ListInfo *myInfo);//strDir为传入的文件夹路径，vecFiles为扫描到的文件集
	BOOL isSercetFile(const CString &filename, CString &Summery);
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
	void Monitoring();
	CWinThread*  h_fastscan;
	HANDLE m_hThreadEvent;
	static unsigned long filenum;//所有扫描到的文件的个数
	static int fileindex;//所有的找出文件的个数
	afx_msg void OnBnClickedButton2();
	//Ontimer的几个数
	UINT mm;
	UINT ss;
	UINT mss;
};
typedef struct
{
	CSelfFind* sf1;
	char path[3];
	char SpecialPath[MAX_PATH];
}Transfer;
