/******************************************************************************
 *  ��Ȩ���У�C��2012-2016                                                    *
 *  ��������Ȩ����                                                            *
 *****************************************************************************
 *  ���� : ����
 *  �汾 : 1.0
 *****************************************************************************/
/*  �޸ļ�¼: 
      ����       �汾    �޸���             �޸�����
    --------------------------------------------------------------------------
******************************************************************************/
#ifndef DateTimeUtil_h_
#define DateTimeUtil_h_

#include <vector>
#include <set>

namespace MyWeb
{
    class CDateTimeUtil
    {
    public:
        static std::wstring Now();
        static std::wstring FormatLocalDateTime(const Poco::Timestamp& utcTimestamp, const std::string& strFormat);
    };
}

#endif // DateTimeUtil_h_