#include "JRware.h"
#include "JRbase.h"
#include <iostream>
#include <windows.h>

/* 交互服务类实现 */
jr::JRware::JRware()
{
    m_runMode = NORMAL_MODE;
}

void jr::JRware::start()
{
    help();
    do {
        Sleep(m_runMode);
        Logs.log() << "heart beat!" << std::endl;
    } while (true);
}

void jr::JRware::help()
{
    std::cout << "Welcome to JRware!" << std::endl;
    std::cout << "You can input cmds like this:" << std::endl;
    std::cout << "i\t";
}