#pragma once

#include <OAIdl.h>
#include <MsHtmHst.h>

namespace MyWeb
{
    class MyDispatchHost;
    class MyEventDispatch : public IDispatch
    {
    public:
        MyEventDispatch(MyDispatchHost* pHost, const CComPtr<IDocHostUIHandler>& spDocHostUIHandler);
        ~MyEventDispatch();

        void SetDocHostUIHandler(const CComPtr<IDocHostUIHandler>& spDocHostUIHandler);
        // IUnknown
        STDMETHODIMP QueryInterface(REFIID iid, void ** ppvObject);
        ULONG STDMETHODCALLTYPE AddRef();
        ULONG STDMETHODCALLTYPE Release();

        // IDispatch
        STDMETHODIMP GetTypeInfoCount(unsigned int FAR* pctinfo);
        STDMETHODIMP GetTypeInfo(unsigned int iTInfo, LCID  lcid,
            ITypeInfo FAR* FAR*  ppTInfo);
        STDMETHODIMP GetIDsOfNames(REFIID riid, OLECHAR FAR* FAR* rgszNames,
            unsigned int cNames, LCID lcid, DISPID FAR* rgDispId);
        STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
            DISPPARAMS FAR* pDispParams, VARIANT FAR* parResult,
            EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr);

    private:
        MyDispatchHost *m_pHost;
        CComPtr<IDocHostUIHandler> m_spDocHostUIHandler;
        DWORD m_dwDLControl;
        bool m_bSendSourceOnDocComplete;
        LPTSTR m_pszStrDoc;
        CComBSTR m_ZeroLenStr;
    };
}