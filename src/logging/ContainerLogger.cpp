/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/logging/ContainerLogger.hpp>

namespace powsybl {

namespace logging {

ContainerLogger::Logs::const_iterator ContainerLogger::cbegin() const {
    return m_logs.cbegin();
}

ContainerLogger::Logs::const_iterator ContainerLogger::cend() const {
    return m_logs.cend();
}

const LogMessage& ContainerLogger::getLogMessage(unsigned long index) const {
    return m_logs.at(index);
}

bool ContainerLogger::isDebugEnabled() const {
    return true;
}

bool ContainerLogger::isErrorEnabled() const {
    return true;
}

bool ContainerLogger::isInfoEnabled() const {
    return true;
}

bool ContainerLogger::isTraceEnabled() const {
    return true;
}

bool ContainerLogger::isWarnEnabled() const {
    return true;
}

void ContainerLogger::log(const Level& level, const std::string& message) {
    m_logs.emplace_back(level, message);
}

unsigned long ContainerLogger::size() const {
    return m_logs.size();
}

}  // namespace logging

}  // namespace powsybl
