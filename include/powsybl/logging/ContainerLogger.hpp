/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_LOGGING_CONTAINERLOGGER_HPP
#define POWSYBL_LOGGING_CONTAINERLOGGER_HPP

#include <powsybl/logging/LogMessage.hpp>
#include <powsybl/logging/Logger.hpp>

namespace powsybl {

namespace logging {

/**
 * A Logger implementation which log all messages to a container
 */
class ContainerLogger : public Logger {
public:
    typedef std::vector<LogMessage> Logs;

public: // Logger
    bool isDebugEnabled() const override;

    bool isErrorEnabled() const override;

    bool isInfoEnabled() const override;

    bool isTraceEnabled() const override;

    bool isWarnEnabled() const override;

public:
    ContainerLogger() = default;

    ~ContainerLogger() noexcept override = default;

    Logs::const_iterator cbegin() const;

    Logs::const_iterator cend() const;

    const LogMessage& getLogMessage(unsigned long index) const;

    unsigned long size() const;

private: // Logger
    void log(const Level& level, const std::string& message) override;

private:
    Logs m_logs;
};

}  // namespace logging

}  // namespace powsybl

#endif  // POWSYBL_LOGGING_CONTAINERLOGGER_HPP
