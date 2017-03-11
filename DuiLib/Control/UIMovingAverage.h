/******************************************************************************
 *  ��Ȩ���У�C��2012-2016													*
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

#include <map>

namespace DuiLib
{
	class CCandlestickChartUI;
	class CMovingAverageUI : public CHorizontalLayoutUI
	{
	public:
		CMovingAverageUI();
		~CMovingAverageUI();

		void SetOwner(CCandlestickChartUI* pOwner);
		void UpdateData(size_t nIndex);
		virtual void DoPaint(HDC hDC, const RECT& rcPaint);
	private:
		CCandlestickChartUI* m_pOwner;
	};
}

