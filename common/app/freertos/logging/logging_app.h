
#pragma once

#include "FreeRTOS.h"
#include "task.h"

#include "logging.h"


bool create_log_app(LogBuilder *builder, LogSubscriber *subscribers, size_t num_subs, Send *sender, size_t frequency_hz);