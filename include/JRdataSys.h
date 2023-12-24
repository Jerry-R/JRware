#ifndef JRDATASYS_H
#define JRDATASYS_H
#include <string>
#include <fstream>
#include <vector>
#include <boost/serialization/singleton.hpp>

namespace jr
{
#define JRdataSystem jr::JRdataSys::get_mutable_instance()
    static constexpr char *LEFT_TAR = "[";
    static constexpr char *RIGHT_TAR = "]";
    static constexpr uint32_t MAX_LINE_NUM = 10000;
    /* 数据坐标 */
    struct Pos
    {
    private:
        uint32_t m_x;
        uint32_t m_y;

    public:
        Pos();
        Pos(uint32_t x, uint32_t y);
        uint32_t X();
        uint32_t Y();
        void reset();
    };

    /* 数据单元格 */
    struct Cell
    {
    private:
        Pos m_pos;
        std::string m_data;

    public:
        Cell();
        void setCell(Pos pos, const std::string &data);
        void reset();
        Pos getPos();
        std::string getData();
    };

    /* 基础数据事件, 由一系列数据单元组成
     * 其格式为 【事件ID】，【时间】，【标题】，【内容】，【分类】
     */
    struct Event
    {

    private:
        std::vector<Cell> m_event;
        bool checkEventData(const std::string &data);

    public:
        enum EventAttr
        {
            EVENT_ID = 0,      // 事件ID
            EVENT_TIME = 1,    // 时间
            EVENT_TITLE = 2,   // 标题
            EVENT_DATA = 3,    // 内容
            EVENT_CLASS = 4,   // 分类
            EVENT_ATTR_MAX = 5 // 事件属性总数
        };
        Event();
        void reset();
        void setEvent(uint32_t posX, const EventAttr &attr, const std::string &data);
    };

    /* 数据存储系统 */
    class JRdataSys : public boost::serialization::singleton<JRdataSys>
    {
    private:
        std::fstream m_dataConnect;
        uint32_t m_curLineNo;
        void convertStr2Event(uint32_t lineNo, std::string &lineStr, Event &event);

    public:
        void init(const std::string &path);
        void close();
        uint32_t getCurLineNo();
        void pushLine(const std::string &title, const std::string &data, const std::string &eventClass);
        void readAllLine(std::vector<Event> &out);
    };
}

#endif