/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/logging/LoggerFactory.hpp>

#include "powsybl/logging/NoopLogger.hpp"

namespace powsybl {

namespace logging {

void LoggerFactory::addLogger(const std::string& name, std::unique_ptr<Logger>&& logger) {
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_loggers.insert(std::make_pair(name, std::move(logger)));
}

LoggerFactory& LoggerFactory::getInstance() {
    static LoggerFactory s_instance;

    return s_instance;
}

Logger& LoggerFactory::getLogger(const std::string& name) {
    return getInstance().getLoggerByName(name);
}

Logger& LoggerFactory::getLoggerByName(const std::string& name) const {
    static NoopLogger s_defaultLogger;

    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    const auto& it = m_loggers.find(name);
    if (it != m_loggers.end()) {
        return *(it->second);
    }

    std::size_t pos = name.rfind("::");
    if (pos != std::string::npos) {
        return getLoggerByName(name.substr(0, pos));
    }

    return s_defaultLogger;
}

}

}
