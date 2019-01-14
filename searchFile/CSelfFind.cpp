// CSelfFind.cpp: 实现文件
//

#include "stdafx.h"
#include "searchFile.h"
#include "CSelfFind.h"

//宏定义
#define NAME1 "秘密"
#define NAME2 "绝密"
#define NAME3 "机密"

//非用户自定义部分
IMPLEMENT_DYNCREATE(CSelfFind, CFormView)
CSelfFind::CSelfFind()
	: CFormView(CSelfFind::IDD)
{

}
CSelfFind::~CSelfFind()
{
}
void CSelfFind::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_SHOWTIME, m_time);
}
BEGIN_MESSAGE_MAP(CSelfFind, CFormView)
	ON_BN_CLICKED(IDC_ALLDISKSCAN, &CSelfFind::OnBnClickedAlldiskscan)
	ON_BN_CLICKED(IDC_FASTSCAN, &CSelfFind::OnBnClickedFastscan)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CSelfFind::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CSelfFind::OnNMRClickList1)
	ON_COMMAND(ID_32771, &CSelfFind::On32771)
	ON_COMMAND(ID_32772, &CSelfFind::On32772)
	ON_COMMAND(ID_32773, &CSelfFind::On32773)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CSelfFind::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSelfFind::OnBnClickedButton2)
END_MESSAGE_MAP()
// CSelfFind 诊断
#ifdef _DEBUG
void CSelfFind::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSelfFind::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

// CSelfFind 对话框
//全局变量
int isfinished = 0;//线程完成的个数
int ischecked = 0;//创建线程的个数
unsigned long filenum = 0;//所有扫描到的文件的个数
unsigned long fileindex = 0;//所有的找出文件的个数

//线程函数
//全盘扫描线程函数
//DWORD WINAPI ThreadAllDiskScan(LPVOID lp)
UINT ThreadAllDiskScan(LPVOID lp)
{
	
	
	Transfer tf = *(Transfer*)lp;
	char path[3] = { 0 };
	path[0] = tf.path[0];
	path[1] = tf.path[1];
	path[2] = tf.path[2];
	//MessageBox(NULL,path,"",NULL);

	ListInfo li[1];
	li->strDir = tf.path;
	tf.sf1->GetDlgItem(IDC_ALLDISKSCAN)->EnableWindow(FALSE);
	tf.sf1->GetDlgItem(IDC_FASTSCAN)->EnableWindow(FALSE);
	tf.sf1->TraverseDir(li);
	isfinished++;
	MessageBox(NULL, tf.path, "扫描完成", NULL);
	return 0;
}
//快速扫描的线程函数
//DWORD WINAPI ThreadFastScan(LPVOID lp)
UINT ThreadFastScan(LPVOID lp)
{
	char path[MAX_PATH] = { 0 };
	sprintf(path, "%s", ((Transfer*)lp)->SpecialPath);
	//MessageBox(NULL, path, NULL, NULL);
	ListInfo li[1];
	li->strDir = ((Transfer*)lp)->SpecialPath;
	((Transfer*)lp)->sf1->GetDlgItem(IDC_ALLDISKSCAN)->EnableWindow(FALSE);
	((Transfer*)lp)->sf1->GetDlgItem(IDC_FASTSCAN)->EnableWindow(FALSE);
	((Transfer*)lp)->sf1->TraverseDir(li);
	isfinished++;//没完成一个线程数量加一
	MessageBox(NULL, ((Transfer*)lp)->SpecialPath, "扫描完成", NULL);
	return 0;
}
//监视当前所有的线程是否结束
UINT ThreadEndTimer(LPVOID lp)
{
	Transfer tf = *(Transfer*)lp;
	while (true)
	{
		if (isfinished == ischecked)
		{
			tf.sf1->KillTimer(1);
			tf.sf1->GetDlgItem(IDC_FASTSCAN)->EnableWindow(TRUE);
			tf.sf1->GetDlgItem(IDC_ALLDISKSCAN)->EnableWindow(TRUE);
			ischecked = 0;
			isfinished = 0;
			fileindex = 0;
			filenum = 0;
			return 0;
		}
	}
}

