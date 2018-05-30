/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_PUT_TIME_HPP
#define POWSYBL_IIDM_PUT_TIME_HPP

#ifndef __GNUC__
#    define HAS_PUT_TIME 1
#else
#    include <features.h>
#    if __GNUC_PREREQ(5, 0)
#        define HAS_PUT_TIME 1
#    else
#        define HAS_PUT_TIME 0
#    endif  // __GNUC_PREREQ(5, 0)
#endif  // __GNUC__

#if HAS_PUT_TIME

#include <iomanip>

namespace stdcxx {

using std::put_time;

}  // namespace stdcxx

#else

#include <ctime>
#include <string>

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

#endif  // POWSYBL_IIDM_PUT_TIME_HPP
