
// MFCApplication12Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication12.h"
#include "MFCApplication12Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication12Dlg 对话框

BEGIN_DHTML_EVENT_MAP(CMFCApplication12Dlg)
	//DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	//DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()


CMFCApplication12Dlg::CMFCApplication12Dlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(IDD_MFCAPPLICATION12_DIALOG, IDR_HTML_MFCAPPLICATION12_DIALOG, pParent)
{
	shells = 0;
	webshells = new WebShell[50];
	Json::CharReaderBuilder reader;
	Json::Value root;
	std::string errs;
	CString mess;
	std::ifstream f("setting.json");
	if (f.is_open()) {
		bool parsingSuccessful = Json::parseFromStream(reader, f, &root, &errs);
		if (!parsingSuccessful) {
			mess.Format(_T("%s"), errs);
			//MessageBox(mess);
		}
	}
	else {
		ofstream o("setting.json");
		o.close();
		//MessageBox(_T("file error!"));
	}
	std::ofstream out("log");
	if (root.isArray()) {
		Json::Int size = root.size();
		for (Json::Int i = 0; i < size; i++) {
			METHOD meth;
			if (root[i]["method"].asString() == "GET")meth = GET;
			else meth = POST;
			webshells[i] = WebShell(root[i]["address"].asString(), meth, root[i]["pass"].asString());
			if (root[i]["custom"].isObject()) {
				if (root[i]["custom"]["encrypt"].isString())webshells[i].ParseMethod(root[i]["custom"]["encrypt"].asString());
				if (root[i]["custom"]["place"].isString() && root[i]["custom"]["placevalue"].isString())webshells[i].setPlace(root[i]["custom"]["place"].asString(), root[i]["custom"]["placevalue"].asString());
				if (root[i]["custom"]["addonget"].isObject()) {
					Json::Value::Members member = root[i]["custom"]["addonget"].getMemberNames();
					for (Json::Value::Members::iterator it = member.begin(); it != member.end(); it++) {
						webshells[i].addAddonGet(*it, root[i]["custom"]["addonget"][*it].asString());
					}

				}
				if (root[i]["custom"]["addonpost"].isObject()) {
					Json::Value::Members member = root[i]["custom"]["addonpost"].getMemberNames();
					for (Json::Value::Members::iterator it = member.begin(); it != member.end(); it++) {
						webshells[i].addAddonPost(*it, root[i]["custom"]["addonpost"][*it].asString());
					}

				}
			}
			shells++;
		}
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	EnableAutomation();
}

void CMFCApplication12Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication12Dlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMFCApplication12Dlg, CDHtmlDialog)
	DISP_FUNCTION(CMFCApplication12Dlg, "testfun", testFunction,VT_BSTR,VTS_NONE)
	DISP_FUNCTION(CMFCApplication12Dlg, "getShells", getShells, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMFCApplication12Dlg,"addShell",addShell,VT_NULL,VTS_BSTR)
	DISP_FUNCTION(CMFCApplication12Dlg,"getfilelist",getFileList,VT_BSTR,VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CMFCApplication12Dlg,"uploadfile",uploadFile,VT_BOOL,VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMFCApplication12Dlg,"downloadfile",downloadFile,VT_BOOL,VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMFCApplication12Dlg,"setdb",setDB,VT_NULL,VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMFCApplication12Dlg,"execsql",execSQL,VT_BSTR,VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMFCApplication12Dlg, "execCom", evalCommand, VT_BSTR, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CMFCApplication12Dlg,"updatesetting",save,VT_NULL,VTS_BSTR)
	DISP_FUNCTION(CMFCApplication12Dlg, "rm", deleteShell, VT_NULL, VTS_I4)
	DISP_FUNCTION(CMFCApplication12Dlg, "reloadShell", reloadShell, VT_NULL, VTS_I4 VTS_BSTR)
END_DISPATCH_MAP()



// CMFCApplication12Dlg 消息处理程序

