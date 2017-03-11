/******************************************************************************
 *  ��Ȩ���У�C��2010-2017													*
 *  ��������Ȩ����                                                            *
 *****************************************************************************
 *  ���� : ����
 *  �汾 : 1.0
 *****************************************************************************/
/*  �޸ļ�¼: 
      ����       �汾    �޸���             �޸�����
    --------------------------------------------------------------------------
******************************************************************************/
#pragma once

#include <vector>

namespace DuiLib
{
    class UILIB_API CCurveChartUI : public CControlUI
    {
    public:
        enum
        {
            FLAG_CAN_ZOOM_IN = 0x01,
            FLAG_CAN_ZOOM_OUT = 0x02,
        };

    public:
        CCurveChartUI();
        ~CCurveChartUI();

        virtual LPCTSTR GetClass() const override;

        virtual LPVOID GetInterface(LPCTSTR pstrName) override;

        virtual void DoEvent(TEventUI& event) override;

        virtual void DoPaint(HDC hDC, const RECT& rcPaint) override;

        virtual void SetPos(RECT rc) override;

        void SetDatas(const std::vector<double>& datas);
        const std::vector<double>& GetDatas() const;

        void AddData(double data);

        void SetZoomValue(double nZoomValue);
        double GetZoomValue() const;

        void SetCurveLineColor(DWORD dwCurveLineColor);
        DWORD GetCurveLineColor() const;

        DWORD GetZoomFlag() const;

        bool CanZoomIn() const;
        bool CanZoomOut() const;
    private:
        void CalMinMaxDrawValue();
        void DrawChart(HDC hDC, const RECT& rcGraph);

        int GetMaxDrawCounts() const;


    private:

        static const double sm_nMaxZoomVal;
        static const double sm_nMinZoomVal;

    private:
        int		m_nMaxDrawItems;
        double	m_nZoomValue;
        double	m_minDrawValue;
        double  m_maxDrawValue;
        DWORD m_dwAxisColor;
        DWORD m_dwCurveColor;

		bool m_isDrawAxis;
        std::vector<double> m_datas;
    };
}