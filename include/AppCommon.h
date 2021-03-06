/******************************************************************************
 *  版权所有（C）2010-2015，xlfy.tech                      *
 *  保留所有权利。                                                            *
 ******************************************************************************
 *  作者 : 熊浪
 *  版本 : 1.0
 *****************************************************************************/
/*  修改记录: 
      日期       版本    修改人             修改内容
    --------------------------------------------------------------------------
******************************************************************************/
#ifndef __AppCommon_h__
#define __AppCommon_h__ 1

inline Poco::Logger& logger()
{
    return Poco::Util::Application::instance().logger();
}

inline Poco::Util::LayeredConfiguration& config()
{
    return Poco::Util::Application::instance().config();
}

#endif // __AppCommon_h__