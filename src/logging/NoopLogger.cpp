/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "powsybl/logging/NoopLogger.hpp"

namespace powsybl {

namespace logging {

bool NoopLogger::isDebugEnabled() const {
    return false;
}

bool NoopLogger::isErrorEnabled() const {
    return false;
}

bool NoopLogger::isInfoEnabled() const {
    return false;
}

bool NoopLogger::isWarnEnabled() const {
    return false;
}

bool NoopLogger::isTraceEnabled() const {
    return false;
}

void NoopLogger::log(const Level& /*level*/, const std::string& /*message*/) {
    // Nothing to do
}

}  // namespace logging

}  // namespace powsybl
