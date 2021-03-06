#pragma once

class CMainFrame;

namespace Xgamer{
	namespace UI{

		class CAddrListItem : public CListContainerElementUI
		{
		public:
			explicit CAddrListItem();
			~CAddrListItem();

			LPCTSTR GetClass() const;
			LPVOID GetInterface(LPCTSTR pstrName);
			
		//	void SetInit(const CDuiString& sIcon, const CDuiString& sTitle, const CDuiString& sUrl);
			void ReSetData(const CDuiString& sIcon, const CDuiString& sTitle, const CDuiString& sUrl);
			void DoEvent(TEventUI& event);
			CDuiString GetUrl() const;
			CDuiString GetIcon() const;
			CDuiString GetTitle() const;

		private:
			void DoInit();

			CContainerUI* m_pCloseLayout;
			
			CIconUI* m_pIcon;
			CLabelUI* m_pTitle;
			CLabelUI* m_pUrl;
		};


		//search item
		class CSearchItem : public CListContainerElementUI
		{
		public:
			explicit CSearchItem();
			~CSearchItem();

			LPCTSTR GetClass() const;
			LPVOID GetInterface(LPCTSTR pstrName);

			void DoEvent(TEventUI& event);

			void SetSearchText(const CDuiString& sText);
			CDuiString GetUrl() const;

			static void SetSearchExtraInfo(const CDuiString& sHead, const CDuiString& sTail);
			//used to convert text
			static CDuiString Convert(const CDuiString& sText);
		private:
			void DoInit();
			CLabelUI* m_pTitle;
			CDuiString m_sSearchText;
			//
			static CDuiString sm_sHead;
			static CDuiString sm_sTail;
		};

		//game item
		class CGameItem : public CListContainerElementUI
		{
		private:
			struct GameData
			{
				std::wstring _sId;
				std::wstring _sIconPath;
				std::wstring _sGameName;
				std::wstring _sGameType;
				std::wstring _sGameDesc;
			};
		public:
			explicit CGameItem();
			~CGameItem();

			LPCTSTR GetClass() const;
			LPVOID GetInterface(LPCTSTR pstrName);
			void DoEvent(TEventUI& event);

			// sIconPath---{http://www.xgamer.cn/.....} south path
			void InitData(const std::wstring& sId, const std::wstring& sIconPath
				, const std::wstring& sGameName, const std::wstring& sGameType
				, const std::wstring& sGameDesc);
			
			std::wstring GetGameId() const;
			// Get the local downloading icon path 
			// eg: c:/user/...
			std::wstring CGameItem::GetUrl() const;
			std::wstring GetIconPath() const;

			//void PaintText(HDC hDC);
			//static void SetSearchExtraInfo(const CDuiString& sHead, const CDuiString& sTail);
			//used to convert text
			//Return a copy CDuiString
		private:
			void DoInit();
			CLabelUI* m_pTitle;
			CLabelUI* m_pDesc;
			CLabelUI* m_pImg;

			GameData m_data;
			static const std::string sm_sIconPath;

			std::pair<concurrency::cancellation_token_source, concurrency::cancellation_token_source> m_pair_cts;
	/*		static CDuiString m_sHead;
			static CDuiString m_sTail;*/
		};

	}//UI
}//Xgamer
