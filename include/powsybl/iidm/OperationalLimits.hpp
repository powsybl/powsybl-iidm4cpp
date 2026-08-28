/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OPERATIONALLIMITS_HPP
#define POWSYBL_IIDM_OPERATIONALLIMITS_HPP

#include <powsybl/iidm/LimitType.hpp>
#include <powsybl/iidm/PropertiesHolder.hpp>

namespace powsybl {

namespace iidm {

class OperationalLimits : public PropertiesHolder {
public:
    OperationalLimits() = default;

    OperationalLimits(const OperationalLimits&) = default;

    OperationalLimits(OperationalLimits&&) noexcept = default;

    virtual ~OperationalLimits() noexcept = default;

    OperationalLimits& operator=(const OperationalLimits&) = default;

    OperationalLimits& operator=(OperationalLimits&&) noexcept = default;


    virtual const LimitType& getLimitType() const = 0;

    virtual void remove() = 0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_OPERATIONALLIMITS_HPP
