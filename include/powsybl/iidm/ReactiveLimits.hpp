/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_REACTIVELIMITS_HPP
#define POWSYBL_IIDM_REACTIVELIMITS_HPP

#include <powsybl/iidm/ReactiveLimitsKind.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

class IIDM_DECLSPEC ReactiveLimits {
public:
    ReactiveLimits() = default;

    ReactiveLimits(const ReactiveLimits&) = default;

    virtual ~ReactiveLimits() noexcept = default;

    ReactiveLimits& operator=(const ReactiveLimits&) = default;

    virtual ReactiveLimitsKind getKind() const = 0;

    virtual double getMinQ(double p) const = 0;

    virtual double getMaxQ(double p) const = 0;
};

}  // namespace iidm

}  // namespace powsybl

#endif //POWSYBL_IIDM_REACTIVELIMITS_HPP
