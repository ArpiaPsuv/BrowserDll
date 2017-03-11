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

namespace DuiLib
{
	struct UILIB_API TCandlestickChartData
	{
		double m_highVal;
		double m_lowVal;
		double m_beginVal;
		double m_endVal;
		double m_transVolume;
		double m_transAmount;
		double m_changeAmount;
		double m_changeRange;
		CDuiString m_strXText;

		bool IsRose() const
		{
			return (m_endVal > m_beginVal || 
				(m_beginVal == m_endVal && m_changeRange >= 0) ) ? true : false;
		}
	};

}