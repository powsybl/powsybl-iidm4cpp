/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/logging/Level.hpp>

#include <array>

#include <powsybl/AssertionError.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/format.hpp>

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

    auto index = static_cast<unsigned char>(level);
    if (index >= s_levelNames.size()) {
        throw powsybl::AssertionError(stdcxx::format("Unexpected %1% value: %2%", stdcxx::simpleClassName<Level>(), index));
    }
    return *(s_levelNames.begin() + index);
}

}  // namespace logging

}  // namespace powsybl
