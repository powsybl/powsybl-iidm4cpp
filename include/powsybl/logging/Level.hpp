/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_LOGGING_LEVEL_HPP
#define POWSYBL_LOGGING_LEVEL_HPP

#include <string>

#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace logging {

enum class Level : unsigned int {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR
};

/**
 * Get the name of the specified log level
 *
 * @param level the log level to convert to string
 *
 * @return the name of the specified log level
 */
IIDM_DECLSPEC std::string getLevelName(const Level& level);

}  // namespace logging

}  // namespace powsybl

#endif  // POWSYBL_LOGGING_LEVEL_HPP
