#ifndef JRBASE_H
#define JRBASE_H
#include "JRtime.h"
#include <iostream>
#include <exception>
#include <stdarg.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <boost/serialization/singleton.hpp>

namespace jr
{
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define Logs jr::Log::get_mutable_instance()
#define GlobalConfigs jr::GlobalConfig::get_mutable_instance()

    static constexpr int SUCCESS = 0;
    static constexpr int FAIL = -1;

    /* 将str左边补0至len长度 */
    static inline void padZero4String(std::string &str, uint32_t len)
    {
        size_t strLen = str.length();
        if (strLen < len) {
            std::string padStr(len - strLen, '0');
            str = padStr + str;
        }
    }

    /* 将输入字符串src按照target字符分割，保存至out */
    static inline void splitString(const std::string &src, char target, std::vector<std::string> &out)
    {
        if (unlikely(src.empty())) {
            return;
        }
        std::stringstream ss(src);
        std::string temp;
        while (getline(ss, temp, target)) {
            out.emplace_back(temp);
        }
        out.erase(out.begin());
    }

    /* 自定义异常类 */
    class JRException : public std::exception
    {
    private:
        std::string m_errorMsg;

    public:
        JRException() = default;
        JRException(const char *format, ...)
        {
            m_errorMsg.clear();
            va_list args;
            va_start(args, format);
            const auto len = vsnprintf(nullptr, 0, format, args);
            va_end(args);
            m_errorMsg.resize(static_cast<size_t>(len) + 1);
            va_start(args, format);
            vsnprintf(&m_errorMsg.front(), len + 1, format, args);
            va_end(args);
            m_errorMsg.resize(static_cast<size_t>(len));
        }
        virtual const char *what(void) const noexcept override { return m_errorMsg.c_str(); }
    };

    /* 日志系统
     * 示例: Logs.log() << "hello world!" << std::endl;
     */
    class Log : public boost::serialization::singleton<Log>
    {
    private:
        std::ofstream m_logStream;
        std::string m_funcName;

    public:
        void setLogPath(const std::string &path);
        std::ofstream &log();
        void close();
        void setFuncName(const std::string &name);
    };

    /* 日志守卫类，生产函数级日志信息 */
    class LogGuard
    {
    private:
        std::string m_funcName;
    public:
        LogGuard() = default;
        LogGuard(const std::string &funcName);
        ~LogGuard();
        void setFuncName(const std::string &name);
    };

    /* 全局配置类 */
    class GlobalConfig : public boost::serialization::singleton<GlobalConfig>
    {
    private:
        std::string m_logPath;
        std::string m_dataSysPath;

    public:
        void init();
        std::string getLogPath();
        std::string getDataSysPath();
    };
}

#endif