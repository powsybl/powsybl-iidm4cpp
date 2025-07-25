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

ConsoleLogger::ConsoleLogger() :
    ConsoleLogger(Level::INFO) {
}

ConsoleLogger::ConsoleLogger(const Level &level) :
    m_level(level) {
}

bool ConsoleLogger::isDebugEnabled() const {
    return m_level <= Level::DEBUG;
}

bool ConsoleLogger::isErrorEnabled() const {
    return m_level <= Level::ERROR;
}

bool ConsoleLogger::isInfoEnabled() const {
    return m_level <= Level::INFO;
}

bool ConsoleLogger::isTraceEnabled() const {
    return m_level <= Level::TRACE;
}

bool ConsoleLogger::isWarnEnabled() const {
    return m_level <= Level::WARN;
}

void ConsoleLogger::log(const Level& level, const std::string& message) {
    std::clog << LogMessage(level, message) << '\n' << std::flush;
}

}  // namespace logging

}  // namespace powsybl
