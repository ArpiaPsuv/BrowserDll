#pragma once

#include <WinDef.h>

namespace MyWeb
{
    class MyOleInitialize
    {

    public:

        /** Ĭ�Ϲ��캯������ʼ��OLE
        */
        MyOleInitialize();

        /** Ĭ���������������ʼ��OLE
        */
        ~MyOleInitialize();

    private:

        /** ��ʼ�����
        */
        HRESULT m_hr;
    };
}