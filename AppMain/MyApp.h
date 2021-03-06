#pragma once

#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Util/XMLConfiguration.h"

namespace util = Poco::Util;

class MyApp : public Poco::Util::Application
    /// This sample demonstrates some of the features of the Poco::Util::Application class,
    /// such as configuration file handling and command line arguments processing.
    ///
    /// Try zip --help (on Unix platforms) or zip /help (elsewhere) for
    /// more information.
{
public:
    MyApp();
protected:
    void initialize(Poco::Util::Application& self);
    void uninitialize();
    void reinitialize(Poco::Util::Application& self);
    
    void defineOptions(Poco::Util::OptionSet& options);

    void handleHelp(const std::string& name, const std::string& value);

    void handleRender(const std::string& name, const std::string& value);

    void displayHelp();

    int main(const std::vector<std::string>& args);

    void onPropertyChanged(const void*, const Poco::Util::AbstractConfiguration::KeyValue& kv);
private:
    
    void _MyInitialize();
    void _MyUninitalize();

    bool m_bHelpRequested;
    bool m_bRenderProcess;
    
    int m_nRenderID;

    /// ptrXmlConfig 
    /// 保存原始指针， 在config变化时，
    /// 重新加载xml文件.
    Poco::AutoPtr<Poco::Util::XMLConfiguration> m_ptrXmlConfig;
    /// xml config path.
    std::string m_pathXmlConfig;
};
