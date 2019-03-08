/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/stdcxx/put_time.hpp>

#if !HAS_PUT_TIME

namespace stdcxx {

std::string put_time(const struct std::tm* time, const char* format) {
    char buffer[128];

    if (strftime(buffer, 128, format, time) > 0) {
        return buffer;
    }

    return "";
}

}  // namespace stdcxx

#endif  // HAS_PUT_TIME
