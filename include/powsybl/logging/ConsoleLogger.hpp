/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_LOGGING_CONSOLELOGGER_HPP
#define POWSYBL_LOGGING_CONSOLELOGGER_HPP

#include <powsybl/logging/Logger.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace logging {

/**
 * A Logger implementation which log all messages to the standard output
 */
class IIDM_DECLSPEC ConsoleLogger : public Logger {
public: // Logger
    bool isDebugEnabled() const override;

    bool isErrorEnabled() const override;

    bool isInfoEnabled() const override;

    bool isTraceEnabled() const override;

    bool isWarnEnabled() const override;

public:
    ConsoleLogger() = default;

    ~ConsoleLogger() noexcept override = default;

private: // Logger
    void log(const Level& level, const std::string& message) override;
};

}  // namespace logging

}  // namespace powsybl

#endif  // POWSYBL_LOGGING_CONSOLELOGGER_HPP
