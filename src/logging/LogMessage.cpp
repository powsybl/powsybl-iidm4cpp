/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/logging/LogMessage.hpp>
#include <powsybl/logging/MessageFormat.hpp>
#include <powsybl/stdcxx/put_time.hpp>

#include <powsybl/AssertionError.hpp>

namespace powsybl {

namespace logging {

LogMessage::LogMessage(const std::time_t& instant, const Level& level, const std::string& message) :
    m_instant(instant),
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
    struct std::tm result = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr};
    if (localtime_r(&m_instant, &result) == nullptr) {
        throw AssertionError(format("Unable to get the date."));
    }
    std::ostringstream oss;
    oss << stdcxx::put_time(&result, "%Y-%m-%d %X") << " - " << getLevelName(m_level) << " - " << m_message;

    return oss.str();
}

}  // namespace logging

}  // namespace powsybl
