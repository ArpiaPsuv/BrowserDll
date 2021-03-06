#pragma once

#include <OAIdl.h>

namespace MyWeb
{
    class MyDispatchHost
    {
    public:
        virtual ~MyDispatchHost();
        virtual STDMETHODIMP AllowFocusChange(/* [out] */ __RPC__out BOOL *pfAllow);

        virtual void OnEventOnClick();
        virtual void DownloadBegin();
        virtual void DownloadComplete();
        virtual void WindowSetHeight(LONG height);
        virtual void WindowSetWidth(LONG width);
        virtual void WindowSetTop(LONG Top);
        virtual void WindowSetLeft(LONG Left);
        virtual void TitleChange2(BSTR Text);
        virtual void FileDownload(VARIANT_BOOL * ActiveDocument, VARIANT_BOOL * Cancel);
        virtual void StatusTextChange2(BSTR Text);
        virtual void ProgressChange2(long Progress, long ProgressMax);
        virtual void DocumentComplete(IDispatch* pDisp, VARIANT* pvURL);
        virtual void NavigateComplete2(IDispatch* pDisp, VARIANT* pvURL);
        virtual void ClientToHostWindow(LONG * CX, LONG * CY);
        virtual void SetSecureLockIcon(LONG SecureLockIcon);
        virtual void NavigateError(IDispatch * pDisp, VARIANT * URL, VARIANT * TargetFrameName, VARIANT * StatusCode, VARIANT_BOOL * Cancel);
        virtual void PrivacyImpactedStateChange(VARIANT_BOOL PrivacyImpacted);
        virtual void WindowSetResizable(VARIANT_BOOL Resizable);

        virtual void CommandStateChange2(long Command, VARIANT_BOOL Enable);
        virtual void WindowClosing(VARIANT_BOOL IsChildWindow, VARIANT_BOOL* Cancel);
        virtual void NewWindow2(IDispatch * * ppDisp, VARIANT_BOOL * Cancel);
        virtual void NewWindow3(IDispatch** ppDisp, VARIANT_BOOL* Cancel, unsigned long dwFlags, BSTR bstrUrlContext, BSTR bstrUrl);
        virtual void BeforeNavigate2(IDispatch* pDisp, VARIANT* pvURL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel);
    };
}