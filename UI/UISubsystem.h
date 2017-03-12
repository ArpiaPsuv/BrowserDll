/******************************************************************************
*  ��Ȩ���У�C��2010-2018��xlfy.tech                      *
*  ��������Ȩ����                                                            *
******************************************************************************
*  ���� : ����
*  �汾 : 1.0
*****************************************************************************/
/*  �޸ļ�¼:
����       �汾    �޸���             �޸�����
--------------------------------------------------------------------------
******************************************************************************/
#ifndef UISubsystem_h__
#define UISubsystem_h__ 1

#include "Poco/Logger.h"
#include "Poco/AutoPtr.h"
#include "Poco/Util/Subsystem.h"
#include "Poco/Util/LayeredConfiguration.h"
#include "Poco/BasicEvent.h"
#include "MyString.h"
#include <memory>

namespace Poco
{
    namespace Util
    {
        class XMLConfiguration;
    }
}

namespace MyWeb {
     namespace UI {

        class CFrameMgr;
        struct TBrowserModuleApis;
        class MYUI_API CUISubsystem :
            public    Poco::Util::Subsystem
        {
        public:
            CUISubsystem();

            virtual ~CUISubsystem();

        public:

            Poco::Logger& logger() const;

            Poco::Util::LayeredConfiguration& config() const;

            int RunUISubsystem();

        public:

            Poco::BasicEvent<void> CreateFrame;
            Poco::BasicEvent<const uint32_t> QuitFrame;

            Poco::BasicEvent<WPARAM> CreateRenderParentWnd;

            int CreateBrowserCtrl(HWND hBindWnd);
            void DestroyBrowserCtrl(int nIndex);

            void SetBrowserCtrlPos(int nIndex, RECT rc);

            void NavigateUrl(int nIndex, const MyString& url);
            void ExecuteJscode(int nIndex, const MyString& jscode);

            bool RegisterBrowserCallback(int nIndex);
            bool UnRegisterBrowserCallback(int nIndex);


            /** Subsystem interface
            */
        protected:
            virtual const char* name() const;
            /// Returns the name of the subsystem.
            /// Must be implemented by subclasses.

            virtual void initialize(Poco::Util::Application& app);
            /// Initializes the subsystem.

            virtual void uninitialize();
            /// Uninitializes the subsystem.

            virtual void reinitialize(Poco::Util::Application& app);
            /// Re-initializes the subsystem.
            ///
            /// The default implementation just calls
            /// uninitialize() followed by initialize().
            /// Actual implementations might want to use a
            /// less radical and possibly more performant 
            /// approach.

            virtual void defineOptions(Poco::Util::OptionSet& options);
            /// Called before the Application's command line processing begins.
            /// If a subsystem wants to support command line arguments,
            /// it must override this method.
            /// The default implementation does not define any options.
            ///
            /// To effectively handle options, a subsystem should either bind
            /// the option to a configuration property or specify a callback
            /// to handle the option.

        private:

            void _InitBrowserModuleApis();

            void handleTest(const std::string& name, const std::string& value);

        private:

            Poco::Logger* m_pLogger;

            Poco::AutoPtr<Poco::Util::LayeredConfiguration> m_spConf;

            std::shared_ptr<CFrameMgr> m_spFrameMgr;
            std::shared_ptr<TBrowserModuleApis> m_spBrowserModuleApis;
            std::string m_configPath;

            HMODULE m_hBrowserModule;
            
        };

        inline Poco::Logger& CUISubsystem::logger() const
        {
            poco_check_ptr(m_pLogger);
            return *m_pLogger;
        }
    } // UI
} // MyWeb
#endif // UISubsystem_h__