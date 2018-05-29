/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "powsybl/logging/ConsoleLogger.hpp"

#include <chrono>
#include <iostream>

#include <powsybl/stdcxx/put_time.hpp>

namespace powsybl {

namespace logging {

std::string now() {
    const std::time_t& now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    struct std::tm result = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr};
    if (localtime_r(&now, &result) == nullptr) {
        // TODO: throw an AssertionError
    }

    std::ostringstream oss;
    oss << stdcxx::put_time(&result, "%Y-%m-%d %X");

    return oss.str();
}

bool ConsoleLogger::isDebugEnabled() const {
    return true;
}

bool ConsoleLogger::isErrorEnabled() const {
    return true;
}

bool ConsoleLogger::isInfoEnabled() const {
    return true;
}

bool ConsoleLogger::isTraceEnabled() const {
    return true;
}

bool ConsoleLogger::isWarnEnabled() const {
    return true;
}

void ConsoleLogger::log(Level level, const std::string& message) {
    if (level == Level::ERROR) {
        std::cerr << now() << " - " << getLevelName(level) << " - " << message << std::endl;
    } else {
        std::cout << now() << " - " << getLevelName(level) << " - " << message << std::endl;
    }
}

}

}