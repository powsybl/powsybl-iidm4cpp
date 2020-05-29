/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/logging/LogMessage.hpp>

#include <chrono>

#include <powsybl/stdcxx/MessageFormat.hpp>
#include <powsybl/stdcxx/time.hpp>

#include <powsybl/AssertionError.hpp>

namespace powsybl {

namespace logging {

LogMessage::LogMessage(const Level& level, const std::string& message) :
    m_instant(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())),
    m_level(level),
    m_message(message) {
}

const std::time_t& LogMessage::getInstant() const {
    return m_instant;
}

const Level& LogMessage::getLevel() const {
    return m_level;
}

const std::string& LogMessage::getMessage() const {
    return m_message;
}

std::string LogMessage::toString() const {
    const std::tm& result = stdcxx::localtime(m_instant);

    std::ostringstream oss;
    oss << stdcxx::put_time(&result, "%Y-%m-%d %X") << " - " << getLevelName(m_level) << " - " << m_message;

    return oss.str();
}

std::ostream& operator<<(std::ostream& stream, const LogMessage& logMessage) {
    stream << logMessage.toString();

    return stream;
}

}  // namespace logging

}  // namespace powsybl
