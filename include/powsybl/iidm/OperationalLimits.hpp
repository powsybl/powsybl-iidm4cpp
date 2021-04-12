/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OPERATIONALLIMITS_HPP
#define POWSYBL_IIDM_OPERATIONALLIMITS_HPP

#include <powsybl/iidm/LimitType.hpp>

namespace powsybl {

namespace iidm {

class OperationalLimits {
public:
    OperationalLimits() = default;

    OperationalLimits(const OperationalLimits&) = default;

    OperationalLimits(OperationalLimits&&) noexcept = default;

    virtual ~OperationalLimits() = default;

    OperationalLimits& operator=(const OperationalLimits&) = delete;

    OperationalLimits& operator=(OperationalLimits&&) noexcept = delete;

    virtual const LimitType& getLimitType() const = 0;

    virtual void remove();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_OPERATIONALLIMITS_HPP