BOOL CMFCApplication12Dlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	SetExternalDispatch(GetIDispatch(TRUE));
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	IHTMLDocument2* pDoc;
	GetDHtmlDocument(&pDoc);
	CString funcname = _T("addShells");
	VARIANT varaint;
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication12Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication12Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDHtmlDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication12Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CMFCApplication12Dlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CMFCApplication12Dlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}
BOOL CMFCApplication12Dlg::PreTranslateMessage(MSG *pMsg)
{
	if ((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
		(pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
		return FALSE;
	CComQIPtr<IOleInPlaceActiveObject> spInPlaceActiveObject = m_wndBrowser.GetControlUnknown();
	if (spInPlaceActiveObject && spInPlaceActiveObject->TranslateAccelerator(pMsg) == S_OK)
		return TRUE;
	return FALSE;
}
BOOL CMFCApplication12Dlg::CanAccessExternal()
{
	return TRUE;
}
HRESULT STDMETHODCALLTYPE CMFCApplication12Dlg::TranslateAccelerator(LPMSG lpMsg,
	const GUID *pguidCmdGroup,
	DWORD nCmdID)
{
	if (lpMsg && lpMsg->message == WM_KEYDOWN)
	{
		bool bCtrl = (0x80 == (0x80 & GetKeyState(VK_CONTROL)));

		// prevent Ctrl+N
		if (lpMsg->wParam == 'N' && bCtrl)
		{
			return S_OK;
		}

		// prevent Ctrl+F
		if (lpMsg->wParam == 'F' && bCtrl)
		{
			return S_OK;
		}

		// prevent F5
		if (lpMsg->wParam == VK_F5)
		{
			return S_OK;
		}

		// prevent ESC
		if (lpMsg->wParam == VK_ESCAPE)
		{
			return S_OK;
		}

		// prevent ENTER
		/*if (lpMsg->wParam == VK_RETURN)
		{
			return S_OK;
		}*/
	}
	return CDHtmlDialog::TranslateAccelerator(lpMsg, pguidCmdGroup, nCmdID);
}
HRESULT CMFCApplication12Dlg::CallJSFunction(IHTMLDocument2* pDoc2, CString strFunctionName, DISPPARAMS dispParams, VARIANT* varResult, EXCEPINFO* exceptInfo, UINT* nArgErr)

{

	IDispatch *pDispScript = NULL;

	HRESULT hResult;

	hResult = pDoc2->get_Script(&pDispScript);

	if (FAILED(hResult)) { return S_FALSE; }

	DISPID dispid;

	CComBSTR objbstrValue = strFunctionName;

	BSTR bstrValue = objbstrValue.Copy();

	OLECHAR *pszFunct = bstrValue;

	hResult = pDispScript->GetIDsOfNames(IID_NULL, &pszFunct, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (S_OK != hResult)

	{
		pDispScript->Release();

		return hResult;
	}

	varResult->vt = VT_VARIANT;

	hResult = pDispScript->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispParams, varResult, exceptInfo, nArgErr);

	pDispScript->Release();

	return hResult;

}
BSTR CMFCApplication12Dlg::testFunction() {
	MessageBox(_T("mdzz"));
	TCHAR Content[1024] = _T("你好");
	return ::SysAllocString(Content);
}
BSTR CMFCApplication12Dlg::getShells() {
	std::string res;
	std::string buffer;
	std::ifstream f;
	f.open("setting.json");
	while (!f.eof()) {
		f >> buffer;
		res += buffer;
	}
	CString Res;
	Res=res.c_str();
	return ::SysAllocString(Res);
}
void CMFCApplication12Dlg::addShell(BSTR str) {
	Json::CharReaderBuilder reader;
	Json::Value root;
	string errs;
	stringstream s;
	s << (_bstr_t)str;
	bool parsingSuccessful = Json::parseFromStream(reader, s, &root, &errs);
	if (!parsingSuccessful) {
		CString mess;
		mess.Format(_T("%s"), errs);
		MessageBox(mess);
	}
	if (root.isObject()) {
			METHOD meth;
			if (root["method"].asString() == "GET")meth = GET;
			else meth = POST;
			webshells[shells] = WebShell(root["address"].asString(), meth, root["pass"].asString());
			if (root["custom"].isObject()) {
				if (root["custom"]["encrypt"].isString())webshells[shells].ParseMethod(root["custom"]["encrypt"].asString());
				if (root["custom"]["place"].isString() && root["custom"]["placevalue"].isString())webshells[shells].setPlace(root["custom"]["place"].asString(), root["custom"]["placevalue"].asString());
				if (root["custom"]["addonget"].isObject()) {
					Json::Value::Members member = root["custom"]["addonget"].getMemberNames();
					for (Json::Value::Members::iterator it = member.begin(); it != member.end(); it++) {
						webshells[shells].addAddonGet(*it, root["custom"]["addonget"][*it].asString());
					}

				}
				if (root["custom"]["addonpost"].isObject()) {
					Json::Value::Members member = root["custom"]["addonpost"].getMemberNames();
					for (Json::Value::Members::iterator it = member.begin(); it != member.end(); it++) {
						
						webshells[shells].addAddonPost(*it, root["custom"]["addonpost"][*it].asString());
					}

				}
			}
			shells++;
		}
		
	}
BSTR CMFCApplication12Dlg::getFileList(int index, BSTR basedir) {
	std::string ans;
	std::string directory = (_bstr_t)basedir;
	webshells[index].GetFileList(ans, directory);
	CString Res;
	ans = curlhelper::UTF8ToGBK(ans);
	Res = ans.c_str();
	return ::SysAllocString(Res);
}
BOOL  CMFCApplication12Dlg::downloadFile(int index, BSTR filename,BSTR downame) {

	dowloadStruct* down = new dowloadStruct();
	down->index = index;
	down->filename = (_bstr_t)filename;
	down->downname = (_bstr_t)downame;
	down->shells = webshells;
	AfxBeginThread(downloadFileThread, down);
	return true;

}
BSTR  CMFCApplication12Dlg::evalCommand(int index, BSTR command) {
	std::string ans;
	std::string comm = (_bstr_t)command;
	webshells[index].ShellCommandExec(comm, ans);
	CString res;
	ans = curlhelper::UTF8ToGBK(ans);
	res = ans.c_str();
	return ::SysAllocString(res);
}
BOOL CMFCApplication12Dlg::uploadFile(int index, BSTR  uploadfile, BSTR filename) {
	uploadStruct* upload=new uploadStruct();
	upload->filename = (_bstr_t)uploadfile;
	upload->updir = (_bstr_t)filename;
	upload->shells = webshells;
	upload->index = index;
	AfxBeginThread(uploadFileThread,upload);
	return true;
}
BSTR CMFCApplication12Dlg::execSQL(int index,BSTR db, BSTR sql) {
	std::string ans;
	std::string sqlsentence = (_bstr_t)sql;
	std::string database = (_bstr_t)db;
	CURLcode code=webshells[index].ExecSQL(sqlsentence, database, ans);
	CString res;
	ans=curlhelper::UTF8ToGBK(ans);
	res = ans.c_str();
	return ::SysAllocString(res);
}
void CMFCApplication12Dlg::setDB(int index, BSTR host, BSTR user, BSTR pass, BSTR db, BSTR port) {
	string dbhost = (_bstr_t)host;
	string dbuser = (_bstr_t)user;
	string dbpass = (_bstr_t)pass;
	string dbport = (_bstr_t)port;
	webshells[index].setSQLconnection(dbhost,dbuser, dbpass,dbport);
}
void CMFCApplication12Dlg::save(BSTR json) {
	string data = (_bstr_t)json;
	ofstream out("setting.json");
	out << data;
	out.close();
}
UINT CMFCApplication12Dlg::downloadFileThread(LPVOID download) {
	dowloadStruct* down = (dowloadStruct*)download;
	CURLcode code = down->shells[down->index].GetFile(down->filename, down->downname);
	if (code == CURLE_OK || code == CURLE_HTTP_NOT_FOUND) {
		CString message;
		message = down->filename.c_str();
		message += " 下载成功";
		AfxMessageBox(message);
		return true;
	}
	else {
		CString message;
		message = down->filename.c_str();
		message += " 下载失败";
		AfxMessageBox(message);
		return false;
	}
}
UINT CMFCApplication12Dlg::uploadFileThread(LPVOID upfile) {
	uploadStruct* s = (uploadStruct*)upfile;
	CURLcode code = s->shells[s->index].UploadFile(s->filename,s->updir);
	if (code == CURLE_OK || code == CURLE_HTTP_NOT_FOUND) {
		CString message;
		message = s->filename.c_str();
		message += " 上传成功";
		AfxMessageBox(message);
		return true;
	}
	else {
		CString message;
		message = s->filename.c_str();
		message += " 上传失败";
		AfxMessageBox(message);
		return false;
	}
}
void CMFCApplication12Dlg::deleteShell(int index) {
	for (int i = index; i < shells; i++) {
		webshells[i] = webshells[i + 1];
	}
}
void  CMFCApplication12Dlg::reloadShell(int index, BSTR jsonstr) {
	Json::CharReaderBuilder reader;
	Json::Value root;
	string errs;
	stringstream s;
	s << (_bstr_t)jsonstr;
	bool parsingSuccessful = Json::parseFromStream(reader, s, &root, &errs);
	if (!parsingSuccessful) {
		CString mess;
		mess.Format(_T("%s"), errs);
		MessageBox(mess);
	}
	if (root.isObject()) {
		METHOD meth;
		if (root["method"].asString() == "GET")meth = GET;
		else meth = POST;
		webshells[index] = WebShell(root["address"].asString(), meth, root["pass"].asString());
		if (root["custom"].isObject()) {
			if (root["custom"]["encrypt"].isString())webshells[index].ParseMethod(root["custom"]["encrypt"].asString());
			if (root["custom"]["place"].isString() && root["custom"]["placevalue"].isString())webshells[index].setPlace(root["custom"]["place"].asString(), root["custom"]["placevalue"].asString());
			if (root["custom"]["addonget"].isObject()) {
				Json::Value::Members member = root["custom"]["addonget"].getMemberNames();
				for (Json::Value::Members::iterator it = member.begin(); it != member.end(); it++) {
					webshells[index].addAddonGet(*it, root["custom"]["addonget"][*it].asString());
				}

			}
			if (root["custom"]["addonpost"].isObject()) {
				Json::Value::Members member = root["custom"]["addonpost"].getMemberNames();
				for (Json::Value::Members::iterator it = member.begin(); it != member.end(); it++) {

					webshells[index].addAddonPost(*it, root["custom"]["addonpost"][*it].asString());
				}

			}
		}
		//shells++;
	}

}