// CSelfFind 消息处理程序
//初始化函数
void CSelfFind::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	LONG styles;
	styles = GetWindowLong(m_list.m_hWnd, GWL_STYLE);
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, styles | LVS_REPORT);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString str[] = { _T("标号"),_T("文件名字"),_T("文件路径"),_T("摘要") };
	//设置表头
	m_list.InsertColumn(0, str[0], LVCFMT_LEFT, 100, -1);
	m_list.InsertColumn(1, str[1], LVCFMT_LEFT, 500, -1);
	m_list.InsertColumn(2, str[2], LVCFMT_LEFT, 500, -1);
	m_list.InsertColumn(3, str[3], LVCFMT_LEFT, 1000, -1);
	//托盘程序

}

//主处理函数,递归遍历指定目录
void CSelfFind::TraverseDir(ListInfo *myInfo/*, std::vector<CString>& vecFiles*/)//strDir为传入的文件夹路径，vecFiles为扫描到的文件集
{
	const CString strDir = myInfo->strDir;
	CFileFind ff;
	CString tmp_strDir = strDir;
	CString Summary;
	CString tmp;
	//在路径后面添加\*.*后缀
	if (tmp_strDir.Right(1) != "\\")
		tmp_strDir += "\\";
	tmp_strDir += "*.*";
	BOOL ret = ff.FindFile(tmp_strDir);
	while (ret)
	{
		ret = ff.FindNextFile();
		if (ff.IsDirectory() && !ff.IsDots())
		{
			CString path = ff.GetFilePath();
			//TraverseDir(path, vecFiles);
			myInfo->strDir = path;
			TraverseDir(myInfo);
		}
		else if (!ff.IsDirectory() && !ff.IsDots())
		{
			CString name = ff.GetFileName();//获取带后缀的文件名
			CString FilePath = ff.GetFilePath();//获取文件路径
			CString FileTitle = ff.GetFileTitle();//获取不带后缀的文件名
			if (FilePath.IsEmpty())
			{
				return;
			}
			m_showeveryfile.Format("已扫描文件个数%u-正在扫描%s",filenum+1,FilePath);
			SetDlgItemText(IDC_SHOWEVERYFILE,m_showeveryfile);
			filenum++;
			if (name.Right(4) == ".doc" || name.Right(5) == ".docx" || name.Right(4) == ".xls" || name.Right(5) == "xlsx" || name.Right(4) == ".pdf"|| name.Right(4) == ".wps")
			{
				if (isSercetFile(FilePath, Summary))
				{
					if (!name.IsEmpty()&&!FilePath.IsEmpty()&&!Summary.IsEmpty())
					{
						tmp.Format("%u", fileindex + 1);
						m_list.InsertItem(fileindex, tmp);
						m_list.SetItemText(fileindex, 1, name);
						m_list.SetItemText(fileindex, 2, FilePath);
						m_list.SetItemText(fileindex, 3, Summary);
						fileindex += 1;
					}
				}
			}
			else
			{
			}
		}
	}
	ff.Close();
}

//主扫描函数，对找到特定格式的文件进行搜索其中是否有关键字
BOOL CSelfFind::isSercetFile(const CString &filename, CString &Summary)
{
	a2tcmd(filename);
	//查找txt中的数据
	char *tmpName[3] = { NAME1,NAME2,NAME3 };
	CString filetxt;
	filetxt.Format("%s.txt", filename);
	CStdioFile myFile[3];
	CFileException fileException;
	char *words = (char *)malloc(1024 * sizeof(char));
	for (int i = 0; i < 3; i++)
	{
		if (myFile[i].Open(filetxt, CFile::typeText | CFile::modeRead, &fileException))
		{
			{
				while (myFile[i].ReadString((LPTSTR)words, 1024) != NULL)
				{

					if (CString(words).Find(tmpName[i]) != -1)
					{
						CString dealstr = words;
						int position = dealstr.Find(tmpName[i]);
						CString leftstr;
						CString rightstr;
						//从左边position-FINDINTVALUES字节开始获取后面FINDINTVALUE个字节
						int isEnough = position - FINDINTVALUE;
						if (isEnough <= 0)
						{
							leftstr = CString(words).Mid(position - FINDINTVALUE, position);
						}
						else
						{
							leftstr = CString(words).Mid(position - FINDINTVALUE, FINDINTVALUE);
						}
						rightstr = CString(words).Mid(position, FINDINTVALUE + strlen(tmpName[i]));
						Summary = leftstr + rightstr;
						myFile[i].Close();
						remove(filetxt);
						return TRUE;
					}
				}
			}
			myFile[i].Close();
		}
	}
	remove(filetxt);
	return FALSE;
}

