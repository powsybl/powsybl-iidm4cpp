/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CURRENTLIMITS_HPP
#define POWSYBL_IIDM_CURRENTLIMITS_HPP

#include <functional>
#include <map>
#include <string>
#include <vector>

#include <powsybl/iidm/LoadingLimits.hpp>

namespace powsybl {

namespace iidm {

class CurrentLimits : public LoadingLimits {
public:  // OperationalLimits
    const LimitType& getLimitType() const override;

    void remove() override;

public:
    CurrentLimits(OperationalLimitsHolder& owner, double permanentLimit, LoadingLimits::TemporaryLimits temporaryLimits);

    CurrentLimits(const CurrentLimits&) = default;

    CurrentLimits(CurrentLimits&&) noexcept = default;

    ~CurrentLimits() override = default;

    CurrentLimits& operator=(const CurrentLimits&) = delete;

    CurrentLimits& operator=(CurrentLimits&&) noexcept = delete;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CURRENTLIMITS_HPP
