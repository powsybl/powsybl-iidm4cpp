/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_LOGGING_LOGGER_HPP
#define POWSYBL_LOGGING_LOGGER_HPP

#include <string>

#include <powsybl/logging/Level.hpp>

namespace powsybl {

namespace logging {

/**
 * The logger interface
 */
class Logger {
public:
    virtual ~Logger() noexcept = default;

    /**
     * Log a debug message
     *
     * @tparam T... the type of the parameters of the message
     *
     * @param message the message to log
     * @param args the parameters of the message
     */
    template <typename... T>
    void debug(const std::string& message, const T&... args);

    /**
     * Log an error message
     *
     * @tparam T... the type of the parameters of the message
     *
     * @param message the message to log
     * @param args the parameters of the message
     */
    template <typename... T>
    void error(const std::string& message, const T&... args);

    /**
     * Log an information message
     *
     * @tparam T... the type of the parameters of the message
     *
     * @param message the message to log
     * @param args the parameters of the message
     */
    template <typename... T>
    void info(const std::string& message, const T&... args);

    /**
     * Return true if debug messages should be logged by this logger, false otherwise
     *
     * @return true if debug messages should be logged by this logger, false otherwise
     */
    virtual bool isDebugEnabled() const = 0;

    /**
     * Return true if error messages should be logged by this logger, false otherwise
     *
     * @return true if error messages should be logged by this logger, false otherwise
     */
    virtual bool isErrorEnabled() const = 0;

    /**
     * Return true if information messages should be logged by this logger, false otherwise
     *
     * @return true if information messages should be logged by this logger, false otherwise
     */
    virtual bool isInfoEnabled() const = 0;

    /**
     * Return true if trace messages should be logged by this logger, false otherwise
     *
     * @return true if trace messages should be logged by this logger, false otherwise
     */
    virtual bool isTraceEnabled() const = 0;

    /**
     * Return true if warning messages should be logged by this logger, false otherwise
     *
     * @return true if warning messages should be logged by this logger, false otherwise
     */
    virtual bool isWarnEnabled() const = 0;

    /**
     * Log a trace message
     *
     * @tparam T... the type of the parameters of the message
     *
     * @param message the message to log
     * @param args the parameters of the message
     */
    template <typename... T>
    void trace(const std::string& message, const T&... args);

    /**
     * Log a warning message
     *
     * @tparam T... the type of the parameters of the message
     *
     * @param message the message to log
     * @param args the parameters of the message
     */
    template <typename... T>
    void warn(const std::string& message, const T&... args);

private:
    /**
     * Log a message
     *
     * @param level the log level of the message
     * @param message the message to log
     */
    virtual void log(Level level, const std::string& message) = 0;
};

}  // namespace logging

}  // namespace powsybl

#include <powsybl/logging/Logger.hxx>

#endif  // POWSYBL_LOGGING_LOGGER_HPP
