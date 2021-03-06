#pragma once

#include "MyString.h"
#include <memory>
#include <thread>
#include <ExDisp.h>
#include <Mshtml.h>
#include <MsHtmHst.h>
#include <map>

#include "MyDispatchHost.h"

namespace MyWeb
{
    struct TExternalItem;
    class JSExternal;
    class MyBrowserCtrl : public MyDispatchHost
    {
        friend class CActiveXCtrl;

        struct TData
        {
            HANDLE _hThreadHandle;
            DWORD _dwThreadId;
            std::shared_ptr<MyBrowserCtrl> _spBrowserCtrl;
        };
    public:

        explicit MyBrowserCtrl(HWND hBindWnd);

        ~MyBrowserCtrl();

        void MessageLoopThread();

        bool HandleCustomThreadMsg(const MSG& msg);

        void WaitThreadMsgQueueCreate();

        HANDLE GetThreadHandle() const;
        DWORD GetThreadId() const;

        void SetBindWindow(HWND hBindWnd);
        HWND GetBindWindow() const;

        void SetHostWindow(HWND hHostWnd);
        HWND GetHostWindow() const;

        void NavigateUrl(const MyString& url);
        void ExecuteJscode(const MyString& jscode);

        void RegisterMethod(const std::shared_ptr<TExternalItem>& spExternalItem);
        void UnregisterMethod(const MyString& methodName);

        bool CreateControl(const CLSID clsid);
        bool CreateControl(LPCTSTR pstrCLSID);

        HRESULT RegisterEventHandler(BOOL inAdvise);

        virtual HRESULT GetExternalCall(LPVOID* ppRet);

        virtual STDMETHODIMP QueryInterface(REFIID iid, void ** ppvObject);
        /*virtual HRESULT Download(IMoniker *pmk,IBindCtx *pbc,DWORD dwBindVerb,LONG grfBINDF,BINDINFO *pBindInfo,
        LPCOLESTR pszHeaders,LPCOLESTR pszRedir,UINT uiCP);*/

        void NavigateComplete2(IDispatch * pDisp, VARIANT * pvURL);

        CLSID GetClisd() const;
      
        void SetVisible(bool bVisible = true);
        void SetPos(RECT rc);
        RECT GetPos() const;

        void SetFocus();

        LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    private:

        bool GetHTMLWindow(CComPtr<IHTMLWindow2>& spHtmlWindow);

    public:
        static int CreateBrowserCtrl(HWND hBindWnd);
        static void DestroyBrowserCtrl(int nIndex);
        static void SetBrowserCtrlPos(int nIndex, RECT rc);
        static void BrowserCtrlNavigateUrl(int nIndex, const MyString& url);
        static void BrowserCtrlExecuteJscode(int nIndex, const MyString& jscode);
        static bool BrowserCtrlRegisterMethod(int nIndex, const TExternalItem& externalItem);
    private:
        virtual void ReleaseControl();
        virtual bool DoCreateControl();

    private:
        static int sm_nIndex;
        static std::map<int, TData> sm_spBrowserCtrls;

        CLSID m_clsid;
        RECT m_rcItem;

        DWORD m_dwCookie;

        IOleObject* m_pUnk;
        CActiveXCtrl* m_pControl;
        CComPtr<IWebBrowser2> m_spWebBrowser2;
        CComPtr<IDispatch> m_spEventDispatch;
        CComPtr<IDocHostUIHandler> m_spDocHostUIHandler;
        CComPtr<JSExternal> m_spJsExternal;

        HWND m_hBindWnd;
        HWND m_hHostWnd;
        bool m_bCreated;

        HANDLE m_hInitEvent;
        DWORD m_dwWebWorkThreadId;
    };
}

