/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_TIME_HPP
#define POWSYBL_STDCXX_TIME_HPP

#include <ctime>

#include <ctime>

#include <ctime>

#include <powsybl/stdcxx/export.hpp>
#include <powsybl/stdcxx/features.hpp>

namespace stdcxx {

IIDM_DECLSPEC std::tm tm();

}  // namespace stdcxx

#if !defined(__GNUC__) || __GNUC_PREREQ(5, 0)

#include <iomanip>

#define HAS_PUT_TIME 1

namespace stdcxx {

using std::put_time;

}  // namespace stdcxx

#else

#include <string>

#define HAS_PUT_TIME 0

namespace stdcxx {

std::string put_time(const struct std::tm* time, const char* format);

}  // namespace stdcxx

#endif

namespace stdcxx {

IIDM_DECLSPEC std::tm localtime(const std::time_t& time);

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_TIME_HPP

