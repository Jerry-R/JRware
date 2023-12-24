#include "JRTime.h"
#include "JRbase.h"
#include <iostream>
#include <chrono>
#include <time.h>

/* 年份结构实现 */
void jr::Year::reset()
{
    m_year = MIN_YEAR;
}

void jr::Year::checkYear(uint32_t year)
{
    if (unlikely(year < MIN_YEAR || year > MAX_YEAR)) {
        throw JRException("[%s] year:%u is invalid!", __func__, year);
    }
}

jr::Year::Year()
{
    reset();
}

jr::Year::Year(uint32_t year)
{
    checkYear(year);
    m_year = year;
}

std::string jr::Year::get()
{
    return std::to_string(m_year);
}

/* 月份结构实现 */
void jr::Month::reset()
{
    m_month = MIN_MONTH;
}

void jr::Month::checkMonth(uint32_t month)
{
    if (unlikely(month < MIN_MONTH || month > MAX_MONTH)) {
        throw JRException("[%s] month:%u is invalid!", __func__, month);
    }
}

jr::Month::Month()
{
    reset();
}

jr::Month::Month(uint32_t month)
{
    checkMonth(month);
    m_month = month;
}

std::string jr::Month::get()
{
    std::string monthStr = std::to_string(m_month);
    padZero4String(monthStr, MAX_MONTH_LEN);
    return monthStr;
}

/* 天数结构实现 */
void jr::Day::reset()
{
    m_day = MIN_DAY;
}

void jr::Day::checkDay(uint32_t day)
{
    if (unlikely(day < MIN_DAY || day > MAX_DAY)) {
        throw JRException("[%s] day:%u is invalid!", __func__, day);
    }
}

jr::Day::Day()
{
    reset();
}

jr::Day::Day(uint32_t day)
{
    checkDay(day);
    m_day = day;
}

std::string jr::Day::get()
{
    std::string dayStr = std::to_string(m_day);
    padZero4String(dayStr, MAX_DAY_LEN);
    return dayStr;
}

/* 时分秒结构实现 */
void jr::HmsTime::reset()
{
    m_hour = 0;
    m_minute = 0;
    m_second = 0;
    m_ms = 0;
}

void jr::HmsTime::checkHmsTime(uint32_t hour, uint32_t minute, uint32_t second, uint32_t ms)
{
    if (unlikely(hour >= MAX_HOUR || minute >= MAX_MINUTE || second >= MAX_SECOND || ms >= MAX_MS)) {
        throw JRException("[%s] hour:%u minute:%u second:%u ms:%u is invalid!", __func__, hour, minute, second, ms);
    }
}

jr::HmsTime::HmsTime()
{
    reset();
}

jr::HmsTime::HmsTime(uint32_t hour, uint32_t minute, uint32_t second, uint32_t ms)
{
    checkHmsTime(hour, minute, second, ms);
    m_hour = hour;
    m_minute = minute;
    m_second = second;
    m_ms = ms;
}

std::string jr::HmsTime::get()
{
    std::string hourStr = std::to_string(m_hour);
    std::string minuteStr = std::to_string(m_minute);
    std::string secondStr = std::to_string(m_second);
    std::string msStr = std::to_string(m_ms);
    padZero4String(hourStr, MAX_HOUR_LEN);
    padZero4String(minuteStr, MAX_MINUTE_LEN);
    padZero4String(secondStr, MAX_SECOND_LEN);
    padZero4String(msStr, MAX_MS_LEN);
    return hourStr + ":" + minuteStr + ":" + secondStr + ":" + msStr;
}

/* 时间类实现 */
void jr::JRTime::showTime()
{
    jr::LogGuard logGuard("JRTime::showTime");
    Logs.log() << jr_year.get() << "-" << jr_month.get() << "-" << jr_day.get() << " " << jr_hmsTime.get() << std::endl;
}

void jr::JRTime::setCurrentTime()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm *timeStru = std::localtime(&nowTime);

    if (unlikely(timeStru == nullptr)) {
        throw JRException("[%s] call localtime fail!", __func__);
    }

    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    jr_year = Year(static_cast<uint32_t>(timeStru->tm_year + 1900));
    jr_month = Month(static_cast<uint32_t>(timeStru->tm_mon + 1));
    jr_day = Day(static_cast<uint32_t>(timeStru->tm_mday));
    jr_hmsTime = HmsTime(static_cast<uint32_t>(timeStru->tm_hour),
                         static_cast<uint32_t>(timeStru->tm_min),
                         static_cast<uint32_t>(timeStru->tm_sec),
                         static_cast<uint32_t>(ms.count()));
}

std::string jr::JRTime::getCurrentTime()
{
    setCurrentTime();
    return jr_year.get() + "-" + jr_month.get() + "-" + jr_day.get() + " " + jr_hmsTime.get();
}