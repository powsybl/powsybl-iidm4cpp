/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/logging/ConsoleLogger.hpp>

#include <iostream>

#include <powsybl/logging/LogMessage.hpp>

namespace powsybl {

namespace logging {

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

void ConsoleLogger::log(const Level& level, const std::string& message) {
    std::clog << LogMessage(level, message) << std::endl;
}

}  // namespace logging

}  // namespace powsybl
