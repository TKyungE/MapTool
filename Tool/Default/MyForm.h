#pragma once

// CMyForm �� ���Դϴ�.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

public:
	typedef struct tagVIBInfo
	{
		unsigned int iNumVer;
		unsigned int iStride;
		unsigned long dwFVF;
		D3DPRIMITIVETYPE ePrimitiveType;
		unsigned int iNumPrimitive;

		unsigned int iIndicesByte;
		D3DFORMAT eIndexFormat;
	}VIBINFO;

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();

private:
	VIBINFO m_tVIBInfo;
};


