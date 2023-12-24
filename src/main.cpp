#include "JRTime.h"
#include "JRbase.h"
#include "JRdataSys.h"
#include "JRware.h"
#include <iostream>
#include <string>

//todo: 
// 1.设计新闻记忆系统：收集、查询
// ----功能1. 记录功能：输入新闻字符串，记录在本地数据存储系统
// ----功能2. 查询功能：精确查询+模糊查询
// 2.实现本地数据存储功能，对接新闻记忆系统
// ----功能1. 以事件为单位，基类格式（事件ID，时间，标题，内容，分类）
// 实现类似存储系统

int jrMain()
{
    GlobalConfigs.init();
    Logs.setLogPath(GlobalConfigs.getLogPath());
    JRdataSystem.init(GlobalConfigs.getDataSysPath());
    jr::JRware worker;
    worker.start();
    
    return jr::SUCCESS;
}

int main()
{
    try {
        int ret = jrMain();
        if (ret == jr::SUCCESS) {
            Logs.log() << "exec success!" << std::endl;
        } else {
            Logs.log() << "exec fail!" << std::endl;
        }
    }
    catch (const jr::JRException &e) {
        Logs.log() << e.what() << std::endl;
    } catch (...) {
        Logs.log() << "unknown error!" << std::endl;
    }
    Logs.close();
    JRdataSystem.close();
}
