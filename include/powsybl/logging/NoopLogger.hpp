/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NOOPLOGGER_HPP
#define POWSYBL_IIDM_NOOPLOGGER_HPP

#include <powsybl/logging/Logger.hpp>

namespace powsybl {

namespace logging {

/**
 * A Logger implementation for which all log levels are disabled
 */
class NoopLogger : public Logger {
public: // Logger
    bool isDebugEnabled() const override;

    bool isErrorEnabled() const override;

    bool isInfoEnabled() const override;

    bool isTraceEnabled() const override;

    bool isWarnEnabled() const override;

public:
    NoopLogger() = default;

    virtual ~NoopLogger() = default;

private: // Logger
    void log(Level level, const std::string& message) override;
};

}  // namespace logging

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NOOPLOGGER_HPP
