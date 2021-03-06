#include "stdafx.h"
#include "MyDispatchHost.h"

namespace MyWeb
{
    MyDispatchHost::~MyDispatchHost() {}
    HRESULT MyDispatchHost::AllowFocusChange(BOOL * pfAllow) { return E_NOTIMPL; }
    void MyDispatchHost::OnEventOnClick() {}
    void MyDispatchHost::DownloadBegin() {}
    void MyDispatchHost::DownloadComplete() {}
    void MyDispatchHost::WindowSetHeight(LONG height) {}
    void MyDispatchHost::WindowSetWidth(LONG width) {}
    void MyDispatchHost::WindowSetTop(LONG Top) {}
    void MyDispatchHost::WindowSetLeft(LONG Left) {}
    void MyDispatchHost::TitleChange2(BSTR Text) {}
    void MyDispatchHost::FileDownload(VARIANT_BOOL * ActiveDocument, VARIANT_BOOL * Cancel) {}
    void MyDispatchHost::StatusTextChange2(BSTR Text) {}
    void MyDispatchHost::ProgressChange2(long Progress, long ProgressMax) {}
    void MyDispatchHost::DocumentComplete(IDispatch * pDisp, VARIANT * pvURL) {}
    void MyDispatchHost::NavigateComplete2(IDispatch * pDisp, VARIANT * pvURL) {}
    void MyDispatchHost::ClientToHostWindow(LONG * CX, LONG * CY) {}
    void MyDispatchHost::SetSecureLockIcon(LONG SecureLockIcon) {}
    void MyDispatchHost::NavigateError(IDispatch * pDisp, VARIANT * URL, VARIANT * TargetFrameName, VARIANT * StatusCode, VARIANT_BOOL * Cancel) {}
    void MyDispatchHost::PrivacyImpactedStateChange(VARIANT_BOOL PrivacyImpacted) {}
    void MyDispatchHost::WindowSetResizable(VARIANT_BOOL Resizable) {}
    void MyDispatchHost::CommandStateChange2(long Command, VARIANT_BOOL Enable) {}
    void MyDispatchHost::WindowClosing(VARIANT_BOOL IsChildWindow, VARIANT_BOOL * Cancel) {}
    void MyDispatchHost::NewWindow2(IDispatch ** ppDisp, VARIANT_BOOL * Cancel)
    {
        if (Cancel)
        {
            *Cancel = VARIANT_TRUE;
        }
    }
    void MyDispatchHost::NewWindow3(IDispatch ** ppDisp, VARIANT_BOOL * Cancel, unsigned long dwFlags, BSTR bstrUrlContext, BSTR bstrUrl)
    {
        if (Cancel)
        {
            *Cancel = VARIANT_TRUE;
        }
    }
    void MyDispatchHost::BeforeNavigate2(IDispatch * pDisp, VARIANT * pvURL, VARIANT * Flags, VARIANT * TargetFrameName, VARIANT * PostData, VARIANT * Headers, VARIANT_BOOL * Cancel) {}
}