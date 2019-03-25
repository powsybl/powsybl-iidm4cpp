/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_LOGGING_LOGMESSAGE_HPP
#define POWSYBL_LOGGING_LOGMESSAGE_HPP

#include <ctime>

#include <powsybl/logging/Level.hpp>

namespace powsybl {

namespace logging {

class LogMessage {
public:
    LogMessage(const Level& level, const std::string& message);

    ~LogMessage() noexcept = default;

    const std::time_t& getInstant() const;

    const Level& getLevel() const;

    const std::string& getMessage() const;

    std::string toString() const;

private:
    std::time_t m_instant;

    Level m_level;

    std::string m_message;
};

std::ostream& operator<<(std::ostream& stream, const LogMessage& logMessage);

}  // namespace logging

}  // namespace powsybl

#endif  // POWSYBL_LOGGING_LOGMESSAGE_HPP
