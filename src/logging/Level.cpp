/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/logging/Level.hpp>

#include <array>

#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace logging {

std::string getLevelName(const Level& level) {
    static std::array<std::string, 5> s_levelNames {{
        "TRACE",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR"
    }};

    unsigned int val = static_cast<unsigned int>(level);
    return (val < s_levelNames.size()) ? s_levelNames.at(val) : format("%1%", val);
}

}  // namespace logging

}  // namespace powsybl