//将找到的特定格式的文件转化位.txt文件
void CSelfFind::a2tcmd(CString filename)
{
	//调用all2txt转化为txt
	CString lpCmdLine;
	CString filetxt;
	filetxt.Format("%s.txt", filename);
	if (filename.Right(4) == ".wps")
	{
		lpCmdLine.Format("all2txt\\a2tcmd.exe -tdoc \"%s\" \"%s\"", filename, filetxt);
	}
	else
	{
		lpCmdLine.Format("all2txt\\a2tcmd.exe \"%s\" \"%s\"", filename, filetxt);
	}
	CStringA tmp = lpCmdLine;
	char *lpcCmdLinestr = tmp.GetBuffer();
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	CreateProcess(NULL, lpcCmdLinestr, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
}

//获取电脑磁盘的个数
unsigned long CSelfFind::getDisknum()
{
	int DiskCount = 0;
	DWORD DiskInfo = GetLogicalDrives();
	//利用GetLogicalDrives()函数可以获取系统中逻辑驱动器的数量，函数返回的是一个32位无符号整型数据。
	while (DiskInfo)//通过循环操作查看每一位数据是否为1，如果为1则磁盘为真,如果为0则磁盘不存在。
	{
		if (DiskInfo & 1)//通过位运算的逻辑与操作，判断是否为1
		{
			++DiskCount;
		}
		DiskInfo = DiskInfo >> 1;//通过位运算的右移操作保证每循环一次所检查的位置向右移动一位。
		//DiskInfo = DiskInfo/2;
	}
	return DiskCount;
}

//获取我的电脑，我的文档目录
void CSelfFind::getPath(char *path1, char *path2)
{
	char m_lpszDefaultDir1[MAX_PATH] = { 0 };
	char szDocument1[MAX_PATH] = { 0 };
	memset(m_lpszDefaultDir1, 0, MAX_PATH);

	char m_lpszDefaultDir2[MAX_PATH] = { 0 };
	char szDocument2[MAX_PATH] = { 0 };
	memset(m_lpszDefaultDir2, 0, MAX_PATH);

	LPITEMIDLIST pidl = NULL;
	LPITEMIDLIST pid2 = NULL;

	SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument1))
	{
		GetShortPathName(szDocument1, m_lpszDefaultDir1, _MAX_PATH);
	}

	SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pid2);
	if (pid2 && SHGetPathFromIDList(pid2, szDocument2))
	{
		GetShortPathName(szDocument2, m_lpszDefaultDir2, _MAX_PATH);
	}
	strcpy(path1, szDocument1);
	strcpy(path2, szDocument2);
}

