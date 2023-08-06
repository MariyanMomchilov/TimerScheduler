#pragma once
#include "./common_types.h"

struct Log {
    time_point_t created;
    std::vector<time_point_t> scheduledForExecution;
    std::vector<time_point_t> executed;
};
