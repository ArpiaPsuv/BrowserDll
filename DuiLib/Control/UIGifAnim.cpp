#include "StdAfx.h"
#include "UIGifAnim.h"

namespace DuiLib
{
    //write by wangji
    struct CGifAnimUI::Imp
    {
        bool                m_bRealStop            ;//�ⲿֹͣ��
        bool                m_bLoadImg            ;//�Ƿ���ع�ͼƬ
        bool                m_bTimer            ;//�Ƿ�������ʱ��
        bool                m_bAutoStart        ;//�Ƿ��Զ���ʼ
        int                    m_nDelay            ;//ѭ��������
        UINT                m_nFrameCount        ;//gifͼƬ��֡��
        UINT                m_nFramePosition    ;//��ǰ�ŵ��ڼ�֡
        CGifHandler*        m_pGifImage            ;//gif����
        PropertyItem*        m_pPropertyItem        ;//֡��֮֡����ʱ��
        CPaintManagerUI*&    m_pManager            ;
        CGifAnimUI*            m_pOwer                ;//ӵ����
        Imp(CPaintManagerUI* & pManager):m_pManager(pManager),
        m_bLoadImg(false),m_bTimer(false),
        m_nDelay(100),m_pGifImage(NULL),m_nFrameCount(0U),
        m_pPropertyItem(NULL),m_nFramePosition(0U),
        m_pOwer(NULL),m_bRealStop(false),m_bAutoStart(true)
        {
        }
        void SetOwer(CGifAnimUI *pOwer)
        {
            m_pOwer = pOwer;
        }
        ~Imp()
        {
            if ( m_pGifImage != NULL )
            {
                delete m_pGifImage;
                m_pGifImage = NULL;
            }
            if ( m_pPropertyItem != NULL )
            {
                free( m_pPropertyItem );
                m_pPropertyItem = NULL;
            }
        }
        inline void CheckLoadImage()
        {
            if(!m_bLoadImg)
            {
                LoadGifImage();
            }    
        }
        inline bool IsLoadImage()
        {
            return m_bLoadImg;
        }
        virtual void LoadGifImage()
        {
            CDuiString sImag = m_pOwer->GetBkImage();
            m_bLoadImg = true;
            m_pGifImage    =    CRenderEngine::LoadGif(STRINGorID(sImag),0, 0);
            if (NULL == m_pGifImage) 
            {
                return;
            }
            const UINT nCount = m_pGifImage->GetFrameCount();
            ASSERT(nCount > 0);
            if(0 == nCount)
            {
                return;
            }
            m_nFrameCount = m_pGifImage->GetFrameCount();
            m_nFramePosition = 0;

            m_nDelay = 100;
            if (m_nDelay <= 0 ) 
                m_nDelay = 100;
            if(!m_bAutoStart)
                m_bRealStop = true;
            if(m_bAutoStart && m_pOwer->IsVisible())
                StartAnim();
        }
        void SetAutoStart(bool bAuto)
        {
            m_bAutoStart = bAuto;
        }
        void StartAnim()
        {
            if(!m_bTimer)
            {
                if(!IsLoadImage())
                    LoadGifImage();
                if(m_pGifImage)
                    m_pManager->SetTimer( m_pOwer, EVENT_TIEM_ID, m_nDelay );
                m_bTimer = true;
            }
        }
        void StopAnim(bool bGoFirstFrame)//bGoFirstFrame �Ƿ��ܵ���һ֡
        {
            if(m_bTimer)
            {
                if(bGoFirstFrame)
                {
                    m_nFramePosition = 0U;
                    m_pOwer->Invalidate();
                }
                m_pManager->KillTimer( m_pOwer, EVENT_TIEM_ID );
                m_bTimer = false;
            }
        }
        void EventOnTimer(const WPARAM idEvent )
        {
            if ( idEvent != EVENT_TIEM_ID )
                return;
            ++m_nFramePosition;
            if(m_nFramePosition >= m_nFrameCount)
                m_nFramePosition = 0;
            if(!m_pOwer->IsVisible())return;
            m_pOwer->Invalidate();
        }
        void DrawFrame( HDC hDC,const RECT& rcPaint,const RECT &rcItem)
        {
            if ( NULL == hDC || NULL == m_pGifImage ) 
            {
                return;
            }
            TImageInfo* pImageInfo = NULL;
            pImageInfo = m_pGifImage->GetNextFrameInfo();
            if (pImageInfo != NULL)
            {
                RECT rcBmpPart = {0};
                RECT rcCorners = {0};
                rcBmpPart.right = pImageInfo->nX;
                rcBmpPart.bottom = pImageInfo->nY;
                RECT rcPaintTmp = rcItem;
                RECT rcItemTmp = rcItem;
                rcItemTmp.right = rcItemTmp.left + pImageInfo->nX;
                rcItemTmp.bottom = rcItemTmp.top + pImageInfo->nY;
                rcPaintTmp.right = rcPaintTmp.left + pImageInfo->nX;
                rcPaintTmp.bottom = rcPaintTmp.top + pImageInfo->nY;
                CRenderEngine::DrawImage(hDC,pImageInfo->hBitmap,rcItemTmp, rcPaintTmp,rcBmpPart,rcCorners,pImageInfo->alphaChannel,255);
            }
        }
        void EventSetVisible(bool bVisible)
        {
            if(bVisible)
            {
                if(!m_bRealStop)
                    StartAnim();
            }
            else
                StopAnim(true);
        }
    };
    CGifAnimUI::CGifAnimUI(void):m_pImp(new CGifAnimUI::Imp(m_pManager))
    {
        this;
        m_pImp->SetOwer(this);
    }
    CGifAnimUI::~CGifAnimUI(void)
    {
        m_pImp->StopAnim(false);
        delete m_pImp;
        m_pImp = nullptr;
    }

