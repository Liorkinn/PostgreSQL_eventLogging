#include "variable_log.h"

variable_log* variable_log::variable_log_ = 0;

variable_log* variable_log::instance()
{
    if (!variable_log_) {
        return variable_log_ = new variable_log();
    }
    return variable_log_;
}

variable_log::variable_log()
{
    prDebug_ = NoLog;
    pathLogDebug_ = "";
}

variable_log::~variable_log()
{
    if (variable_log_) {
        delete variable_log_;
    }
}
