/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_POWSYBLEXCEPTION_HPP
#define POWSYBL_POWSYBLEXCEPTION_HPP

#include <stdexcept>

namespace powsybl {

class PowsyblException : public std::runtime_error {
public:
    explicit PowsyblException(const std::string& message);

    ~PowsyblException() noexcept override = default;
};

}  // namespace powsybl

#endif  // POWSYBL_POWSYBLEXCEPTION_HPP