    //************************************
    // Method:    GetClass
    // FullName:  CGifAnimUI::GetClass
    // Access:    public 
    // Returns:   LPCTSTR
    // Qualifier: const
    // Note:      
    //************************************
    LPCTSTR CGifAnimUI::GetClass() const
    {
        return _T("GifAnimUI");
    }

    //************************************
    // Method:    GetInterface
    // FullName:  CGifAnimUI::GetInterface
    // Access:    public 
    // Returns:   LPVOID
    // Qualifier:
    // Parameter: LPCTSTR pstrName
    // Note:      
    //************************************
    LPVOID CGifAnimUI::GetInterface( LPCTSTR pstrName )
    {
            if( _tcscmp(pstrName, _T("GifAnim")) == 0 ) 
                return static_cast<CGifAnimUI*>(this);
            return CLabelUI::GetInterface(pstrName);
    }
    void CGifAnimUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if( _tcscmp(pstrName, _T("auto")) == 0 ) 
            m_pImp->SetAutoStart(_tcscmp(pstrValue, _T("true")) == 0);
        else
            __super::SetAttribute(pstrName, pstrValue);
    }
    void CGifAnimUI::Init()
    {
        __super::Init();
        m_pImp->CheckLoadImage();
    }
    void CGifAnimUI::SetVisible(bool bVisible /*= true*/)
    {
        __super::SetVisible(bVisible);
        m_pImp->EventSetVisible(bVisible);
    }
    void CGifAnimUI::SetInternVisible(bool bVisible/* = true*/)
    {
        __super::SetInternVisible(bVisible);
        m_pImp->EventSetVisible(bVisible);
    }
    //************************************
    // Method:    DoPaint
    // FullName:  CGifAnimUI::DoPaint
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: HDC hDC
    // Parameter: const RECT & rcPaint
    // Note:      
    //************************************
    void CGifAnimUI::DoPaint( HDC hDC, const RECT& rcPaint )
    {
        if( !::IntersectRect( &m_rcPaint, &rcPaint, &m_rcItem ) ) return;
        m_pImp->DrawFrame( hDC,rcPaint,m_rcItem);
    }

    //************************************
    // Method:    DoEvent
    // FullName:  CGifAnimUI::DoEvent
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: TEventUI & event
    // Note:      
    //************************************
    void CGifAnimUI::DoEvent( TEventUI& event )
    {
        this;
        WPARAM nID = event.wParam;
        if( event.Type == UIEVENT_TIMER )
            m_pImp->EventOnTimer(nID);
    }
    void CGifAnimUI::StartAnim()
    {
        m_pImp->m_bRealStop = false;
        m_pImp->StartAnim();
    }
    void CGifAnimUI::StopAnim()
    {
        m_pImp->m_bRealStop = true;
        m_pImp->StopAnim(true);
    }
}
