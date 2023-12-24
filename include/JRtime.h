#ifndef JRTIME_H
#define JRTIME_H
#include <string>

namespace jr
{
    static constexpr uint32_t MIN_YEAR = 2000;
    static constexpr uint32_t MAX_YEAR = 2199;
    static constexpr uint32_t MIN_MONTH = 1;
    static constexpr uint32_t MAX_MONTH = 12;
    static constexpr uint32_t MAX_MONTH_LEN = 2;
    static constexpr uint32_t MIN_DAY = 1;
    static constexpr uint32_t MAX_DAY = 31;
    static constexpr uint32_t MAX_DAY_LEN = 2;
    static constexpr uint32_t MAX_HOUR = 24;
    static constexpr uint32_t MAX_MINUTE = 60;
    static constexpr uint32_t MAX_SECOND = 60;
    static constexpr uint32_t MAX_MS = 1000;
    static constexpr uint32_t MAX_HOUR_LEN = 2;
    static constexpr uint32_t MAX_MINUTE_LEN = 2;
    static constexpr uint32_t MAX_SECOND_LEN = 2;
    static constexpr uint32_t MAX_MS_LEN = 3;

    /* 年份结构 */
    struct Year
    {
    private:
        uint32_t m_year;
        void checkYear(uint32_t year);

    public:
        Year();
        Year(uint32_t year);
        void reset();
        std::string get();
    };

    /* 月份结构 */
    struct Month
    {
    private:
        uint32_t m_month;
        void checkMonth(uint32_t month);

    public:
        Month();
        Month(uint32_t month);
        void reset();
        std::string get();
    };

    /* 天数结构 */
    struct Day
    {
    private:
        uint32_t m_day;
        void checkDay(uint32_t day);

    public:
        Day();
        Day(uint32_t day);
        void reset();
        std::string get();
    };

    /* 时分秒结构，微秒级 */
    struct HmsTime
    {
    private:
        uint32_t m_hour;
        uint32_t m_minute;
        uint32_t m_second;
        uint32_t m_ms;
        void checkHmsTime(uint32_t hour, uint32_t minute, uint32_t second, uint32_t ms);

    public:
        HmsTime();
        HmsTime(uint32_t hour, uint32_t minute, uint32_t second, uint32_t ms);
        void reset();
        std::string get();
    };

    /* 自定义时间类
     * 示例: 
     * jr::JRTime test;
     * jr::JRTime time(jr::Year(2023), jr::Month(12), jr::Day(9), jr::HmsTime(9, 49, 51, 123));
     */
    class JRTime
    {
    public:
        JRTime() = default;
        JRTime(Year year, Month month, Day day, HmsTime hmsTime) : jr_year(year), jr_month(month), jr_day(day), jr_hmsTime(hmsTime) {}
        
        /* 显示时间 */
        void showTime();
        /* 设置当前时间 */
        void setCurrentTime();
        /* 获取当前时间 */
        std::string getCurrentTime();

    private:
        Year jr_year;
        Month jr_month;
        Day jr_day;
        HmsTime jr_hmsTime;
    };
}

#endif