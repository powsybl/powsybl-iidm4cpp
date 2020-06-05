/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_EXCEPTION_HPP
#define POWSYBL_STDCXX_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace stdcxx {

class PropertyNotFoundException : public std::runtime_error {
public:
    explicit PropertyNotFoundException(const std::string& message);

    ~PropertyNotFoundException() noexcept override = default;
};

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_EXCEPTION_HPP
