#include "stdafx.h"
#include "AddressEdit.h"
#include "AddressBar.hpp"
#include "../Dialog/DialogDefine.hpp"
#include "../common/SqlExe.hpp"
#include "../Error/ErrorInfo.hpp"
#include "../Dialog/PromptDlg.hpp"
#include "../Dialog/DialogDefine.hpp"
#include "UrlCacheUtil.h"
#include "AddressBar.hpp"
#include "../Common/AssistTools.hpp"
#include "../Web/WebDocument.h"
#include "../MainFrame.hpp"
#include "../Utils/TranslateUtils.h"
#include "../Core/Core_Instance.h"
//#include "../Utils/LoggerMgr.h"
#include "CustomAutoComplete.h"

#include <comutil.h>
#       pragma comment(lib, "comsuppw.lib")
using namespace Xgamer::UI;
using namespace Xgamer::Dialog;
using namespace Poco::Data;

namespace Poco {
namespace Data {


template <>
class TypeHandler<Url>
{
public:
	static void bind(std::size_t pos, const Url& obj, AbstractBinder* pBinder)
	{
		// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
		poco_assert_dbg (pBinder != 0);
		pBinder->bind(pos++, obj._url);
		pBinder->bind(pos++, obj._domain);
		pBinder->bind(pos++, obj._path);
		pBinder->bind(pos++, obj._title);
		pBinder->bind(pos++, obj._fragment);
	}

	static void prepare(std::size_t pos, const Url& obj, AbstractPreparation* pPrepare)
	{
		// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
		poco_assert_dbg (pPrepare != 0);
		pPrepare->prepare(pos++, obj._url);
		pPrepare->prepare(pos++, obj._domain);
		pPrepare->prepare(pos++, obj._path);
		pPrepare->prepare(pos++, obj._title);
		pPrepare->prepare(pos++, obj._fragment);

	}

	static std::size_t size()
	{
		return 5;
	}

	static void extract(std::size_t pos, Url& obj, const Url& defVal, AbstractExtractor* pExt)
	{
		poco_assert_dbg (pExt != 0);
		if (!pExt->extract(pos++, obj._url))
			obj._url = defVal._url;
		if (!pExt->extract(pos++, obj._domain))
			obj._domain = defVal._domain;
		if (!pExt->extract(pos++, obj._path))
			obj._path = defVal._path;
		if (!pExt->extract(pos++, obj._title))
			obj._title = defVal._title;
		if (!pExt->extract(pos++, obj._fragment))
			obj._fragment = defVal._fragment;
	}

private:
	TypeHandler();
	~TypeHandler();
	TypeHandler(const TypeHandler&);
	TypeHandler& operator=(const TypeHandler&);
};


} } // namespace Poco::Data


namespace DuiLib
{
	class CAddressEditWnd : public CWindowWnd
	{
	public:
		CAddressEditWnd();
		~CAddressEditWnd();

		void Init(CAddressEditUI* pOwner);
		RECT CalPos();

		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		void SetKeepFocus(bool bKeepFocus);
		bool IsKeepFocus() const;

		HWND GetHwnd() const;
	protected:
		typedef std::pair<concurrency::cancellation_token_source, concurrency::cancellation_token_source> pair_cts;
		pair_cts m_pair_cts;
		CAddressEditUI* m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;
		bool m_bKeepFocus;
	};

	class CAddressDropDownWnd : public WindowImplBase
	{
	public:
		CAddressDropDownWnd(CAddressEditUI* pOwner);
		~CAddressDropDownWnd();

		void OnFinalMessage( HWND hWnd );
		void InitWindow();
		RECT CalPos();
		void Notify(TNotifyUI& msg);
		void OnPrepare(TNotifyUI& msg);
		void OnSelect(TNotifyUI& msg);
		void OnItemClick(TNotifyUI& msg);
		void OnDelete(TNotifyUI& msg);
		virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		CControlUI* CreateControl(LPCTSTR pstrClass);
		CDuiString CalIconPath(const CDuiString& sUrl);
		bool SetSearchText(const CDuiString& sText);
		void SetSearchExtraInfo(const CDuiString& sHead, const CDuiString& sTail);
		bool UpdateAddrItemData(const CDuiString& sTitle, const CDuiString& sUrl, int index);
		bool UpdateGameItemData(
				const std::wstring& sId, 
				const std::wstring& sIconPath, 
				const std::wstring& sGameName,
				const std::wstring& sGameType,
				const std::wstring& sGameDesc,
				int index=0
				);
		//void OnUpdateEditText();
		bool UpDateDropWndRect(int nWidth, int nHeight);
		bool UpdateDropWndData(const std::vector<Url>& vec_url);
		CDuiString GetFirstUrl() const;
		void SetGameHeight(int nCount);
		int GetGameHeight() const;
		bool SetDropItemVisible(int nIndex, bool bVisible);
		int GetCount();
		std::string GetGameSearchId() const;
	//	CDuiString m_szSql;
		CDuiString m_editText;
		CDuiString m_szTitle;
		CDuiString m_szUrl;
		CDuiString m_szFirstUrl;
		int m_nIndex;
		int m_nWindowWidth;
		bool m_bIsShowWindow;
		bool m_bIsTextEditOK;
		bool m_bIsReDraw;
		bool m_bIsClearItem;

		int m_gameHeight;
		std::string m_gameId;//在游戏搜索时赋值， 再发送消息
	protected:
		LPCTSTR GetWindowClassName(void) const;
		virtual CDuiString GetSkinFile();
		virtual CDuiString GetSkinFolder();
	private:
		CAddressEditUI* m_pOwner;
		CListUI* m_pMainList;
		CDialogBuilder* m_pBuilder;
	};

	CAddressEditWnd::CAddressEditWnd() 
		: m_pOwner(NULL)
		, m_hBkBrush(NULL)
		, m_bInit(false)
		, m_bKeepFocus(false)
	{
	}

	CAddressEditWnd::~CAddressEditWnd()
	{
		m_pair_cts.second.cancel();
	//	m_pOwner->m_pCustomAutoComplete->Unbind();
	}


	void CAddressEditWnd::Init(CAddressEditUI* pOwner)
	{
		m_pOwner = pOwner;
		RECT rcPos = CalPos();
		UINT uStyle = WS_CHILD | ES_AUTOHSCROLL;
		if ( m_pOwner->IsPasswordMode() ) uStyle |= ES_PASSWORD;
		Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
		HFONT hFont=NULL;
		int iFontIndex=m_pOwner->GetFont();
		if (iFontIndex!=-1)
			hFont=m_pOwner->GetManager()->GetFont(iFontIndex);
		if (hFont==NULL)
			hFont=m_pOwner->GetManager()->GetDefaultFontInfo()->hFont;

		SetWindowFont(m_hWnd, hFont, TRUE);
		Edit_LimitText(m_hWnd, m_pOwner->GetMaxChar());
		if ( m_pOwner->IsPasswordMode() ) Edit_SetPasswordChar(m_hWnd, m_pOwner->GetPasswordChar());
		Edit_SetText(m_hWnd, m_pOwner->GetText());
		Edit_SetModify(m_hWnd, FALSE);
		SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
		Edit_Enable(m_hWnd, m_pOwner->IsEnabled() == true);
		Edit_SetReadOnly(m_hWnd, m_pOwner->IsReadOnly() == true);
		//Styls
		LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
		styleValue |= pOwner->GetWindowStyls();
		::SetWindowLong(GetHWND(), GWL_STYLE, styleValue);
		::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
		::SetFocus(m_hWnd);
	/*	if (m_pOwner->m_pCustomAutoComplete->Bind(m_hWnd, ACO_UPDOWNKEYDROPSLIST | ACO_AUTOSUGGEST | ACO_AUTOAPPEND, _T("www.%s.com")))
		{
		}*/
		m_bInit = true;    
	}

