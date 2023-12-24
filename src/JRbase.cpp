#include "JRbase.h"

/* 日志系统实现 */
std::ofstream &jr::Log::log()
{
    jr::JRTime curTime;
    m_logStream << "[" << curTime.getCurrentTime() << "]";
    if (!m_funcName.empty()) {
        m_logStream << "[" + m_funcName << "]";
    }
    return m_logStream;
}

void jr::Log::setLogPath(const std::string &path)
{
    log().open(path);
    log() << "Log Path:" << path << std::endl;
}

void jr::Log::close()
{
    log().close();
}

void jr::Log::setFuncName(const std::string &name)
{
    m_funcName = name;
}

/* 日志守卫类实现 */
jr::LogGuard::LogGuard(const std::string &funcName)
{
    setFuncName(funcName);
    Logs.log() << "begin" << std::endl; 
}

jr::LogGuard::~LogGuard()
{
    setFuncName(m_funcName);
    Logs.log() << "end" << std::endl;
    Logs.setFuncName("");
}

void jr::LogGuard::setFuncName(const std::string &name)
{
    m_funcName = name;
    Logs.setFuncName(m_funcName);
}

/* 全局配置类实现 */
void jr::GlobalConfig::init()
{
    m_logPath = "D:\\3.project\\3.JRware\\output\\logFile.log";
    m_dataSysPath = "D:\\3.project\\3.JRware\\output\\dataSys.txt";
}

std::string jr::GlobalConfig::getLogPath()
{
    return m_logPath;
}

std::string jr::GlobalConfig::getDataSysPath()
{
    return m_dataSysPath;
}