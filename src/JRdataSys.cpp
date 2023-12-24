#include "JRdataSys.h"
#include "JRbase.h"
#include <algorithm>

/* 数据坐标结构实现 */
void jr::Pos::reset()
{
    m_x = 0;
    m_y = 0;
}

jr::Pos::Pos()
{
    reset();
}

jr::Pos::Pos(uint32_t x, uint32_t y)
{
    m_x = x;
    m_y = y;
}

uint32_t jr::Pos::X()
{
    return m_x;
}

uint32_t jr::Pos::Y()
{
    return m_y;
}

/* 数据单元格结构实现 */
void jr::Cell::reset()
{
    m_pos.reset();
    m_data.clear();
}

jr::Cell::Cell()
{
    reset();
}

void jr::Cell::setCell(Pos pos, const std::string &data)
{
    m_pos = pos;
    m_data = data;
}

jr::Pos jr::Cell::getPos()
{
    return m_pos;
}

std::string jr::Cell::getData()
{
    return m_data;
}

/* 基础数据事件结构实现 */
void jr::Event::reset()
{
    m_event.clear();
    m_event.resize(static_cast<uint32_t>(EVENT_ATTR_MAX));
    for_each(m_event.begin(), m_event.end(), [&](jr::Cell &cell) { cell.reset(); });
}

jr::Event::Event()
{
    reset();
}

bool jr::Event::checkEventData(const std::string &data)
{
    if (data.empty() || data.find(LEFT_TAR) != std::string::npos || data.find(RIGHT_TAR) != std::string::npos) {
        Logs.log() << "data:" << data << " is invalid!" << std::endl;
        return false;
    }
    return true;
}

void jr::Event::setEvent(uint32_t posX, const EventAttr &attr, const std::string &data)
{
    uint32_t posY = static_cast<uint32_t>(attr);
    uint32_t eventAttrMax = static_cast<uint32_t>(EVENT_ATTR_MAX);
    if (unlikely(posY >= eventAttrMax)) {
        throw JRException("[%s] attr:%u is invalid!", __func__, posY);
    }

    if (checkEventData(data)) {
        m_event[posY].setCell(Pos(posX ,posY), data);
    }
}

/* 数据存储系统实现 */
void jr::JRdataSys::init(const std::string &path)
{
    jr::LogGuard logGuard("JRdataSys::init");
    static bool init = false;

    if (!init) {
        m_curLineNo = 0;
        m_dataConnect.open(path, std::ios::app | std::ios::in | std::ios::out);
        if (m_dataConnect.fail()) {
            throw JRException("[%s] open path:%s fail!", __func__, path);
        }
        std::string temp;
        while (getline(m_dataConnect, temp, '\n')) {
            m_curLineNo++;
        }
        Logs.log() << "dataSysPath:" << path << std::endl;
        Logs.log() << "CurrLineNo:" << m_curLineNo << std::endl;
        init = true;
    }
    
}

void jr::JRdataSys::close()
{
    m_dataConnect.close();
}

uint32_t jr::JRdataSys::getCurLineNo()
{
    return m_curLineNo;
}

void jr::JRdataSys::pushLine(const std::string &title, const std::string &data, const std::string &eventClass)
{
    jr::LogGuard logGuard("JRdataSys::pushLine");
    JRTime time;
    m_dataConnect.clear();
    m_dataConnect << "[" << std::to_string(m_curLineNo) << "]";
    m_dataConnect << "[" << time.getCurrentTime() << "]";
    m_dataConnect << "[" << title << "]";
    m_dataConnect << "[" << data << "]";
    m_dataConnect << "[" << eventClass << "]" << std::endl;
    m_dataConnect.flush();
    Logs.log() << "CurrLineNo:" << m_curLineNo << "  write finsh!" << std::endl;
    m_curLineNo++;
}

void jr::JRdataSys::convertStr2Event(uint32_t lineNo, std::string &lineStr, Event &event)
{
    std::vector<std::string> lineStrVec;
    splitString(lineStr, '[', lineStrVec);
    for (auto &it : lineStrVec) {
        it = it.substr(0, it.length() - 1);
    }
    if (likely(lineStrVec.size() >= Event::EventAttr::EVENT_ATTR_MAX)) {
        event.setEvent(lineNo, Event::EventAttr::EVENT_ID, lineStrVec[static_cast<uint32_t>(Event::EventAttr::EVENT_ID)]);
        event.setEvent(lineNo, Event::EventAttr::EVENT_TIME, lineStrVec[static_cast<uint32_t>(Event::EventAttr::EVENT_TIME)]);
        event.setEvent(lineNo, Event::EventAttr::EVENT_TITLE, lineStrVec[static_cast<uint32_t>(Event::EventAttr::EVENT_TITLE)]);
        event.setEvent(lineNo, Event::EventAttr::EVENT_DATA, lineStrVec[static_cast<uint32_t>(Event::EventAttr::EVENT_DATA)]);
        event.setEvent(lineNo, Event::EventAttr::EVENT_CLASS, lineStrVec[static_cast<uint32_t>(Event::EventAttr::EVENT_CLASS)]);
    }
}

void jr::JRdataSys::readAllLine(std::vector<jr::Event> &out)
{
    jr::LogGuard logGuard("JRdataSys::readAllLine");
    uint32_t lineNo = 0;
    std::string lineStr;
    Event event;
    m_dataConnect.clear();
    m_dataConnect.seekg(0, std::ios::beg);
    while (getline(m_dataConnect, lineStr, '\n')) {
        event.reset();
        convertStr2Event(lineNo, lineStr, event);
        out.emplace_back(event);
        lineNo++;
    }
    Logs.log() << "read finsh! total line:" << lineNo << std::endl;
}