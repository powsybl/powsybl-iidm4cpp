/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VALIDATIONEXCEPTION_HPP
#define POWSYBL_IIDM_VALIDATIONEXCEPTION_HPP

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Validable.hpp>

namespace powsybl {

namespace iidm {

class ValidationException : public PowsyblException {
public:
    ValidationException(const Validable& validable, const std::string& message);

    virtual ~ValidationException() = default;
};

}

}

#endif  // POWSYBL_IIDM_VALIDATIONEXCEPTION_HPP
