/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/AssertionError.hpp>

namespace powsybl {

AssertionError::AssertionError(const std::string& message) :
    std::runtime_error(message) {
}

}  // namespace powsybl
