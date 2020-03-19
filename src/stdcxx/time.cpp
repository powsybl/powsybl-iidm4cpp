/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/stdcxx/time.hpp>

#include <boost/date_time/c_time.hpp>

namespace stdcxx {

std::tm localtime(const std::time_t& time) {
#if defined __GNUC__
    std::tm result {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr};
#else
    std::tm result {0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif
    boost::date_time::c_time::localtime(&time, &result);
    return result;
}

#if !HAS_PUT_TIME

std::string put_time(const struct std::tm* time, const char* format) {
    char buffer[128];

    if (strftime(buffer, 128, format, time) > 0) {
        return buffer;
    }

    return "";
}

#endif  // HAS_PUT_TIME

}  // namespace stdcxx

