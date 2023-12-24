#ifndef JRWARE_H
#define JRWARE_H
#include <cstdint>

namespace jr
{
    static constexpr uint32_t NORMAL_MODE = 1000; // 正常模式下一秒处理一次请求
    static constexpr uint32_t FAST_MODE = 10;     // 快速模式下10毫秒处理一次请求
    /* 交互服务 */
    class JRware
    {
    private:
        void help();
        uint32_t m_runMode;
    public:
        JRware();
        void start();
    };
}

#endif