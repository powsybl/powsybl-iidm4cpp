/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/stdcxx/time.hpp>

#include <array>

#include <boost/date_time/c_time.hpp>

namespace stdcxx {

std::tm localtime(const std::time_t& time) {
#if defined __GNUC__
    std::tm result {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr};
    boost::date_time::c_time::localtime(&time, &result);
    return result;
#else
    return *boost::date_time::c_time::localtime(&time, nullptr);
#endif
}

#if !HAS_PUT_TIME

std::string put_time(const struct std::tm* time, const char* format) {
    std::array<char, 128> buffer;

    if (strftime(buffer.data(), buffer.size(), format, time) > 0) {
        return buffer.data();
    }

    return "";
}

#endif  // HAS_PUT_TIME

}  // namespace stdcxx

