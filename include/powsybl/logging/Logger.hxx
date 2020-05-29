/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_LOGGING_LOGGER_HXX
#define POWSYBL_LOGGING_LOGGER_HXX

#include <powsybl/logging/Logger.hpp>

#include <powsybl/stdcxx/MessageFormat.hpp>

namespace powsybl {

namespace logging {

template <typename... T>
void Logger::debug(const std::string& message, const T&... args) {
    if (isDebugEnabled()) {
        log(Level::DEBUG, stdcxx::format(message, args...));
    }
}

template <typename... T>
void Logger::error(const std::string& message, const T&... args) {
    if (isErrorEnabled()) {
        log(Level::ERROR, stdcxx::format(message, args...));
    }
}

template <typename... T>
void Logger::info(const std::string& message, const T&... args) {
    if (isInfoEnabled()) {
        log(Level::INFO, stdcxx::format(message, args...));
    }
}

template <typename... T>
void Logger::trace(const std::string& message, const T&... args) {
    if (isTraceEnabled()) {
        log(Level::TRACE, stdcxx::format(message, args...));
    }
}

template <typename... T>
void Logger::warn(const std::string& message, const T&... args) {
    if (isWarnEnabled()) {
        log(Level::WARN, stdcxx::format(message, args...));
    }
}

}  // namespace logging

}  // namespace powsybl

#endif  // POWSYBL_LOGGING_LOGGER_HXX