	inline void CAddressEditWnd::SetKeepFocus(bool bKeepFocus)
	{
		m_bKeepFocus = bKeepFocus;
	}


	inline bool CAddressEditWnd::IsKeepFocus() const
	{
		return m_bKeepFocus;
	}

	inline HWND CAddressEditWnd::GetHwnd() const
	{
		return m_hWnd;
	}



	RECT CAddressEditWnd::CalPos()
	{
		CDuiRect rcPos = m_pOwner->GetPos();
		RECT rcInset = m_pOwner->GetTextPadding();
		rcPos.left += rcInset.left;
		rcPos.top += rcInset.top;
		rcPos.right -= rcInset.right;
		rcPos.bottom -= rcInset.bottom;
		LONG lEditHeight = m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->tm.tmHeight;
		if ( lEditHeight < rcPos.GetHeight() ) {
			rcPos.top += (rcPos.GetHeight() - lEditHeight) / 2;
			rcPos.bottom = rcPos.top + lEditHeight;
		}
		return rcPos;
	}

	LPCTSTR CAddressEditWnd::GetWindowClassName() const
	{
		return _T("EditWnd");
	}

	LPCTSTR CAddressEditWnd::GetSuperClassName() const
	{
		return WC_EDIT;
	}

	void CAddressEditWnd::OnFinalMessage(HWND /*hWnd*/)
	{
		m_pOwner->Invalidate();
		// Clear reference and die
		if ( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
		m_pOwner->m_pEditWindow = NULL;
		if (m_pOwner->m_pDropDownWindow)
		{
			m_pOwner->m_pDropDownWindow->Close();
		}
		delete this;
	}

	LRESULT CAddressEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		if (uMsg == WM_CHAR)
		{
			//edit changed 
			//keep focus on address edit
			m_bKeepFocus = true;
			int nstatsel = 0;
			int nendsel = 0;
			::SendMessage(m_hWnd, EM_GETSEL, (WPARAM)&nstatsel, (LPARAM)&nendsel);
			int nTextLeng = ::GetWindowTextLength(m_hWnd) + 1;
			TCHAR* szText = new TCHAR[nTextLeng];
			ZeroMemory(szText , nTextLeng);
			Edit_GetText(m_hWnd,szText,nTextLeng);

			if (nstatsel>0&&nendsel>0&&nendsel>nstatsel)
			{
				CDuiString szEditText(szText);
				CDuiString szSelFirstText(szEditText.Left(1));
				CDuiString szFirstText(szEditText.Mid(nstatsel,1));
				if (_tcscmp(szFirstText,szSelFirstText) == 0)
				{
					Edit_SetSel(m_hWnd,nstatsel+1,nendsel);
					CDuiString szSelText(szEditText.Mid(nstatsel+1,nendsel - nstatsel));
					m_pOwner->SetSelText(szSelText);
					//设置选中部分的字
					delete[] szText;
				//	::OutputDebugStringW(_T("WM_CHAR OnEditChanged\n"));
					return OnEditChanged(uMsg, wParam, lParam, bHandled);;
				}
			}
			delete[] szText;
		}
		else if (uMsg == WM_KEYDOWN)
		{
			TEventUI event = { 0 };
			event.Type = UIEVENT_KEYDOWN;
			event.chKey = (TCHAR)wParam;
			event.wKeyState = Xgamer::Utils::MyMapKeyState();
			event.dwTimestamp = ::GetTickCount();
			event.wParam = wParam;
			event.lParam = lParam;
			m_pOwner->Event(event);

			switch(wParam)
			{
				case VK_DOWN:
				case VK_UP:
				case VK_TAB:
					return lRes;
				default:
					bHandled = FALSE;
					break;
			}
		}
		//else if (uMsg == WM_SETFOCUS)
		//{
		//	if (m_pOwner && !m_pOwner->m_pDropDownWindow)
		//	{
		//		m_pOwner->m_pDropDownWindow = new CAddressDropDownWnd(m_pOwner);
		//		m_pOwner->m_pDropDownWindow->CreateDuiWindow(m_pOwner->GetManager()->GetPaintWindow(), _T(""), WS_POPUP, 0);
		//		m_pOwner->m_pDropDownWindow->ShowWindow(true, false);
		//		::BringWindowToTop(m_pOwner->m_pDropDownWindow->GetHWND());
		//	}
		//}

		if ( uMsg == WM_KILLFOCUS ) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
		else if ( uMsg == OCM_COMMAND ) 
		{
			if ( GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE ) 
			{
				if (!m_pOwner->IsSetSelText())
				{
				//	::OutputDebugStringW(_T(" uMsg == OCM_COMMAND OnEditChanged start"));
					lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
				}
				return lRes;
			}
			else if ( GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE )
			{
				RECT rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
			}
		}
	/*	else if ( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN ) {
			m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_RETURN);
		}*/
		
		else if ( uMsg == OCM__BASE + WM_CTLCOLOREDIT  || uMsg == OCM__BASE + WM_CTLCOLORSTATIC ) {
			if ( m_pOwner->GetNativeEditBkColor() == 0xFFFFFFFF ) return NULL;
			::SetBkMode((HDC)wParam, TRANSPARENT);
			DWORD dwTextColor = m_pOwner->GetTextColor();
			::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor),GetGValue(dwTextColor),GetRValue(dwTextColor)));
			if ( m_hBkBrush == NULL ) {
				DWORD clrColor = m_pOwner->GetNativeEditBkColor();
				m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
			}
			return (LRESULT)m_hBkBrush;
		}
		else bHandled = FALSE;
		if ( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CAddressEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	/*	CDuiString sDegbug;
		sDegbug.Format(_T(" CAddressEditWnd::OnKillFocus [hwnd=%d]\n"), wParam);
		::OutputDebugString(sDegbug.GetData());*/
		if (m_pOwner)
		{
			if (m_pOwner->m_pDropDownWindow && wParam)
			{
				if (wParam == (ULONG)(m_pOwner->m_pDropDownWindow->GetHWND()))
				{
					bHandled = TRUE;
				//::OutputDebugString(L"wParam == (ULONG)(m_pOwner->m_pDropDownWindow->GetHWND()\n");
					return S_OK;
				}
			}

			if (IsKeepFocus())
			{
				bHandled = TRUE;
				::SetFocus(m_hWnd);
			//	::OutputDebugString(L"OnKillFocus [reset focus on edit]\n");
				return S_OK;
			}
		}

		LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		PostMessage(WM_CLOSE);
		return lRes;
	}

	LRESULT CAddressEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if ( !m_bInit ) return 0;
		if ( m_pOwner == NULL ) return 0;
		// Copy text back
		int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
		LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
		ASSERT(pstr);
		if ( pstr == NULL ) return 0;
		::GetWindowText(m_hWnd, pstr, cchLen);
		//set edit control' text = 
		m_pOwner->m_sText = pstr;
	/*	::OutputDebugStringW(_T("sennotify:"));
		::OutputDebugStringW(pstr);
		::OutputDebugStringW(_T("\n"));*/
		m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
		m_pOwner->SetSearchText(pstr);
		if (m_pOwner->IsSetText())
			return S_OK;
		m_pair_cts.second.cancel();
		auto token = m_pair_cts.first.get_token();
		m_pair_cts.second = m_pair_cts.first;
		m_pair_cts.first = concurrency::cancellation_token_source();
		auto ptrText = std::make_shared<std::wstring>(std::wstring(pstr));
		auto ptrVecUrl = std::make_shared<std::vector<Url>>(std::vector<Url>());
		auto t = concurrency::create_task([this, ptrText, ptrVecUrl]()
		{
			Session& session = m_pOwner->GetSession();
			CDuiString szSql;
			//转变查询匹配url语句
			szSql.Format(_T("select * from tb_url where url like '%%%s%%' order by case when url='%s' then 10 else 0 end desc,case when path='%s' then 9 else 0 end desc,case when path like'%%%s%%' then 8 else 0 end desc,case when domain='%s' then 7 else 0 end desc,length(url) limit 9"),
				ptrText->c_str(), ptrText->c_str(), ptrText->c_str(), ptrText->c_str(), ptrText->c_str());
			//执行查询结果存入vec_url
			session << W2A_UTF8(szSql.GetData()), into(*ptrVecUrl.get()), now;
		}, token);

		t.then([this, ptrVecUrl]()
		{
			int nHeight = ptrVecUrl->size() * 47/*地址栏搜索47/个*/ + 2/*上下边框一像素*/ + 26/*给默认搜索留的*/;
			nHeight += m_pOwner->GetGameHeight();
			nHeight += 6;

			static CContainerUI* pParent = static_cast<CContainerUI*>(m_pOwner->GetParent()->GetParent());
			assert(pParent && "can not find edit' parent's parent's control");
			RECT rc = pParent->GetPos();
			CDuiRect inset(pParent->GetInset());
			POINT pt = {rc.left+inset.left, rc.bottom+1};
			ClientToScreen(m_hWnd, &pt);
			//更新地址栏弹出窗口数据
			m_pOwner->UpdateDropWndData(*(ptrVecUrl.get()));
			//更新地址栏弹出窗口大小
			m_pOwner->UpDateDropWndRect(rc.right-rc.left-inset.left, nHeight);
		}, token)
		// 
		// 设置sel edit;
		//
			.then([this, ptrVecUrl]()
		{
			if (ptrVecUrl->size() > 0 && !m_pOwner->m_bSetSelText)
			{
				do
				{
					m_pOwner->m_bSetSelText = true;
					CDuiString szUrl(m_pOwner->GetFirstUrl());
					CDuiString szSelEditText(m_pOwner->GetSelText());
					CDuiString szText(m_pOwner->GetText());
					if (!szSelEditText.IsEmpty())
					{
						szText.Replace(szSelEditText,L"");
						m_pOwner->m_sText = szText;
					//	m_pEditAddress->SetSelText(L"");
					}
					if (!szUrl.IsEmpty())
					{
						//赋值给edit
						szText = m_pOwner->GetText();
						int nPos = szUrl.Find(szText);
						nPos = nPos + szText.GetLength();
						if (nPos>0)
						{
							CDuiString szSelText = szUrl.Mid(nPos,szUrl.GetLength()- nPos);
							int nSelPos = szText.GetLength();
							break;
							//if (!szSelText.IsEmpty())
							//{
							//	m_pOwner->SetSelText(szSelText);
							//	CDuiString szDomain = GetHttpWebUrl(szUrl.GetData()).GetString();
							//	if (szDomain.Find(szText) == -1)
							//	{
							//		m_pOwner->SetSel(szText.GetLength(),szText.GetLength() );
							//		break;
							//	}
							//	//截取多次
							//	CDuiString szDomainMid = GetIconNamebyWebUrl(szUrl.GetData()).GetString();
							////	CDuiString szCom = GetComWebUrl(szUrl);
							//	CDuiString szFind = szDomainMid;
							//	szFind = szFind + _T(".");
							//	//szFind = szFind + szCom;
							//	int nTwoSelPos = -1;
							//	nTwoSelPos = szSelText.Find(szFind);
							//	while(nTwoSelPos!=-1)
							//	{
							//		int nTmpSelPos = szSelText.Find(szText);
							//		if (nTmpSelPos!=-1)
							//		{
							//			nTmpSelPos = nTmpSelPos + szText.GetLength();
							//		}
							//		else
							//		{
							//			m_pOwner->SetSel(szText.GetLength(),szText.GetLength() );
							//			m_pOwner->SetIsSetText(false);
							//			break;
							//		}
							//		szSelText = szSelText.Mid(nTmpSelPos,szSelText.GetLength()- nTmpSelPos);
							//		nTwoSelPos = szSelText.Find(szFind);
							//	}
							//	//szSelText.Find(szText) == -1
							//	szText = szText + szSelText;
							//	m_pOwner->m_sText = szText;
							//	m_pOwner->SetSel(nSelPos,szSelText.GetLength() + nSelPos );
							//}
						}
					}
					m_pOwner->SetSelText(L"");
				}while(0);
				m_pOwner->m_bSetSelText = false;
			}
		}, token);
		return 0;
	}


	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	const std::string CAddressEditUI::DBFile(Core_Instance::instance()->GetRootPath().toString() + "\\url.db");
	CAddressEditUI::CAddressEditUI(CMainFrame* pHost) 
		:m_pHost(pHost)
		,m_pEditWindow(NULL)
		,m_pDropDownWindow(NULL)
		,m_uMaxChar(255)
		,m_bReadOnly(false)
		,m_bSetText(false)
		,m_bSetSelText(false)
		,m_bPasswordMode(false)
		,m_cPasswordChar(_T('*'))
		,m_uButtonState(0)
		,m_dwEditbkColor(0xFFFFFFFF)
		,m_iWindowStyls(0)
		,m_sqlite3Session(SQLite::Connector::KEY, DBFile)
		//,m_pCustomAutoComplete(new CCustomAutoComplete())
	{
		m_sqlite3Session.encryption("1qaz@WSX3edc$RFV5tgb^YHN@xgamer.cn");
		ASSERT(m_pHost);
		SetTextPadding(CDuiRect(4, 3, 4, 3));
		SetBkColor(0xFFFFFFFF);
	}

	CAddressEditUI::~CAddressEditUI()
	{
		//m_pCustomAutoComplete = NULL;
	}

	std::wstring CAddressEditUI::GetIconPath() const
	{
		return A2W_UTF8(Core_Instance::instance()->GetPath("icon").toString());
	}

	void CAddressEditUI::SetKeepFocus(bool bKeepFocus)
	{	
		if (m_pEditWindow)
			m_pEditWindow->SetKeepFocus(bKeepFocus);
	}

	bool CAddressEditUI::IsKeepFocus() const
	{
		return m_pEditWindow ? m_pEditWindow->IsKeepFocus() : false;
	}


	bool CAddressEditUI::UpdateAddrItemData(const CDuiString& sTitle, const CDuiString& sUrl, int index)
	{
		return m_pDropDownWindow ? m_pDropDownWindow->UpdateAddrItemData(sTitle, sUrl, index) : false;
	}

	bool CAddressEditUI::UpdateGameItemData(
		const std::wstring& sId, 
		const std::wstring& sIconPath, 
		const std::wstring& sGameName, 
		const std::wstring& sGameType,
		const std::wstring& sGameDesc,
		int index)
	{
		return m_pDropDownWindow ? m_pDropDownWindow->UpdateGameItemData(sId, sIconPath, sGameName, sGameType, sGameDesc, index) : false;
	}

	CDuiString CAddressEditUI::GetFirstUrl() const
	{
		return m_pDropDownWindow ? m_pDropDownWindow->GetFirstUrl() : _T("");
	}

	int CAddressEditUI::GetDropWndCountOfItems() const
	{
		return m_pDropDownWindow ? m_pDropDownWindow->GetCount() : 0;
	}


	int CAddressEditUI::GetGameHeight() const
	{
		return m_pDropDownWindow ? m_pDropDownWindow->GetGameHeight() : 0;
	}

	std::string CAddressEditUI::GetGameSearchId() const
	{
		return m_pDropDownWindow ? m_pDropDownWindow->GetGameSearchId() : "";
	}

	bool CAddressEditUI::SetSearchText(const CDuiString& sText)
	{
		/*if (!m_pDropDownWindow)
		{
			m_pDropDownWindow = new CAddressDropDownWnd(this);
			m_pDropDownWindow->CreateDuiWindow(m_pManager->GetPaintWindow(), _T(""), WS_POPUP, 0);
			m_pDropDownWindow->ShowWindow(true, false);
			::BringWindowToTop(m_pDropDownWindow->GetHWND());
		}*/

	//	ASSERT(m_pDropDownWindow);
		return  m_pDropDownWindow ? m_pDropDownWindow->SetSearchText(sText) : false;
	}


	void CAddressEditUI::SetGameHeight(int nCount)
	{
		if (m_pDropDownWindow)
		{
			m_pDropDownWindow->SetGameHeight(nCount);
		}
	}

	bool CAddressEditUI::SetDropItemVisible(int nIndex, bool bVisible)
	{
		return m_pDropDownWindow ? m_pDropDownWindow->SetDropItemVisible(nIndex, bVisible) : false;
	}
	
	bool CAddressEditUI::UpDateDropWndRect(int nWidth, int nHeight)
	{
		return m_pDropDownWindow ? m_pDropDownWindow->UpDateDropWndRect(nWidth, nHeight) : false;
	}

	bool CAddressEditUI::UpdateDropWndData(const std::vector<Url>& vec_url)
	{
		return m_pDropDownWindow ? m_pDropDownWindow->UpdateDropWndData(vec_url) : false;
	}



	CDuiString CAddressEditUI::GetSelText() const
	{
		return m_szSelText;
	}

	void CAddressEditUI::SetSelText(LPCTSTR pStrSel)
	{
		m_szSelText = pStrSel;
	}


	LPCTSTR CAddressEditUI::GetClass() const
	{
		return _T("AddressEditUI");
	}

	LPVOID CAddressEditUI::GetInterface(LPCTSTR pstrName)
	{
		if ( _tcscmp(pstrName, _T("AddressEdit")) == 0 ) return static_cast<CAddressEditUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	UINT CAddressEditUI::GetControlFlags() const
	{
		if ( !IsEnabled() ) return CControlUI::GetControlFlags();
		return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
	}

	void CAddressEditUI::DoEvent(TEventUI& event)
	{
		if ( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if ( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if ( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
			return;
		}
		if ( event.Type == UIEVENT_KEYDOWN && IsEnabled() )
		{
			switch (event.chKey)
			{
			case VK_DOWN:
			case VK_UP:
			case VK_TAB:
				{
					if (m_pDropDownWindow)
					{
						m_bSetSelText = true;
						::SendMessage(m_pDropDownWindow->GetHWND(), WM_KEYDOWN, event.chKey, 0);
					}
				}
				break;
			case VK_RETURN:
				m_bSetSelText = true;
				GetManager()->SendNotify(this, DUI_MSGTYPE_RETURN);
				if (m_pEditWindow)
				{
					m_pEditWindow->Close();
				}
				break;
			case 0x41:/*'A'*/
				{
					if (event.wKeyState & MK_CONTROL)
					{
						m_bSetSelText = true;
						this->SetSelAll();
					}
				}
				break;
			case VK_DELETE:
			case VK_BACK:
				m_bSetSelText = true;
				return;
			default:
				m_bSetSelText = false;
				break;
			}
		//	GetManager()->SendNotify(this, DUI_MSGTYPE_KEYDOWN, event.wParam, event.lParam);
		}

		if ( event.Type == UIEVENT_WINDOWSIZE )
		{
			if ( m_pEditWindow != NULL ) m_pManager->SetFocusNeeded(this);
		}
		if ( event.Type == UIEVENT_SCROLLWHEEL )
		{
			if ( m_pEditWindow != NULL ) return;
		}
		if ( event.Type == UIEVENT_SETFOCUS && IsEnabled() ) 
		{ 
			if (!m_pDropDownWindow)
			{
				m_pDropDownWindow = new CAddressDropDownWnd(this);
				m_pDropDownWindow->CreateDuiWindow(m_pManager->GetPaintWindow(), _T(""), WS_POPUP, 0);
				m_pDropDownWindow->ShowWindow(true, false);
				::BringWindowToTop(m_pDropDownWindow->GetHWND());
			}

			if (!m_pEditWindow ) 
			{
				m_pEditWindow = new CAddressEditWnd();
				ASSERT(m_pEditWindow);
				m_pEditWindow->Init(this);
				Invalidate();
			}
			else
			{
				SetKeepFocus(true);
				::SetFocus(*m_pEditWindow);
			}
			
		}
		if ( event.Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
		{
			Invalidate();
		}
		if ( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN) 
		{
			if ( IsEnabled() ) {
				GetManager()->ReleaseCapture();
				if ( IsFocused()  )
				{
					if (!m_pDropDownWindow)
					{
						m_pDropDownWindow = new CAddressDropDownWnd(this);
						m_pDropDownWindow->CreateDuiWindow(m_pManager->GetPaintWindow(), _T(""), WS_POPUP, 0);
						m_pDropDownWindow->ShowWindow(true, false);
						::BringWindowToTop(m_pDropDownWindow->GetHWND());
					}

					if (!m_pEditWindow ) 
					{
						m_pEditWindow = new CAddressEditWnd();
						ASSERT(m_pEditWindow);
						m_pEditWindow->Init(this);
						Invalidate();
						SetSelAll();
					}
					else
					{
						SetSelAll();
						SetKeepFocus(true);
					
					}
				
				}
				else if ( m_pEditWindow != NULL )
				{
					SetKeepFocus(true);
#if 1
					int nSize = GetWindowTextLength(*m_pEditWindow);
					if ( nSize == 0 )
						nSize = 1;

					Edit_SetSel(*m_pEditWindow, 0, nSize);
					::OutputDebugString(_T("UIEVENT_BUTTONDOWN    Nofocus\n"));
#else
					POINT pt = event.ptMouse;
					pt.x -= m_rcItem.left + m_rcTextPadding.left;
					pt.y -= m_rcItem.top + m_rcTextPadding.top;
					::SendMessage(*m_pEditWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
#endif
				}
			}
			return;
		}
		if ( event.Type == UIEVENT_MOUSEMOVE ) 
		{
			return;
		}
		if ( event.Type == UIEVENT_BUTTONUP ) 
		{
			return;
		}
		if ( event.Type == UIEVENT_CONTEXTMENU )
		{
			return;
		}
		if ( event.Type == UIEVENT_MOUSEENTER )
		{
			if ( IsEnabled() ) {
				m_uButtonState |= UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if ( event.Type == UIEVENT_MOUSELEAVE )
		{
			if ( IsEnabled() ) {
				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		CLabelUI::DoEvent(event);
	}

	void CAddressEditUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if ( !IsEnabled() ) {
			m_uButtonState = 0;
		}
	}

	void CAddressEditUI::SetText(LPCTSTR pstrText)
	{
		m_sText = pstrText;
		if ( m_pEditWindow != NULL ) 
		{
			Edit_SetText(*m_pEditWindow, m_sText);
		}
		Invalidate();
	}

	void CAddressEditUI::SetMaxChar(UINT uMax)
	{
		m_uMaxChar = uMax;
		if ( m_pEditWindow != NULL ) Edit_LimitText(*m_pEditWindow, m_uMaxChar);
	}

	UINT CAddressEditUI::GetMaxChar()
	{
		return m_uMaxChar;
	}

	void CAddressEditUI::SetReadOnly(bool bReadOnly)
	{
		if ( m_bReadOnly == bReadOnly ) return;

		m_bReadOnly = bReadOnly;
		if ( m_pEditWindow != NULL ) Edit_SetReadOnly(*m_pEditWindow, m_bReadOnly);
		Invalidate();
	}

	bool CAddressEditUI::IsReadOnly() const
	{
		return m_bReadOnly;
	}

	void CAddressEditUI::SetNumberOnly(bool bNumberOnly)
	{
		if ( bNumberOnly )
		{
			m_iWindowStyls |= ES_NUMBER;
		}
		else
		{
			m_iWindowStyls |= ~ES_NUMBER;
		}
	}

	bool CAddressEditUI::IsNumberOnly() const
	{
		return m_iWindowStyls&ES_NUMBER ? true:false;
	}

	int CAddressEditUI::GetWindowStyls() const 
	{
		return m_iWindowStyls;
	}

	void CAddressEditUI::SetPasswordMode(bool bPasswordMode)
	{
		if ( m_bPasswordMode == bPasswordMode ) return;
		m_bPasswordMode = bPasswordMode;
		Invalidate();
	}

	bool CAddressEditUI::IsPasswordMode() const
	{
		return m_bPasswordMode;
	}

	void CAddressEditUI::SetPasswordChar(TCHAR cPasswordChar)
	{
		if ( m_cPasswordChar == cPasswordChar ) return;
		m_cPasswordChar = cPasswordChar;
		if ( m_pEditWindow != NULL ) Edit_SetPasswordChar(*m_pEditWindow, m_cPasswordChar);
		Invalidate();
	}

	TCHAR CAddressEditUI::GetPasswordChar() const
	{
		return m_cPasswordChar;
	}

	LPCTSTR CAddressEditUI::GetNormalImage()
	{
		return m_sNormalImage;
	}

	void CAddressEditUI::SetNormalImage(LPCTSTR pStrImage)
	{
		m_sNormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CAddressEditUI::GetHotImage()
	{
		return m_sHotImage;
	}

	void CAddressEditUI::SetHotImage(LPCTSTR pStrImage)
	{
		m_sHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CAddressEditUI::GetFocusedImage()
	{
		return m_sFocusedImage;
	}

	void CAddressEditUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		m_sFocusedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CAddressEditUI::GetDisabledImage()
	{
		return m_sDisabledImage;
	}

	void CAddressEditUI::SetDisabledImage(LPCTSTR pStrImage)
	{
		m_sDisabledImage = pStrImage;
		Invalidate();
	}

	void CAddressEditUI::SetNativeEditBkColor(DWORD dwBkColor)
	{
		m_dwEditbkColor = dwBkColor;
	}

	DWORD CAddressEditUI::GetNativeEditBkColor() const
	{
		return m_dwEditbkColor;
	}

	void CAddressEditUI::SetSel(long nStartChar, long nEndChar)
	{
		if ( m_pEditWindow != NULL ) Edit_SetSel(*m_pEditWindow, nStartChar,nEndChar);
	}

	void CAddressEditUI::SetSelAll()
	{
		SetSel(0,-1);
	}

	void CAddressEditUI::SetReplaceSel(LPCTSTR lpszReplace)
	{
		if ( m_pEditWindow != NULL ) Edit_ReplaceSel(*m_pEditWindow, lpszReplace);
	}

	void CAddressEditUI::SetPos(RECT rc)
	{
		CControlUI::SetPos(rc);
		if ( m_pEditWindow != NULL ) {
			RECT rcPos = m_pEditWindow->CalPos();
			::SetWindowPos(m_pEditWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
				rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);        
		}
	}

	void CAddressEditUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
		if ( !IsVisible() && m_pEditWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	void CAddressEditUI::SetInternVisible(bool bVisible)
	{
		if ( !IsVisible() && m_pEditWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	SIZE CAddressEditUI::EstimateSize(SIZE szAvailable)
	{
		if ( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 6);
		return CControlUI::EstimateSize(szAvailable);
	}

	void CAddressEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if ( _tcscmp(pstrName, _T("readonly")) == 0 ) SetReadOnly(_tcscmp(pstrValue, _T("true")) == 0);
		else if ( _tcscmp(pstrName, _T("numberonly")) == 0 ) SetNumberOnly(_tcscmp(pstrValue, _T("true")) == 0);
		else if ( _tcscmp(pstrName, _T("password")) == 0 ) SetPasswordMode(_tcscmp(pstrValue, _T("true")) == 0);
		else if ( _tcscmp(pstrName, _T("maxchar")) == 0 ) SetMaxChar(_ttoi(pstrValue));
		else if ( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
		else if ( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
		else if ( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
		else if ( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
		else if ( _tcscmp(pstrName, _T("nativebkcolor")) == 0 ) {
			if ( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetNativeEditBkColor(clrColor);
		}
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CAddressEditUI::PaintStatusImage(HDC hDC)
	{
		if ( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if ( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if ( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
			if ( !m_sDisabledImage.IsEmpty() ) {
				if ( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
				else return;
			}
		}
		else if ( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if ( !m_sFocusedImage.IsEmpty() ) {
				if ( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
				else return;
			}
		}
		else if ( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if ( !m_sHotImage.IsEmpty() ) {
				if ( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) m_sHotImage.Empty();
				else return;
			}
		}

		if ( !m_sNormalImage.IsEmpty() ) {
			if ( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) m_sNormalImage.Empty();
			else return;
		}
	}

	void CAddressEditUI::PaintText(HDC hDC)
	{
		if ( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if ( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		if ( m_sText.IsEmpty() ) return;

		CDuiString sText = m_sText;
		if ( m_bPasswordMode ) {
			sText.Empty();
			LPCTSTR p = m_sText.GetData();
			while( *p != _T('\0') ) {
				sText += m_cPasswordChar;
				p = ::CharNext(p);
			}
		}

		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;
		if ( IsEnabled() ) {
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);
		}
		else {
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);

		}
	}


	void CAddressEditUI::DoInit()
	{
		_ASSERT(m_sqlite3Session.isConnected());

	//	Poco::Logger& error = Poco::Logger::get(ErrorLoggerName);
		try
		{
			m_sqlite3Session << "CREATE TABLE IF NOT EXISTS tb_url(url VARCHAR NOT NULL PRIMARY KEY, domain VARCHAR,\
							path VARCHAR, title VARCHAR, fragment VARCHAR)", Poco::Data::now; 
		}
		catch(Poco::Exception& /*e*/)
		{
		//	error.error("try to create table of the name tb_url in CAddressEditUI::DoInit()");
		//	error.error(e.what());
		}

		boost::array<Url, 5> arrUrl = 
		{
			Url(XGAMER_MAIN_HOME_A, "", "", W2A_UTF8(L"X-网游一族"), ""),
			Url(XGAMER_MAIN_ZONE_A, "", "", W2A_UTF8(L"我的空间"), ""),
			Url(XGAMER_MAIN_SETTINGS_A, "", "", W2A_UTF8(L"设置中心"), ""),
			Url(XGAMER_MAIN_ID_A, "", "", W2A_UTF8(L"身份证生成器"), ""),
			Url(XGAMER_MAIN_PROTOCOL_A, "", "", W2A_UTF8(L"Xgamer服务协议"), ""),
		};


		try
		{
			for(auto it=arrUrl.begin(); it!=arrUrl.end(); ++it)
			{
				m_sqlite3Session << "INSERT INTO tb_url VALUES(:url, :domain, :path, :title, :fm)" ,
					use(*it),   now;
			}
		}
		catch (Poco::Exception&)
		{
		}

		//std::vector<std::string> urls;
		//m_sqlite3Session << "SELECT url FROM tb_url", into(urls), Poco::Data::now; 
		//for (auto url : urls)
		//{
		//	m_pCustomAutoComplete->AddItem(CString(A2W_UTF8(url).c_str()));
		////	m_pCustomAutoComplete->AddItem(
		//}
	}

	///
	/// CAddressDropDownWnd
	///
	
	CAddressDropDownWnd::CAddressDropDownWnd(CAddressEditUI* pOwner)
		:m_pOwner(pOwner)
		,m_pBuilder(new CDialogBuilder)
		,m_pMainList(NULL)
		,m_gameHeight(0)
		,m_bIsShowWindow(false)
		,m_bIsTextEditOK(false)
	{

	}

	CAddressDropDownWnd::~CAddressDropDownWnd()
	{
		if (m_pBuilder)
			delete m_pBuilder;
		m_pOwner->m_pDropDownWindow = NULL;
	}

	
	void CAddressDropDownWnd::OnFinalMessage( HWND hWnd )
	{
		WindowImplBase::OnFinalMessage(hWnd);
		delete this;
	}



	LRESULT CAddressDropDownWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (uMsg == WM_KEYDOWN)
		{
			switch (wParam)
			{
			case VK_DOWN:
				{
					m_pMainList->SelectItem(m_pMainList->FindSelectable(m_pMainList->GetCurSel() + 1, true), false);
				}
				break;
			case VK_UP:
				{
					m_pMainList->SelectItem(m_pMainList->FindSelectable(m_pMainList->GetCurSel() - 1, false), false);
				}
				break;
			case VK_TAB:
				{
					if (::GetKeyState(VK_SHIFT) >= 0)
					{
						SendMessage(WM_KEYDOWN, VK_DOWN, 0);
					}
					else
					{
						SendMessage(WM_KEYDOWN, VK_UP, 0);
					}
				}
				break;
			default:
				return 0;
			}
			bHandled = TRUE;
			return S_OK;
		}
		//if (uMsg == 
		//{
		//	bHandled = TRUE;
		//	//::PostMessage(GetParent(m_hWnd),WM_USER_SET_SEL_ADDR_EDIT_TEXT,0,0);
		//	return 0;
		//}
		//
		//if (/*uMsg == WM_MOUSELEAVE||*/uMsg == WM_KILLFOCUS)
		//{
		//	// 		if (m_hWnd)
		//	// 		{
		//	// 			DestroyWindow(m_hWnd);
		//	// 		}
		//	bHandled = TRUE;
		//}
		//if (uMsg == WM_SHOWWINDOW)
		//{
		//	if (wParam == 1)
		//	{
		//		//	SetFocus(m_hWnd);
		//	}
		//	if (wParam == 0)
		//	{
		//		if (m_bIsClearItem)
		//		{
		//			m_bIsClearItem = !m_bIsClearItem;
		//			return S_OK;
		//		}
		//		ClearItem();
		//	}
		//}
		return S_OK;
	}

	LRESULT CAddressDropDownWnd::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		if (m_pOwner && m_pOwner->m_pEditWindow)
		{
			bHandled = TRUE;
		}
		return S_OK;
	}


	//LRESULT CAddressDropDownWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	//{
	//	bHandled = TRUE;
	//	if (m_pOwner && m_pOwner->m_pEditWindow)
	//	{
	//		if (wParam == (ULONG)(m_hWnd))
	//		{
	//			return S_OK;
	//		}
	//	}
	//	Close();
	//	return S_OK;
	//}

	LPCTSTR CAddressDropDownWnd::GetWindowClassName() const
	{
		return _T("AddressDropDownWnd");
	}			

	RECT CAddressDropDownWnd::CalPos()
	{
		static CContainerUI* pParent = static_cast<CContainerUI*>(m_pOwner->GetParent()->GetParent());
		ASSERT(pParent);
		CDuiRect rcPos = pParent->GetPos();
		RECT rcInset = pParent->GetInset();
		rcPos.left += rcInset.left;
		rcPos.top += rcInset.top;
		rcPos.right -= rcInset.right;
		rcPos.bottom -= rcInset.bottom;
		return rcPos;
	}

	void CAddressDropDownWnd::InitWindow()
	{
		m_pMainList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("list_addr")));
		ASSERT(m_pMainList);

		CAddrListItem* pAddressListItem = NULL;
		for(int i=0; i<9; ++i)
		{
			if ( !m_pBuilder->GetMarkup()->IsValid() ) {
				pAddressListItem = static_cast<CAddrListItem*>(m_pBuilder->Create(DIALOG_BUILDER_FILE_ADDRESS_BAR_ITEM, (UINT)0, this, &m_PaintManager));
			}
			else {
				pAddressListItem = static_cast<CAddrListItem*>(m_pBuilder->Create(this, &m_PaintManager));
			}
			if (pAddressListItem == NULL)
				return ;
			m_pMainList->Add(pAddressListItem);
		}
		//ready for baidu search
		CDialogBuilder builder;
		CSearchItem* pSearchItem = static_cast<CSearchItem*>(builder.Create(_T("ui_address_bar_search.xml"), (UINT)0, this, &m_PaintManager));
		if (pSearchItem == NULL)
			return ;
		m_pMainList->Add(pSearchItem);

		//prepare 3 game search item
		CDialogBuilder gameBuilder;
		CGameItem* pGameItem = NULL; 
		for(int i=0; i<3; ++i)
		{
			if ( !gameBuilder.GetMarkup()->IsValid() ) {
				pGameItem = static_cast<CGameItem*>(gameBuilder.Create(_T("ui_address_bar_game.xml"), (UINT)0, this, &m_PaintManager));
			}
			else {
				pGameItem = static_cast<CGameItem*>(gameBuilder.Create(this, &m_PaintManager));
			}
			if (pGameItem == NULL)
				return ;

			m_pMainList->Add(pGameItem);
			//初始化时注意， 将游戏item visible = false
			//在断网情况下， 会导致vk_down不循环
			pGameItem->SetVisible(false);
		}

		RECT rcPos = CalPos();

		POINT pt = {rcPos.left, rcPos.bottom};
		::ClientToScreen(m_pOwner->m_pHost->GetHWND(), &pt);
		MoveWindow(m_hWnd, pt.x, pt.y, 0, 0, FALSE);
	}


	void CAddressDropDownWnd::Notify(TNotifyUI& msg)
	{
		if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
		{
			OnPrepare(msg);
			return;
		}
		if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
		{
			OnSelect(msg);
		}
		else if (msg.sType == DUI_MSGTYPE_ITEMCLICK)
		{
			OnItemClick(msg);
		}
		else if (msg.sType == DUI_MSGTYPE_CLICK)
		{
			OnDelete(msg);
		}
	}

	void CAddressDropDownWnd::OnPrepare(TNotifyUI& msg)
	{
	}

	void CAddressDropDownWnd::OnSelect(TNotifyUI& msg)
	{
		CControlUI* pControl = m_pMainList->GetItemAt(m_pMainList->GetCurSel());
		if (pControl)
		{
			if (_tcscmp(pControl->GetClass(), _T("AddressListItem")) == 0)
			{
				m_editText = static_cast<CAddrListItem*>(pControl)->GetUrl();
			}
			else if (_tcscmp(pControl->GetClass(), _T("SearchItem")) == 0)
			{
				m_editText = static_cast<CSearchItem*>(pControl)->GetUrl();
			}
			else if (_tcscmp(pControl->GetClass(), _T("GameItem")) == 0)
			{
				m_editText = static_cast<CGameItem*>(pControl)->GetUrl().c_str();
				m_gameId = W2A_UTF8(static_cast<CGameItem*>(pControl)->GetGameId());
			}
			m_pOwner->SetIsSetText(true);
			m_pOwner->SetText(m_editText);
			m_pOwner->SetIsSetText(false);
			m_pOwner->SetSelAll();
		}
	}

	void CAddressDropDownWnd::OnItemClick(TNotifyUI& msg)
	{
		int sel = m_pMainList->GetItemIndex(msg.pSender);
		if (_tcscmp(msg.pSender->GetClass(), _T("AddressListItem")) == 0)
		{
			CAddrListItem* pSel = static_cast<CAddrListItem*>(m_pMainList->GetItemAt(sel));
			if (pSel)
			{
				m_editText = pSel->GetUrl();
			}
		}
		else if (_tcscmp(msg.pSender->GetClass(), _T("SearchItem")) == 0)
		{
			CSearchItem* pSel = static_cast<CSearchItem*>(m_pMainList->GetItemAt(sel));
			if (pSel)
			{
				m_editText = pSel->GetUrl();
			}
		}
		else if (_tcscmp(msg.pSender->GetClass(), _T("GameItem")) == 0)
		{
			m_gameId = W2A_UTF8(static_cast<CGameItem*>(msg.pSender)->GetGameId());
			::SendMessage(GetParent(m_hWnd),WM_GO_GAME_SEARCH , 0, 0);
			m_editText = XGAMER_MAIN_HOME_W;
		}
		m_pOwner->SetText(m_editText);

		TEventUI event = {0};
		event.Type = UIEVENT_KEYDOWN;
		event.chKey = VK_RETURN;
		m_pOwner->Event(event);
	}

	void CAddressDropDownWnd::OnDelete(TNotifyUI& msg)
	{
		CControlUI* pControl = msg.pSender;

		while(_tcscmp(pControl->GetClass(), _T("AddressListItem")) != 0 )
		{
			pControl = pControl->GetParent();
			if (!pControl)
				return;
		}

		CAddrListItem* pItem = static_cast<CAddrListItem*>(pControl);
		/*CDuiString szTitle(pItem->GetTitle());
		CDuiString szUrl(pItem->GetUrl());*/

		std::string szUrl(W2A_UTF8(pItem->GetUrl().GetData()));
		std::string szTitle(W2A_UTF8(pItem->GetTitle().GetData()));

		Session& session = m_pOwner->GetSession();
		session << "DELETE FROM tb_url WHERE url=:url AND title=:title", use(szUrl), use(szTitle), now;


		LPCTSTR pstr = m_pOwner->GetText().GetData();

		std::vector<Url> vec_url;

		CDuiString szSql;
		szSql.Format(_T("select * from tb_url where url like '%%%s%%' order by case when url='%s' then 10 else 0 end desc,case when path='%s' then 9 else 0 end desc,case when path like'%%%s%%' then 8 else 0 end desc,case when domain='%s' then 7 else 0 end desc,length(url) limit 9"),
			pstr,pstr,pstr,pstr,pstr);

		session << W2A_UTF8(szSql.GetData()), into(vec_url), now;
		//send textchange 
		//for rop get app name
		int nHeight = vec_url.size() * 47/*地址栏搜索47/个*/ + 2/*上下边框一像素*/ + 26/*给默认搜索留的*/;
		nHeight += m_pOwner->GetGameHeight();
		nHeight += 6;

		static CContainerUI* pParent = static_cast<CContainerUI*>(m_pOwner->GetParent()->GetParent());
		RECT rc = pParent->GetPos();
		CDuiRect inset(pParent->GetInset());
		POINT pt = {rc.left+inset.left, rc.bottom+1};
		ClientToScreen(m_hWnd, &pt);
		m_pOwner->UpdateDropWndData(vec_url);
		m_pOwner->UpDateDropWndRect(rc.right-rc.left-inset.left, nHeight);
		m_pOwner->GetManager()->SetFocus(NULL);
		m_pOwner->GetManager()->SetFocus(m_pOwner);
	}

	CControlUI* CAddressDropDownWnd::CreateControl(LPCTSTR pstrClass)
	{
		if (_tcscmp(pstrClass, _T("AddressListItem")) == 0)
		{
			return new CAddrListItem;
		}
		else if (_tcscmp(pstrClass, _T("SearchItem")) == 0)
		{
			return new CSearchItem;
		}
		else if (_tcscmp(pstrClass, _T("GameItem")) == 0)
		{
			return new CGameItem;
		}
		return NULL;
	}


	bool CAddressDropDownWnd::UpdateAddrItemData(const CDuiString& sTitle, const CDuiString& sUrl, int index)
	{
		CAddrListItem* pResetItem = static_cast<CAddrListItem*>(m_pMainList->GetItemAt(index));
		if (pResetItem)
		{
			pResetItem->ReSetData(CalIconPath(sUrl), sTitle, sUrl);
			if (index == 0)
			{
				m_szFirstUrl = pResetItem->GetUrl();
			}
			return true;
		}
		return false;
		//may be null
	}

	bool CAddressDropDownWnd::SetSearchText(const CDuiString& sText)
	{
		CSearchItem* pResetItem = static_cast<CSearchItem*>(m_pMainList->GetItemAt(9));
		if (pResetItem)
		{
			pResetItem->SetSearchText(sText);
			return true;
		}
		return false;
	}

	void CAddressDropDownWnd::SetGameHeight(int count)
	{
		if (!m_pMainList)
		{// 程序启动太卡时， 会在此崩溃
			return;
		}
		//这里同时设置 GameItem visible
		//由于 address edit处理vk_tab\vk_up\vk_down
		//在调用m_pMainList->findselectable时，
		//如果控件visble=true会判断可选，进一步影响操作的流畅性
		ASSERT(count >= 0 && count <= 3);
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		long newHeight = rc.bottom - rc.top - m_gameHeight;
		m_gameHeight = 52 * count;

		int nVisbleMax = m_pMainList->GetCount()-(3-count);
		for(int i= 10; i<nVisbleMax; ++i)
		{
			m_pMainList->GetItemAt(i)->SetVisible(true);
		}

		for(int i=nVisbleMax; i<m_pMainList->GetCount(); ++i)
		{
			m_pMainList->GetItemAt(i)->SetVisible(false);
		}

		newHeight += m_gameHeight;
		ResizeClient(-1, newHeight);  
	}

	void CAddressDropDownWnd::SetSearchExtraInfo(const CDuiString& sHead, const CDuiString& sTail)
	{	
		CSearchItem* pResetItem = static_cast<CSearchItem*>(m_pMainList->GetItemAt(9));
		if (pResetItem)
		{
			pResetItem->SetSearchExtraInfo(sHead, sTail);
		}
	}

	CDuiString CAddressDropDownWnd::CalIconPath(const CDuiString& sUrl)
	{
		CUrl tmpUrl;
		if (!tmpUrl.CrackUrl(sUrl))
		{
			CDuiString sUrlFul(sUrl);
			sUrlFul.Assign(_T("http"), 0);
			if (!tmpUrl.CrackUrl(sUrlFul))
				return CDuiString(_T(""));
		}
		return (m_pOwner->GetIconPath() + GetIconByDomainName(tmpUrl.GetHostName())).c_str();
	}

	inline int CAddressDropDownWnd::GetCount()
	{
		return m_pMainList->GetCount();
	}
	
	inline std::string CAddressDropDownWnd::GetGameSearchId() const
	{
		return m_gameId;
	}


	inline CDuiString CAddressDropDownWnd::GetSkinFile()
	{
		return DIALOG_SKIN_FILE_POPUP_ADDRESS_BAR_NAME;
	}

	inline CDuiString CAddressDropDownWnd::GetSkinFolder()
	{
		return DIALOG_SKIN_FOLDER_NAME;
	}

	inline int CAddressDropDownWnd::GetGameHeight() const
	{
		return m_gameHeight;
	}

	bool CAddressDropDownWnd::SetDropItemVisible(int nIndex, bool bVisible)
	{
		CControlUI* pControl = m_pMainList->GetItemAt(nIndex);
		if (pControl)
		{
			pControl->SetVisible(bVisible);
		}
		return false;
	}
	
	bool CAddressDropDownWnd::UpdateGameItemData(
			const std::wstring& sId, 
			const std::wstring& sIconPath, 
			const std::wstring& sGameName, 
			const std::wstring& sGameType,
			const std::wstring& sGameDesc,
			int index)
	{
		int i = index + 10;
		CGameItem* pGameItem = static_cast<CGameItem*>(m_pMainList->GetItemAt(i));
		m_gameHeight = pGameItem->GetFixedHeight() * (index + 1);
		if (pGameItem)
		{
		/*	m_pMainList->Remove(pGameItem);
			
			CDialogBuilder builder;
			CDialogBuilder gameBuilder;
			CGameItem* pNewGameItem = NULL; 
			if ( !gameBuilder.GetMarkup()->IsValid() ) {
					pNewGameItem = static_cast<CGameItem*>(gameBuilder.Create(_T("ui_address_bar_game.xml"), (UINT)0, this, &m_PaintManager));
				}
				else {
				pNewGameItem = static_cast<CGameItem*>(gameBuilder.Create(this, &m_PaintManager));
			}
			if (pNewGameItem == NULL)
				return false;
			m_pMainList->AddAt(pNewGameItem, i);*/
			pGameItem->InitData(sId, sIconPath, sGameName, sGameType, sGameDesc);
			return true;
		}
		return false;
	}

	bool CAddressDropDownWnd::UpDateDropWndRect(int nWidth, int nHeight)
	{
		ResizeClient(nWidth, nHeight);
		return true;
	}

	bool CAddressDropDownWnd::UpdateDropWndData(const std::vector<Url>& vec_url)
	{
		int i = 0;
		for(auto it=vec_url.begin(); it!=vec_url.end() && i < 9; ++it, ++i)
		{
			const CDuiString sUrl(A2W_UTF8(it->_url).c_str());
			const CDuiString sTitle(A2W_UTF8(it->_title).c_str());
			static_cast<CAddrListItem*>(m_pMainList->GetItemAt(i))->ReSetData(CalIconPath(sUrl), sTitle, sUrl);
		}

		for(; i < 9; ++i)
		{
			m_pMainList->GetItemAt(i)->SetVisible(false);
		}

	//	m_pMainList->SelectItem(0);
		return true;
	}

	CDuiString CAddressDropDownWnd::GetFirstUrl() const
	{
		return static_cast<CAddrListItem*>(m_pMainList->GetItemAt(0))->GetUrl();
	}

}
