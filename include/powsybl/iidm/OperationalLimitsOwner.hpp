/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OPERATIONALLIMITSOWNER_HPP
#define POWSYBL_IIDM_OPERATIONALLIMITSOWNER_HPP

#include <memory>

#include <powsybl/iidm/LimitType.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class OperationalLimits;

class OperationalLimitsOwner : public Validable {
public:
    OperationalLimitsOwner() = default;

    OperationalLimitsOwner(const OperationalLimitsOwner&) = default;

    OperationalLimitsOwner(OperationalLimitsOwner&&) noexcept = default;

    ~OperationalLimitsOwner() noexcept override = default;

    OperationalLimitsOwner& operator=(const OperationalLimitsOwner&) = default;

    OperationalLimitsOwner& operator=(OperationalLimitsOwner&&) noexcept = default;

    virtual stdcxx::Reference<OperationalLimits> setOperationalLimits(const LimitType& limitType, std::unique_ptr<OperationalLimits>&& operationalLimits) = 0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_OPERATIONALLIMITSOWNER_HPP
