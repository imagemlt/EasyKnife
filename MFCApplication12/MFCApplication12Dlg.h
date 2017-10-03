
// MFCApplication12Dlg.h : 头文件
//

#pragma once
#include"json/json.h"
#include<iostream>
#include<fstream>
#include<string>
#include"WebShell.h"
#include<sstream>
#pragma comment(lib, "libcurl.lib") 
#pragma comment(lib, "lib_json.lib") 
// CMFCApplication12Dlg 对话框
struct dowloadStruct {
	WebShell* shells;
	int index;
	string filename;
	string downname;
};
struct uploadStruct {
	WebShell* shells;
	int index;
	string filename;
	string updir;
};
class CMFCApplication12Dlg : public CDHtmlDialog
{
// 构造
public:
	CMFCApplication12Dlg(CWnd* pParent = NULL);	// 标准构造函数
	WebShell* webshells;
	int shells;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION12_DIALOG, IDH = IDR_HTML_MFCAPPLICATION12_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL CanAccessExternal();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	BOOL PreTranslateMessage(MSG *pMsg);
	HRESULT CallJSFunction(IHTMLDocument2* pDoc2, CString strFunctionName, DISPPARAMS dispParams, VARIANT* varResult, EXCEPINFO* exceptInfo, UINT* nArgErr);
	STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, const GUID *pguidCmdGroup, DWORD nCmdID);
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
	BSTR testFunction();
	BSTR getShells();
	void addShell(BSTR str);
	BSTR getFileList(int index, BSTR basedir);
	BOOL downloadFile(int index, BSTR filename,BSTR downame);
	BSTR evalCommand(int index, BSTR command);
	BOOL uploadFile(int index, BSTR  uploadfile, BSTR filename);
	BSTR execSQL(int index,BSTR db, BSTR sql);
	void setDB(int index, BSTR host, BSTR user, BSTR pass, BSTR db, BSTR port);
	void save(BSTR json);
	void deleteShell(int index);
	void reloadShell(int index, BSTR jsonstr);
	static	UINT downloadFileThread(LPVOID downloadFile);
	static	UINT uploadFileThread(LPVOID downloadFile);
};
