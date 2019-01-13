// CSelfFind.cpp: 实现文件
//

#include "stdafx.h"
#include "searchFile.h"
#include "CSelfFind.h"

int kkk = 0;//测试马上删除
// CSelfFind 对话框
int isfinished=0;
int ischecked = 0;
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
			tf.sf1->fileindex = 0;
			tf.sf1->filenum = 0;
			CString kkk_tmp;
			kkk_tmp.Format("%d", kkk);
			tf.sf1->MessageBox(kkk_tmp);
			return 0;
		}
	}
}
IMPLEMENT_DYNCREATE(CSelfFind, CFormView)

unsigned long CSelfFind::filenum = 0;//初始化静态成员
int CSelfFind::fileindex = 0;
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

// CSelfFind 消息处理程序



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
			//把所有的搜索到的文件写入文件user.txt
		/*	CString str;
			str.Format("%s\r\n",FilePath);
			CFile mFile;
			if (mFile.Open(_T("user.txt"), CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate) != NULL)
			{
				mFile.SeekToEnd();
				mFile.Write(str, strlen(str));
				mFile.Flush();
				mFile.Close();
			}
			else
			{
				kkk++;
			}*/
			if (FilePath.IsEmpty())
			{
				return;
			}
			m_showeveryfile.Format("已扫描文件个数%u-正在扫描%s",this->filenum+1,FilePath);
			SetDlgItemText(IDC_SHOWEVERYFILE,m_showeveryfile);
			this->filenum++;
			if (name.Right(4) == ".doc" || name.Right(5) == ".docx" || name.Right(4) == ".xls" || name.Right(5) == "xlsx" || name.Right(4) == ".pdf"|| name.Right(4) == ".wps")
			{
				CFile changeWpsName;
				CString realName = FilePath;
				if (name.Right(4) == ".wps")
				{
					FilePath += ".doc";
					changeWpsName.Rename(realName, FilePath);
				}
				if (isSercetFile(FilePath, Summary))
				{
					tmp.Format("%d", fileindex + 1);
					/*myInfo->my_tmp = tmp;
					myInfo->my_name = name;
					myInfo->my_FilePath = FilePath;
					myInfo->my_FileTitle = FileTitle;
					myInfo->my_Summary = Summary;*/
					m_list.InsertItem(fileindex, tmp);
					m_list.SetItemText(fileindex, 1, name);
					m_list.SetItemText(fileindex, 2, FilePath);
					m_list.SetItemText(fileindex, 3, Summary);
					fileindex += 1;
					//改回原来文件名字
					if (realName.Right(4) == ".wps")
					{
						changeWpsName.Rename(FilePath, realName);
					}
				}
				//changeWpsName.Close();
				//myInfo->my_showeveryFile = name;
			}
			else
			{
			}

			//vecFiles.push_back(name);
		}

	}
	
	ff.Close();
}




BOOL CSelfFind::isSercetFile(const CString &filename, CString &Summary)
{
	//分割字符串为->无后缀绝对路径
	CString tmp_filename = filename;
	int i = 0;
	if (tmp_filename.Right(4).Left(1) == ".")
	{
		i = tmp_filename.GetLength();
		tmp_filename = tmp_filename.Left(i - 4);
	}
	else if (tmp_filename.Right(5).Left(1) == ".")
	{
		i = tmp_filename.GetLength();
		tmp_filename = tmp_filename.Left(i - 3);
	}
	//调用all2txt转化为txt
	CString lpCmdLine;
	CString filetxt;
	filetxt.Format("%s.txt", tmp_filename);
	lpCmdLine.Format("all2txt\\a2tcmd.exe \"%s\" \"%s\"", filename, filetxt);
	CStringA tmp = lpCmdLine;
	char *lpcCmdLinestr = tmp.GetBuffer();
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	CreateProcess(NULL, lpcCmdLinestr, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	//WinExec(LPCSTR(lpCmdLine), SW_HIDE);
	//查找txt中的数据
	CStdioFile myFile;
	CFileException fileException;
	char *words = (char *)malloc(1024 * sizeof(char));
	if (myFile.Open(filetxt, CFile::typeText | CFile::modeRead, &fileException))
	{
		while (myFile.ReadString((LPTSTR)words, 1024) != NULL)
		{

			//char *isfind= strstr(words, "秘密");
			if (CString(words).Find("秘密") != -1)
			{
				CString dealstr = words;
				int position = dealstr.Find("秘密");
				CString leftstr;
				CString rightstr;
				//从左边position-FINDINTVALUES字节开始获取后面FINDINTVALUE个字节
				leftstr = CString(words).Mid(position - FINDINTVALUE, FINDINTVALUE);
				rightstr = CString(words).Mid(position, FINDINTVALUE + strlen("秘密"));

				Summary = leftstr + rightstr;
				myFile.Close();
				remove(filetxt);
				return TRUE;
			}
			else
			{
			}
		}
	}

}
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
}


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
void CSelfFind::FastScan()
{
	
	char desktop[MAX_PATH] = {0};
	char mydocument[MAX_PATH] = { 0 };
	getPath(mydocument,desktop);
	
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
		h_fastscan=AfxBeginThread(ThreadFastScan, (LPVOID)tf, 0, 0, 0, NULL);
		ischecked++;
	}
	Transfer* time = (Transfer*)malloc(sizeof(Transfer));
	time->sf1 = this;
	AfxBeginThread(ThreadEndTimer, (LPVOID)time);
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
		file.Open(s, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary, NULL);
		char ZeroBuf[1024] = { 0 };
		file.Write(ZeroBuf, strlen(ZeroBuf));//写入0清空文件防止恢复
		file.Close();
		if (remove(s) == 0)
		{
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
	//WaitForSingleObject(h_fastscan->m_hThread, 3000);
	//TerminateThread(h_fastscan, 0);
	//DWORD dwExitCode;
	//GetExitCodeThread(this->h_fastscan, &dwExitCode);
	//TerminateThread(this->h_fastscan, dwExitCode);
	//if (TerminateThread(h_fastscan[0], 0) != 0&& TerminateThread(h_fastscan[1], 0) != 0)
	//{
	//	MessageBox("结束进程成功");
	//}
	//else
	//{
	//	MessageBox("结束失败");
	//}
	//SuspendThread(h_fastscan[0]);
	/*	if (isfinished == 2)
		{
			KillTimer(1);
		}*/
	//CString str;
	//str.Format("sfsfsf\r\n");
	//CFile mFile(_T("user.txt "),  CFile::modeCreate|CFile::modeWrite| CFile::modeNoTruncate);
	//mFile.SeekToEnd();
	//mFile.Write(str, strlen(str));
	//mFile.Flush();
	//mFile.Close();
	
	CFile changename;
	changename.Rename("1.wps", "1.wps.doc");
	//changename.Close();
	


	/*ss = 0; mm = 0;
	mss = 0;
	SetTimer(1, 100, NULL);*/
}
//监视函数
void CSelfFind::Monitoring()
{
	
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