//获取磁盘的名字
void CSelfFind::getDiskString(char *Diskname)
{
	int DSLength = GetLogicalDriveStrings(0, NULL);
	//通过GetLogicalDriveStrings()函数获取所有驱动器字符串信息长度。
	char* DStr = new char[DSLength];//用获取的长度在堆区创建一个c风格的字符串数组
	GetLogicalDriveStrings(DSLength, (LPTSTR)DStr);
	//通过GetLogicalDriveStrings将字符串信息复制到堆区数组中,其中保存了所有驱动器的信息。

	int DType;

	for (int i = 0; i < getDisknum(); ++i)
		//为了显示每个驱动器的状态，则通过循环输出实现，由于DStr内部保存的数据是A:\NULLB:\NULLC:\NULL，这样的信息，所以DSLength/4可以获得具体大循环范围
	{
		char dir[3] = { DStr[i*4],':','\\' };
		DType = GetDriveType(DStr + i * 4);
		//GetDriveType函数，可以获取驱动器类型，参数为驱动器的根目录
		if (DType == DRIVE_FIXED||DType==DRIVE_REMOVABLE)
		{
			Diskname[i * 2] = *dir;
			Diskname[i * 2 + 1] = '\0';
		}
	}

}

//全盘扫描
void CSelfFind::AllDiskScan()
{
	
	char* diskname = (char*)malloc(2 * 32);
	memset(diskname, 0, 32 * 2);
	getDiskString(diskname);
	for (int i = 0; i < getDisknum(); i++)
	{
		Transfer *tf=(Transfer*)malloc(sizeof(Transfer));
		tf->path[0] = diskname[i * 2];
		tf->path[1] = ':';
		tf->path[2] = 0;
		tf->sf1 = this;
		//myfindfile((char*)path);
		//CreateThread(NULL, 0, ThreadAllDiskScan, (LPVOID)tf, 0, NULL);
		AfxBeginThread(ThreadAllDiskScan, (LPVOID)tf);
		ischecked++;
	}
	Transfer* time = (Transfer*)malloc(sizeof(Transfer));
	time->sf1 = this;
	AfxBeginThread(ThreadEndTimer, (LPVOID)time);
}

//快速扫描
void CSelfFind::FastScan()
{

	char desktop[MAX_PATH] = { 0 };
	char mydocument[MAX_PATH] = { 0 };
	getPath(mydocument, desktop);

	//sprintf(tf->SpecialPath, "%s", desktop);
	//CreateThread(NULL, 0, ThreadFastScan, (LPVOID)tf, 0, NULL);
	//sprintf(tf->SpecialPath, "%s", mydocument);
	char tmp[2][MAX_PATH] = { 0 };
	sprintf(tmp[0], "%s", desktop);
	sprintf(tmp[1], "%s", mydocument);
	for (int i = 0; i < 2; i++)
	{
		Transfer* tf = (Transfer*)malloc(sizeof(Transfer));
		tf->sf1 = this;
		sprintf(tf->SpecialPath, "%s", tmp[i]);
		//CreateThread(NULL, 0, ThreadFastScan, (LPVOID)tf, 0, NULL);
		h_fastscan = AfxBeginThread(ThreadFastScan, (LPVOID)tf, 0, 0, 0, NULL);
		ischecked++;
	}
	Transfer* time = (Transfer*)malloc(sizeof(Transfer));
	time->sf1 = this;
	AfxBeginThread(ThreadEndTimer, (LPVOID)time);
}

//全盘扫描按钮响应函数
void CSelfFind::OnBnClickedAlldiskscan()
{
	// TODO: 在此添加控件通知处理程序代码
	//复位timer

	//开始计时
	m_time.SetWindowTextA("所用时间:00:00:00");
	ss = 0; mm = 0;
	mss = 0;
	SetTimer(1, 100, NULL);
	m_list.DeleteAllItems();
	AllDiskScan();
}

//快速扫描按钮响应函数
void CSelfFind::OnBnClickedFastscan()
{
	// TODO: 在此添加控件通知处理程序代码
	m_time.SetWindowTextA("所用时间:00:00:00");
	ss = 0; mm = 0;
	mss = 0;
	SetTimer(1, 100, NULL);//设置定时器
	m_list.DeleteAllItems();
	FastScan();
}

//双击鼠标左键
void CSelfFind::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

}

