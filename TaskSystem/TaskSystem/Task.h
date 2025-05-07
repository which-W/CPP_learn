#pragma once
#include"global.h"

struct Task
{
    int id;
    std::string description;
    int priority; // 1-高, 2-中, 3-低
    std::string dueDate; // 格式: YYYY-MM-DD

    std::string toString() const {
        std::ostringstream oss;
        oss << "ID: " << id
            << ", 描述: " << description
            << ", 优先级: " << priority
            << ", 截止日期: " << dueDate;
        return oss.str();
    }
};