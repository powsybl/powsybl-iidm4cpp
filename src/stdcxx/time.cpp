/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/stdcxx/time.hpp>

namespace stdcxx {

std::tm tm() {
#if defined __GNUC__
    return {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr};
#else
    return {0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif
}

}  // namespace stdcxx

