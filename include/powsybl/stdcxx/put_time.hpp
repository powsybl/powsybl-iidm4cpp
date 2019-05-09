/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_PUT_TIME_HPP
#define POWSYBL_STDCXX_PUT_TIME_HPP

#include <powsybl/stdcxx/features.hpp>

#if !defined(__GNUC__) || __GNUC_PREREQ(5, 0)

#include <iomanip>

namespace stdcxx {

using std::put_time;

}  // namespace stdcxx

#else

#include <ctime>
#include <string>

namespace stdcxx {

std::string put_time(const struct std::tm* time, const char* format);

}  // namespace stdcxx

#endif

#endif  // POWSYBL_STDCXX_PUT_TIME_HPP
