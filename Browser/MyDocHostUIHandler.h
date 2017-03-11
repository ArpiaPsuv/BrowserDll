#pragma once

#pragma warning(push, 3)
#include <unknwn.h>
#include <exdisp.h>
#include <mshtmhst.h>
#pragma warning(pop)

class JSExternal;
/** IE�ں�IDocHostUIHandler2�ӿ�ת��
*/
class MyDocHostUIHandler:
    public IOleCommandTarget,
    public IDocHostUIHandler2
{
public:

    MyDocHostUIHandler() ;

    ~MyDocHostUIHandler() ;

public:

    /** ��ʼ��
    */
    void Init(CComPtr<IWebBrowser2> spWebBrowser2);

protected:

    /** IUnknown�ӿ��������ü���
    */    
    virtual STDMETHODIMP_(ULONG) AddRef(void) ;

    /** IUnknown�ӿڼ������ü���
    */
    virtual STDMETHODIMP_(ULONG) Release(void) ;

    /** IUnknown��ѯ�ӿ�
    */
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppv) ;

protected:

    /** IOleCommandTarget�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE QueryStatus( /* [unique][in] */ const GUID *pguidCmdGroup,
        /* [in] */ ULONG cCmds,
        /* [out][in][size_is] */ OLECMD prgCmds[],
        /* [unique][out][in] */ OLECMDTEXT *pCmdText) ;

    /** IOleCommandTarget�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE Exec( /* [unique][in] */ const GUID *pguidCmdGroup,
        /* [in] */ DWORD nCmdID,
        /* [in] */ DWORD nCmdexecopt,
        /* [unique][in] */ VARIANT *pvaIn,
        /* [unique][out][in] */ VARIANT *pvaOut) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE ShowContextMenu( /* [in] */ DWORD dwID,
        /* [in] */ POINT *ppt,
        /* [in] */ IUnknown *pcmdtReserved,
        /* [in] */ IDispatch *pdispReserved) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE GetHostInfo( /* [out][in] */ DOCHOSTUIINFO *pInfo) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE ShowUI( /* [in] */ DWORD dwID,
        /* [in] */ IOleInPlaceActiveObject *pActiveObject,
        /* [in] */ IOleCommandTarget *pCommandTarget,
        /* [in] */ IOleInPlaceFrame *pFrame,
        /* [in] */ IOleInPlaceUIWindow *pDoc) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE HideUI( void) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE UpdateUI( void) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE EnableModeless( /* [in] */ BOOL fEnable) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate( /* [in] */ BOOL fActivate) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate( /* [in] */ BOOL fActivate) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE ResizeBorder( /* [in] */ LPCRECT prcBorder,
        /* [in] */ IOleInPlaceUIWindow *pUIWindow,
        /* [in] */ BOOL fRameWindow) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual  HRESULT STDMETHODCALLTYPE TranslateAccelerator( /* [in] */ LPMSG lpMsg,
        /* [in] */ const GUID *pguidCmdGroup,
        /* [in] */ DWORD nCmdID) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual  HRESULT STDMETHODCALLTYPE GetOptionKeyPath( /* [out] */ LPOLESTR *pchKey,
        /* [in] */ DWORD dw) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual  HRESULT STDMETHODCALLTYPE GetDropTarget( /* [in] */ IDropTarget *pDropTarget,
        /* [out] */ IDropTarget **ppDropTarget) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE GetExternal( /* [out] */ IDispatch **ppDispatch) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE TranslateUrl( /* [in] */ DWORD dwTranslate,
        /* [in] */ OLECHAR *pchURLIn,
        /* [out] */ OLECHAR **ppchURLOut) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE FilterDataObject( /* [in] */ IDataObject *pDO,
        /* [out] */ IDataObject **ppDORet) ;

    /** IDocHostUIHandler2�ӿ�
    */
    virtual HRESULT STDMETHODCALLTYPE GetOverrideKeyPath( /* [annotation][out] */ LPOLESTR *pchKey,
        /* [in] */ DWORD dw) ;

private:

    /** Ĭ�ϵ�IDocHostUIHandler
    */
    CComPtr<IDocHostUIHandler2> m_spDocHostUIHandler ;

    /** Ĭ�ϵ�IOleCommandTarget
    */
    CComPtr<IOleCommandTarget> m_spOleCommandTarget ;

    /** �Զ���external�ӿ�
    */ 
    CComPtr<JSExternal> m_spJsExternal ;

    /** ���ü���
    */
    volatile LONG m_lRefCount ;
} ;
