// CSelfFind.cpp: 实现文件
//

#include "stdafx.h"
#include "searchFile.h"
#include "CSelfFind.h"


// CSelfFind 对话框


DWORD WINAPI ThreadAllDiskScan(LPVOID lp)
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
	MessageBox(NULL, tf.path, "扫描完成", NULL);
	return 0;
}
DWORD WINAPI ThreadFastScan(LPVOID lp)
{
	char path[MAX_PATH] = { 0 };
	sprintf(path, "%s", ((Transfer*)lp)->SpecialPath);
	//MessageBox(NULL, path, NULL, NULL);
	ListInfo li[1];
	li->strDir = ((Transfer*)lp)->SpecialPath;
	((Transfer*)lp)->sf1->TraverseDir(li);
	MessageBox(NULL, ((Transfer*)lp)->SpecialPath, "扫描完成", NULL);
	return 0;
}
IMPLEMENT_DYNCREATE(CSelfFind, CFormView)

int CSelfFind::filenum = 0;//初始化静态成员
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
}


BEGIN_MESSAGE_MAP(CSelfFind, CFormView)
	ON_BN_CLICKED(IDC_ALLDISKSCAN, &CSelfFind::OnBnClickedAlldiskscan)
	ON_BN_CLICKED(IDC_FASTSCAN, &CSelfFind::OnBnClickedFastscan)
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
			m_showeveryfile.Format("正在扫描%s", FilePath);
			SetDlgItemText(IDC_SHOWEVERYFILE,m_showeveryfile);
			if (name.Right(4) == ".doc" || name.Right(5) == ".docx" || name.Right(4) == ".xls" || name.Right(5) == "xlsx" || name.Right(4) == ".pdf")
			{
				if (isSercetFile(FilePath, Summary))
				{
					tmp.Format("%d", filenum + 1);
					/*myInfo->my_tmp = tmp;
					myInfo->my_name = name;
					myInfo->my_FilePath = FilePath;
					myInfo->my_FileTitle = FileTitle;
					myInfo->my_Summary = Summary;*/
					m_list.InsertItem(filenum, tmp);
					m_list.SetItemText(filenum, 1, name);
					m_list.SetItemText(filenum, 2, FilePath);
					m_list.SetItemText(filenum, 3, Summary);
					filenum += 1;
				}
				//myInfo->my_showeveryFile = name;
			}
			else
			{
				//myInfo->my_showeveryFile = name;
			}

			//vecFiles.push_back(name);
		}

	}

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
		Transfer tf[1];
		tf->path[0] = diskname[i * 2];
		tf->path[1] = ':';
		tf->path[2] = 0;
		tf->sf1 = this;
		//myfindfile((char*)path);
		CreateThread(NULL, 0, ThreadAllDiskScan, (LPVOID)tf, 0, NULL);
	}
}
void CSelfFind::OnBnClickedAlldiskscan()
{
	// TODO: 在此添加控件通知处理程序代码
	//AllDiskScan();
	char* diskname = (char*)malloc(2 * 32);
	memset(diskname, 0, 32 * 2);
	getDiskString(diskname);
	for (int i = 0; i < getDisknum(); i++)
	{
		Transfer* tf=(Transfer*)malloc(sizeof(Transfer));
		tf->path[0] = diskname[i * 2];
		tf->path[1] = ':';
		tf->path[2] = 0;
		tf->sf1 = this;
		//myfindfile((char*)path);
		CreateThread(NULL, 0, ThreadAllDiskScan, (LPVOID)tf, 0, NULL);
		//MessageBox(tf->path);
	}
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
		CreateThread(NULL, 0, ThreadFastScan, (LPVOID)tf, 0, NULL);
	}
	
}