//响应鼠标右击函数右键
void CSelfFind::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//如果当前没有选中的行右键菜单不显示
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int CurSel = m_list.GetNextSelectedItem(pos);//取得当前的行号
	if (CurSel == -1)return;//如果没有选中的项目，返回
	//显示弹出菜单
	CPoint point;
	GetCursorPos(&point);

	CMenu menu;
	menu.LoadMenuA(IDR_MENU1);
	CMenu* popup = menu.GetSubMenu(0);
	//弹出菜单显示
	popup->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
}

//右键打开文件
void CSelfFind::On32771()
{
	// TODO: 在此添加命令处理程序代码
	int num = m_list.GetSelectionMark();
	CString s = m_list.GetItemText(num, 2);
	ShellExecute(NULL, _T("open"), s, NULL, NULL, SW_SHOWNORMAL);
}

//右键打开文件路径
void CSelfFind::On32772()
{
	// TODO: 在此添加命令处理程序代码
	int num = m_list.GetSelectionMark();
	CString s = m_list.GetItemText(num, 2);
	if (!s.IsEmpty())
	{
		char _Ext[FILENAME_MAX] = { 0 };
		char _Filename[FILENAME_MAX] = { 0 };
		char _Dir[FILENAME_MAX] = { 0 };
		char _Drive[FILENAME_MAX] = { 0 };
		_splitpath(s, _Drive, _Dir, _Filename, _Ext);
		strcat(_Drive, _Dir);
		//MessageBox(_Drive);
		ShellExecute(NULL, _T("open"), _Drive, NULL, NULL, SW_SHOWNORMAL);
	}
}

//右键删除消息处理函数
void CSelfFind::On32773()
{
	// TODO: 在此添加命令处理程序代码
	if (MessageBox("确认要删除当前选中文件吗？", "提示！", MB_OKCANCEL) == IDOK)
	{
		int num = m_list.GetSelectionMark();
		CString s = m_list.GetItemText(num, 2);
		CFile file;
		CString str;
		file.Open(s, CFile::modeWrite | CFile::typeBinary, NULL);
		DWORD f_size = file.GetLength();
		char *ZeroBuf = (char*)malloc(sizeof(char)*f_size);
		for (int i = 0; i < f_size; i++)
		{
			strcat(ZeroBuf, "0");
		}
		file.Write(ZeroBuf, strlen(ZeroBuf));//写入0清空文件防止恢复
		free(ZeroBuf);
		file.Close();
		if (remove(s) == 0)
		{
			m_list.DeleteItem(num);
			//将已删除的行后面的行的序号都重置为标准化的序号
			while (!m_list.GetItemText(num, 0).IsEmpty())
			{
				str.Format("%d", num+1);
				m_list.SetItemText(num, 0, str);
				num++;
			}
			MessageBox("删除成功!");
		}
		else
		{
			MessageBox("删除失败!");
		}
	}
}

//定义一个计时器用来统计扫描的时间
void CSelfFind::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CString str;
	switch (nIDEvent)
	{
	case 1:
		mss+=100;
		if (mss == 1000) { mss = 0; ss++; }
		if (ss == 60) { ss = 0; mm++; }
		str.Format(_T("所用时间:%02i:%02i:%01i"), mm, ss,mss/100);
		m_time.SetWindowTextA(str);
		break;
	case 2:
		KillTimer(1);
		m_time.SetWindowTextA("所用时间:00:00:00");
		mm = 0;
		ss = 0;
		mss = 0;
	}
	CFormView::OnTimer(nIDEvent);
}

//测试按钮
void CSelfFind::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFile file;
	CString str;
	file.Open("1.txt",  CFile::modeWrite | CFile::typeBinary, NULL);
	DWORD f_size = file.GetLength();
	char *ZeroBuf = (char*)malloc(sizeof(char)*f_size);
	for (int i = 0; i < f_size; i++)
	{
		strcat(ZeroBuf, "0");
	}
	
}

void CSelfFind::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	/*KillTimer(1);
	m_time.SetWindowTextA("所用时间:00:00:00");
*/
	CString str;
	str.Format("sfsfsf");
	CFile mFile(_T("user.txt "), CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate);
	mFile.Write(str, strlen(str));
	mFile.Flush();
	mFile.Close();
}