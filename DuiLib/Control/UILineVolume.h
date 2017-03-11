/******************************************************************************
 *  ��Ȩ���У�C��2010-2015													*
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

namespace DuiLib
{
	class CLineChartUI;
	class CLineVolumeUI : public CLabelUI
	{
	public:
		CLineVolumeUI();
		~CLineVolumeUI();

	public:
		void SetOwner(CLineChartUI* pOwner);
		void SetTickLimis(double minTick,double maxTick);

		double GetMinYTick();
		double GetMaxYTick();
		double GetSepYTick();
		void ResetTick();

		void CalAxisDataLimits();
		POINT GetOriginCoordinate() const;

		double GetXScale() const;
		double GetYScale() const;

		void Invalidate();

		void DrawVolume(HDC hDC, const RECT& rcPaint, RECT& rcItem);
		void CalMinMaxDrawValue();

		static CDuiString GetLblText(double nValue);
	private:

		void DrawYAxis(HDC hDC, const RECT& rcGraph, double dyScale);
		void DrawXAxis(HDC hDC, const RECT& rcGraph, double dyScale);

	private:
		double			m_minDrawValue;
		double          m_maxDrawValue;
		double			m_iMinTick;
		double			m_iMaxTick;
		double			m_uStepTick;
		double			m_dxScale;
		double			m_dyScale;
		DWORD			m_dwLineColor;
		std::vector<double> m_sepYTicks;
		CDuiPoint m_ptOriCoorinate;
		CLineChartUI* m_pOwner;
	};
}

