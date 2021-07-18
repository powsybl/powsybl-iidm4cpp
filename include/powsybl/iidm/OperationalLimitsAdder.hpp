/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OPERATIONALLIMITSADDER_HPP
#define POWSYBL_IIDM_OPERATIONALLIMITSADDER_HPP

namespace powsybl {

namespace iidm {

template <typename L>
class OperationalLimitsAdder {
public:
    OperationalLimitsAdder() = default;

    OperationalLimitsAdder(const OperationalLimitsAdder&) = default;

    OperationalLimitsAdder(OperationalLimitsAdder&&) noexcept = delete;

    virtual ~OperationalLimitsAdder() = default;

    OperationalLimitsAdder& operator=(const OperationalLimitsAdder&) = default;

    OperationalLimitsAdder& operator=(OperationalLimitsAdder&&) = delete;

    virtual L& add() = 0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_OPERATIONALLIMITSADDER_HPP

