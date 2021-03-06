/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_ASSERTIONERROR_HPP
#define POWSYBL_ASSERTIONERROR_HPP

#include <stdexcept>

namespace powsybl {

class AssertionError : public std::runtime_error {
public:
    explicit AssertionError(const std::string& message);

    ~AssertionError() noexcept override = default;
};

}  // namespace powsybl

#endif  // POWSYBL_ASSERTIONERROR_HPP
