
#pragma once
class CMainFrame;
 
struct Url
{
	std::string _url;
	std::string _domain;
	std::string _path;
	std::string _title;
	std::string _fragment;

	Url()
		:_url("http://www.xgamer.cn/")
	{

	}


	Url(const std::string& url, const std::string& domain, const std::string& path, 
		const std::string& title, const std::string& fragment)
		:_url(url)
		,_domain(domain)
		,_path(path)
		,_title(title)
		,_fragment(fragment)
	{
	}

	bool operator==(const Url& rhs) const
	{
		return _url == rhs._url && _title == rhs._title;
	}

	bool operator < (const Url& rhs) const
	{
		return  _url.length() < rhs._url.length();
	}

	const std::string& operator () () const
		/// This method is required so we can extract data to a map!
	{
		// we choose the url as examplary key
		return _url;
	}
};

class CCustomAutoComplete;
namespace DuiLib
{
	class CAddressEditWnd;
	class CAddressDropDownWnd;

	class CAddressEditUI : public CLabelUI
	{
		friend class CAddressEditWnd;
		friend class CAddressDropDownWnd;

		static const std::string DBFile;
	public:
		CAddressEditUI(CMainFrame* pHost);
		~CAddressEditUI();

		std::wstring GetIconPath() const;
		void SetKeepFocus(bool bKeepFocus);
		bool IsKeepFocus() const;

		CDuiString GetFirstUrl() const;
		int GetDropWndCountOfItems() const;
		int GetGameHeight() const;
		void SetGameHeight(int nCount);
		std::string GetGameSearchId() const;

		bool SetSearchText(const CDuiString& sText);

		bool SetDropItemVisible(int nIndex, bool bVisible);
		bool UpdateAddrItemData(const CDuiString& sTitle, const CDuiString& sUrl, int index);
		bool UpdateGameItemData(
			const std::wstring& sId, 
			const std::wstring& sIconPath, 
			const std::wstring& sGameName, 
			const std::wstring& sGameType,
			const std::wstring& sGameDesc,
			int index);

		bool UpDateDropWndRect(int nWidth, int nHeight);
		bool UpdateDropWndData(const std::vector<Url>& vec_url);

		void SetIsSetText(bool bSetText);
		bool IsSetText() const;
		bool IsSetSelText() const;
	public:

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetEnabled(bool bEnable = true);
		void SetText(LPCTSTR pstrText);
		void SetMaxChar(UINT uMax);
		UINT GetMaxChar();
		void SetReadOnly(bool bReadOnly);
		bool IsReadOnly() const;
		void SetPasswordMode(bool bPasswordMode);
		bool IsPasswordMode() const;
		void SetPasswordChar(TCHAR cPasswordChar);
		TCHAR GetPasswordChar() const;
		void SetNumberOnly(bool bNumberOnly);
		bool IsNumberOnly() const;
		int GetWindowStyls() const;

		LPCTSTR GetNormalImage();
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage();
		void SetHotImage(LPCTSTR pStrImage);
		LPCTSTR GetFocusedImage();
		void SetFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetDisabledImage();
		void SetDisabledImage(LPCTSTR pStrImage);
		void SetNativeEditBkColor(DWORD dwBkColor);
		DWORD GetNativeEditBkColor() const;

		CDuiString GetSelText()const;
		void SetSelText(LPCTSTR pStrSel);
		void SetSel(long nStartChar, long nEndChar);
		void SetSelAll();
		void SetReplaceSel(LPCTSTR lpszReplace);

		void SetPos(RECT rc);
		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		SIZE EstimateSize(SIZE szAvailable);
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintStatusImage(HDC hDC);
		void PaintText(HDC hDC);
		//sqlite3

		Poco::Data::Session& GetSession();
	protected:

		void DoInit();


		CMainFrame* m_pHost;

		CAddressEditWnd* m_pEditWindow;
		CAddressDropDownWnd* m_pDropDownWindow;
		CCustomAutoComplete* m_pCustomAutoComplete;

		UINT m_uMaxChar;
		bool m_bReadOnly;
		bool m_bPasswordMode;
		TCHAR m_cPasswordChar;
		UINT m_uButtonState;
		CDuiString m_sNormalImage;
		CDuiString m_sHotImage;
		CDuiString m_sFocusedImage;
		CDuiString m_sDisabledImage;
		DWORD m_dwEditbkColor;
		int m_iWindowStyls;
		bool m_bIsPopMenu;
		//用于判断是否为SetText;
		//即使区分是否需要发送textchanged 消息
		bool m_bSetText;
		bool m_bSetSelText;
		CDuiString m_szSelText;

	private:
		Poco::Data::Session m_sqlite3Session;

	};

	///
	/// inline
	///

	inline Poco::Data::Session& CAddressEditUI::GetSession()
	{
		return m_sqlite3Session;
	}

	inline void CAddressEditUI::SetIsSetText(bool bSetText)
	{
		m_bSetText = bSetText;
	}
	
	inline bool CAddressEditUI::IsSetText() const
	{
		return  m_bSetText;
	}

	inline bool CAddressEditUI::IsSetSelText() const
	{
		return  m_bSetSelText;
	}

}