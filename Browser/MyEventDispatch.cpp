#include "stdafx.h"
#include "MyEventDispatch.h"
#include "MyDispatchHost.h"

#include <ExDispid.h>
#include <Guiddef.h>
#include <MsHtmdid.h>
#include <ExDisp.h>

namespace MyWeb
{
    MyEventDispatch::MyEventDispatch(MyDispatchHost* pDispatchHost, const CComPtr<IDocHostUIHandler>& spDocHostUIHandler) :
        m_pHost(pDispatchHost),
        m_spDocHostUIHandler(spDocHostUIHandler),
        m_dwDLControl(DLCTL_DLIMAGES | DLCTL_VIDEOS | DLCTL_BGSOUNDS | DLCTL_NO_DLACTIVEXCTLS | DLCTL_NO_RUNACTIVEXCTLS),
        m_bSendSourceOnDocComplete(false),
        m_pszStrDoc(NULL)
    {

    }

    MyEventDispatch::~MyEventDispatch()
    {

    }

    void MyEventDispatch::SetDocHostUIHandler(const CComPtr<IDocHostUIHandler>& spDocHostUIHandler)
    {
        m_spDocHostUIHandler = spDocHostUIHandler;
    }

    // IUnknown
    STDMETHODIMP MyEventDispatch::QueryInterface(REFIID iid, void ** ppvObject)
    {
        if (!ppvObject)
            return E_POINTER;

        *ppvObject = NULL;
        if (IsEqualIID(iid, (DIID_DWebBrowserEvents2)))
        {
            *ppvObject = this;
            return S_OK;
        }
        else if (m_spDocHostUIHandler)
        {
            return m_spDocHostUIHandler->QueryInterface(iid, ppvObject);
        }

        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE MyEventDispatch::AddRef()
    {
        return 1;
    }

    ULONG STDMETHODCALLTYPE MyEventDispatch::Release()
    {
        return 1;
    }

    // IDispatch
    STDMETHODIMP MyEventDispatch::GetTypeInfoCount(unsigned int FAR* pctinfo)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP MyEventDispatch::GetTypeInfo(unsigned int iTInfo, LCID  lcid,
        ITypeInfo FAR* FAR*  ppTInfo)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP MyEventDispatch::GetIDsOfNames(REFIID riid,
        OLECHAR FAR* FAR* rgszNames,
        unsigned int cNames,
        LCID lcid,
        DISPID FAR* rgDispId)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP MyEventDispatch::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
        WORD wFlags, DISPPARAMS FAR* pDispParams,
        VARIANT FAR* parResult,
        EXCEPINFO FAR* pExcepInfo,
        unsigned int FAR* puArgErr)
    {
        //VARIANT_FALSE = ((VARIANT_BOOL)0)
        //VARIANT_TRUE = ((VARIANT_BOOL)-1)

        //The pDispParams are in reverse order
        //BeforeNavigate2 Cancel=[0], Headers[1], ...pDisp=[6]

        //return DISP_E_MEMBERNOTFOUND;

        USES_CONVERSION;
        switch (dispIdMember)
        {
            //		case DISPID_BEFORENAVIGATE2:
            //			{
            //				if ((pDispParams->cArgs >= 7) && (pDispParams->rgvarg[0].vt == (VT_BOOL | VT_BYREF)))
            //				{
            //					//Cancel = false
            //					//((*pDispParams).rgvarg)[0].boolVal = FALSE;
            //					//Dereferencing
            //					//*V_BOOLREF(&pDispParams->rgvarg[0]) = VARIANT_TRUE;
            //					*(pDispParams->rgvarg[0].pboolVal)  = VARIANT_FALSE;
            //					m_pHost->BeforeNavigate2(pDispParams->rgvarg[6].pdispVal,pDispParams->rgvarg[5].pvarVal,
            //						pDispParams->rgvarg[4].pvarVal,pDispParams->rgvarg[3].pvarVal,
            //						pDispParams->rgvarg[2].pvarVal,pDispParams->rgvarg[1].pvarVal,
            //						pDispParams->rgvarg[0].pboolVal);
            //					pDispParams->rgvarg[4].bstrVal = NULL; //Must be set to NULL
            //					//if (m_pHost->m_EventsPtr)
            //					//{
            //					//	m_pHost->m_EventsPtr->Fire_BeforeNavigate2(m_pHost->m_thisID,
            //					//		pDispParams->rgvarg[5].pvarVal,
            //					//		pDispParams->rgvarg[6].pdispVal,
            //					//		pDispParams->rgvarg[4].pvarVal, //VT_BSTR
            //					//		pDispParams->rgvarg[3].pvarVal, //VT_BSTR
            //					//		pDispParams->rgvarg[2].pvarVal, //VT_BYREF|VT_VARIANT
            //					//		pDispParams->rgvarg[1].pvarVal, //VT_BSTR
            //					//		pDispParams->rgvarg[0].pboolVal);
            //					//}
            //							//&pDispParams->rgvarg[0].boolVal);
            ////This part must be done from app, not here
            ////if (*(pDispParams->rgvarg[0].pboolVal) == VARIANT_TRUE)
            ////{
            ////	m_pHost->pWebBrowser->Stop();
            ////	m_pHost->pWebBrowser->Navigate(m_pHost->m_EventsPtr->m_Startup ,NULL,NULL,NULL,NULL);
            ////}
            //				}
            //			}
            //			break;
                    //In Internet Explorer 6 or later, the IWebBrowser2::Navigate2 event fires only after
                    //the first navigation made in code. It does not fire when a user
                    //clicks a link on a Web page.
                    //
                    // [0]: URL - VT_BSTR
                    //
        case DISPID_NAVIGATECOMPLETE2: //Need to add isTopLevel
            if (pDispParams->cArgs >= 2 && pDispParams->rgvarg[1].vt == VT_DISPATCH)
            {
                m_pHost->NavigateComplete2(pDispParams->rgvarg[1].pdispVal, pDispParams->rgvarg[0].pvarVal);
            }
            break;
        case DISPID_PROGRESSCHANGE:
            if (pDispParams->cArgs >= 2 && pDispParams->rgvarg[0].vt == VT_I4)
            {
                m_pHost->ProgressChange2(pDispParams->rgvarg[1].lVal, pDispParams->rgvarg[0].lVal);
            }
            break;
            //
            // [0]: New status bar text - BSTR
            //
        case DISPID_STATUSTEXTCHANGE:
            if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
            {
                m_pHost->StatusTextChange2(pDispParams->rgvarg[0].bstrVal);
            }
            break;
            /*
            > What I'd like to know is if there is a foolproof way for me to get the
            > source code of the main webpage and every frame immediately after they
            > are downloaded and before any scripts have run.


            Query the document object for IPersistStreamInit, call Save (you can
            create an in-memory IStream implementation with CreateStreamOnHGlobal).
            This gets the HTML exactly as the server sent it. Do it from, say,
            DocumentComplete event
            */
            //
            // [0]: URL - VT_BSTR
            //
        case DISPID_DOCUMENTCOMPLETE:
            if (pDispParams->cArgs >= 2 && pDispParams->rgvarg[1].vt == VT_DISPATCH)
            {
                m_pHost->DocumentComplete(pDispParams->rgvarg[1].pdispVal, pDispParams->rgvarg[0].pvarVal);
            }
            break;
            //	if (pDispParams->cArgs >= 2 && pDispParams->rgvarg[1].vt == VT_DISPATCH)
            //	{
            //		LPDISPATCH lpWBDisp;
            //		HRESULT    hr;
            //		//Get the IDispatch of our browser for comparison
            //		hr = m_pHost->pUnknown->QueryInterface(IID_IDispatch, 
            //				reinterpret_cast<void **>(&lpWBDisp));
            //		if (SUCCEEDED(hr) && lpWBDisp)
            //		{
            //			//CComBSTR url = ((*pDispParams).rgvarg)[0].pvarVal->bstrVal;
            //			//Fire_DocumentCompleteWBEx m_pszStrDoc
            //			if (lpWBDisp == pDispParams->rgvarg[1].pdispVal)
            //			{
            //				//Top-level Window object
            //				if (m_pHost->m_EventsPtr)
            //				{
            //					//Do we want the source before javascripts exec
            //					if (m_bSendSourceOnDocComplete == true)
            //					{
            //						hr = WBHTMLDocAsStream(lpWBDisp);
            //						if ( (SUCCEEDED(hr)) && (m_pszStrDoc) )
            //						{
            //							m_pHost->m_EventsPtr->Fire_DocumentCompleteWBEx(
            //										m_pHost->m_thisID,
            //										pDispParams->rgvarg[0].pvarVal,
            //										pDispParams->rgvarg[1].pdispVal,
            //										VARIANT_TRUE,
            //										T2BSTR(m_pszStrDoc));
            //							free (m_pszStrDoc);
            //							m_pszStrDoc = NULL;
            //						}
            //						else
            //						{
            //							m_pHost->m_EventsPtr->Fire_DocumentCompleteWBEx(
            //										m_pHost->m_thisID,
            //										pDispParams->rgvarg[0].pvarVal,
            //										pDispParams->rgvarg[1].pdispVal,
            //										VARIANT_TRUE,
            //										m_ZeroLenStr);
            //						}
            //					}
            //					else
            //					{
            //						m_pHost->m_EventsPtr->Fire_DocumentComplete(
            //									m_pHost->m_thisID,
            //									pDispParams->rgvarg[0].pvarVal,
            //									pDispParams->rgvarg[1].pdispVal,
            //									VARIANT_TRUE);
            //					}
            //				}
            //			}
            //			else
            //			{
            //				//Frame, IFrames, ...
            //				if (m_pHost->m_EventsPtr)
            //				{
            //					if (m_bSendSourceOnDocComplete == true)
            //					{
            //						hr = WBHTMLDocAsStream(pDispParams->rgvarg[1].pdispVal);
            //						if ( (SUCCEEDED(hr)) && (m_pszStrDoc) )
            //						{
            //							m_pHost->m_EventsPtr->Fire_DocumentCompleteWBEx(
            //										m_pHost->m_thisID,
            //										pDispParams->rgvarg[0].pvarVal,
            //										pDispParams->rgvarg[1].pdispVal,
            //										VARIANT_FALSE,
            //										T2BSTR(m_pszStrDoc));
            //							free (m_pszStrDoc);
            //							m_pszStrDoc = NULL;
            //						}
            //						else
            //						{
            //							m_pHost->m_EventsPtr->Fire_DocumentCompleteWBEx(
            //										m_pHost->m_thisID,
            //										pDispParams->rgvarg[0].pvarVal,
            //										pDispParams->rgvarg[1].pdispVal,
            //										VARIANT_FALSE,
            //										m_ZeroLenStr);
            //						}
            //					}
            //					else
            //					{
            //						m_pHost->m_EventsPtr->Fire_DocumentComplete(
            //									m_pHost->m_thisID,
            //									pDispParams->rgvarg[0].pvarVal,
            //									pDispParams->rgvarg[1].pdispVal,
            //									VARIANT_FALSE);
            //					}
            //				}
            //			}
            //			lpWBDisp->Release();
            //		}
            //		
            //	}
            //}
            //break;
        //
        // [0]: Document title - BSTR
        //
        case DISPID_TITLECHANGE:
            if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
            {
                m_pHost->TitleChange2(pDispParams->rgvarg[0].bstrVal);
            }
            break;
        case DISPID_DOWNLOADBEGIN:
            // Enable the stop menu item and toolbar button
            m_pHost->DownloadBegin();
            break;
        case DISPID_DOWNLOADCOMPLETE:
            m_pHost->DownloadComplete();
            break;
        case DISPID_WINDOWSETHEIGHT:
            if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_I4)
            {
                m_pHost->WindowSetHeight(pDispParams->rgvarg[0].lVal);
            }
            break;
        case DISPID_WINDOWSETLEFT:
            if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_I4)
            {
                m_pHost->WindowSetLeft(pDispParams->rgvarg[0].lVal);
            }
            break;
        case DISPID_WINDOWSETTOP:
            if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_I4)
            {
                m_pHost->WindowSetTop(pDispParams->rgvarg[0].lVal);
            }
            break;
        case DISPID_WINDOWSETWIDTH:
            if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_I4)
            {
                m_pHost->WindowSetWidth(pDispParams->rgvarg[0].lVal);
            }
            break;
        case DISPID_WINDOWCLOSING:
        {
            if ((pDispParams->cArgs >= 2) && (pDispParams->rgvarg[0].vt == (VT_BOOL | VT_BYREF)))
            {
                //Initially, window is not allowed to close, Cancel = true
                //((*pDispParams).rgvarg)[0].boolVal = FALSE;
                *(pDispParams->rgvarg[0].pboolVal) = VARIANT_TRUE;
                //IsChildWindow = TRUE => Window was created from script.
                m_pHost->WindowClosing(pDispParams->rgvarg[1].boolVal, pDispParams->rgvarg[0].pboolVal);
            }
        }
        break;
        case DISPID_COMMANDSTATECHANGE:
            if (pDispParams->cArgs >= 2 && pDispParams->rgvarg[0].vt == VT_BOOL)
            {
                m_pHost->CommandStateChange2(pDispParams->rgvarg[1].lVal, pDispParams->rgvarg[0].boolVal);
            }
            break;
            //if *(pDispParams->rgvarg[0].pboolVal)  = VARIANT_TRUE;
            //then we can not catch download event from WBDownloadMgr
        //case DISPID_FILEDOWNLOAD:
        //	//*(pDispParams->rgvarg[0].pboolVal)  = VARIANT_FALSE;
        //	if ((pDispParams->cArgs >= 2) && (pDispParams->rgvarg[0].vt == (VT_BOOL | VT_BYREF)))
        //	{
        //		//Initially all filedownload is done by us
        //		//if m_useieforfiledl == false, then we are using FileDownloadEx event
        //		//So we don't bother generating an event here
        //		/*if (m_pHost->m_useieforfiledl == true)
        //		{*/
        //			*(pDispParams->rgvarg[0].pboolVal)  = VARIANT_TRUE;
        //			m_pHost->FileDownload(pDispParams->rgvarg[1].pboolVal, pDispParams->rgvarg[0].pboolVal);
        //	}
        //	break;
        case DISPID_CLIENTTOHOSTWINDOW:
            if ((pDispParams->cArgs >= 2) && (pDispParams->rgvarg[0].vt == (VT_BYREF | VT_I4)))
            {
                m_pHost->ClientToHostWindow(pDispParams->rgvarg[1].plVal, pDispParams->rgvarg[0].plVal);
                //&pDispParams->rgvarg[1].lVal,
            }
            break;
            //
            // [3]: VT_BSTR
            // [2]: VT_BSTR
            // [1]: VT_I4 (LONG)
            //
        case DISPID_NAVIGATEERROR:
            if ((pDispParams->cArgs >= 5) &&
                (pDispParams->rgvarg[0].vt == (VT_BOOL | VT_BYREF)))
            {
                //Cancel = false //default
                *(pDispParams->rgvarg[0].pboolVal) = VARIANT_FALSE;
                m_pHost->NavigateError(
                    pDispParams->rgvarg[4].pdispVal,
                    pDispParams->rgvarg[3].pvarVal,
                    pDispParams->rgvarg[2].pvarVal,
                    pDispParams->rgvarg[1].pvarVal,
                    pDispParams->rgvarg[0].pboolVal);
            }
            ////Extract the status code from the DISPPARAMS structure
            //VARIANT * vt_statuscode = pDispParams->rgvarg[1].pvarVal;
            //DWORD  dwStatusCode =  vt_statuscode->lVal;
            ////Extract the event's IDispatch pointer
            //IDispatch *pdispFiredEvent = pDispParams->rgvarg[4].pdispVal;
            break;
        case DISPID_SETSECURELOCKICON:
        {
            if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_I4)
            {
                m_pHost->SetSecureLockIcon(pDispParams->rgvarg[0].lVal);
            }
        }
        break;
        case DISPID_NEWWINDOW2:
        {
            //Initially cancel
            //Cancel = true
            if ((pDispParams->cArgs >= 2) &&
                (pDispParams->rgvarg[0].vt == (VT_BOOL | VT_BYREF)))
            {
                *(pDispParams->rgvarg[0].pboolVal) = VARIANT_TRUE;
                m_pHost->NewWindow2(pDispParams->rgvarg[1].ppdispVal, pDispParams->rgvarg[0].pboolVal);
            }
        }
        break;
        //DWebBrowserEvents2::NewWindow3 is raised before DWebBrowserEvents2::NewWindow2.
        //XP sp2 only
        //
        // [4]: VT_DISPATCH | VT_BYREF
        // [3]: VT_BOOL | VT_BYREF
        // [2]: DWORD (long, NEW_WINDOW3_FLAGS enum)
        // [1]: VT_BSTR
        // [0]: VT_BSTR
        //
        case DISPID_NEWWINDOW3:
        {
            if ((pDispParams->cArgs >= 5) &&
                (pDispParams->rgvarg[3].vt == (VT_BOOL | VT_BYREF)))
            {
                //Initially cancel
                *(pDispParams->rgvarg[3].pboolVal) = VARIANT_TRUE;

                long lflags = 0;
                if (pDispParams->rgvarg[2].vt == VT_I4)
                    long lflags = pDispParams->rgvarg[2].lVal;
                else if (pDispParams->rgvarg[2].vt == VT_UI4)
                    long lflags = static_cast<long>(pDispParams->rgvarg[2].ulVal);

                //Send the event
                m_pHost->NewWindow3(pDispParams->rgvarg[4].ppdispVal,
                    pDispParams->rgvarg[3].pboolVal,
                    lflags,
                    pDispParams->rgvarg[1].bstrVal,
                    pDispParams->rgvarg[0].bstrVal);
            }
        }
        break;
        //
        //[0 in] VT_BOOL that indicates whether the current top-level URL has violated
        //the browser's privacy settings.
        //True or False
        case DISPID_PRIVACYIMPACTEDSTATECHANGE:
            if (pDispParams->cArgs >= 1 && pDispParams->rgvarg[0].vt == VT_BOOL)
            {
                if (pDispParams->rgvarg[0].boolVal) //True or False
                {
                    m_pHost->PrivacyImpactedStateChange(VARIANT_TRUE);
                }
                else
                {
                    m_pHost->PrivacyImpactedStateChange(VARIANT_FALSE);
                }
                //or *(pDispParams->rgvarg[0].pboolVal));
            }
            break;
        case DISPID_WINDOWSETRESIZABLE:
            if (pDispParams->cArgs >= 1 && pDispParams->rgvarg[0].vt == VT_BOOL)
            {
                m_pHost->WindowSetResizable(pDispParams->rgvarg[0].boolVal);
            }
            break;
            //The DISPID_AMBIENT_* and DLCTL_* values are defined in Mshtmdid.h
            //
            // DISPID for Download Control
            //
            //CComVariant vtResult(static_cast<long>(m_dwDLControl));
            //*parResult = vtResult;
            //This only hits for every new webbrowser created
            //Or by setting globalDOCHOSTUIFLAGS property
            //which in turn calls OleControl.OnAmbientChanged forcing
            //MSHTML to call us here to get the flags.
            //Needs a refesh for the new flags to take effect
        case DISPID_AMBIENT_DLCONTROL:
        {
            if (parResult->vt == VT_EMPTY)
            {
                parResult->vt = VT_I4;
                parResult->lVal = static_cast<long>(m_dwDLControl);
            }
        }
        break;
        //		// Change user agent for this web browser host during hyperlinks
                    //MSHTML will also ask for a new user agent via DISPID_AMBIENT_USERAGENT
                    //when navigating to clicked hyperlinks. This ambient property can be
                    //overridden, but it is not used when programmatically calling
                    //the Navigate method; it will also not cause the userAgent
                    //property of the DOM's navigator object or clientInformation
                    //behavior to be altered - this property will always reflect
                    //Internet Explorer's own UserAgent std::string. 
        //		case DISPID_AMBIENT_USERAGENT:
        //		   {
        //			  CString strUserAgent("MyWebBrowserHost");
        //			  if (parResult->vt == VT_EMPTY)
        //			  {
        //				parResult->vt = VT_BSTR;
        //				parResult->bstrVal = ::SysAllocString(T2OLE(strUserAgent));
        //			  {
        //		   }
        //		   break;
        default:
            return DISP_E_MEMBERNOTFOUND;
        }
        return S_OK;
    }
}