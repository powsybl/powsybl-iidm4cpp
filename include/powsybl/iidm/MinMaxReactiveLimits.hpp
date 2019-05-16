/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_MINMAXREACTIVELIMITS_HPP
#define POWSYBL_IIDM_MINMAXREACTIVELIMITS_HPP

#include <powsybl/iidm/ReactiveLimits.hpp>

namespace powsybl {

namespace iidm {

class MinMaxReactiveLimits : public ReactiveLimits {
public: // ReactiveLimits
    ReactiveLimitsKind getKind() const override;

    double getMaxQ(double p) const override;

    double getMinQ(double p) const override;

public:
    MinMaxReactiveLimits();

    MinMaxReactiveLimits(double minQ, double maxQ);

    MinMaxReactiveLimits(const MinMaxReactiveLimits&) = default;

    ~MinMaxReactiveLimits() noexcept override = default;

    MinMaxReactiveLimits& operator=(const MinMaxReactiveLimits&) = default;

    double getMaxQ() const;

    double getMinQ() const;

private:
    double m_minQ;

    double m_maxQ;
};

}  // namespace iidm

}  // namespace powsybl

#endif //POWSYBL_IIDM_MINMAXREACTIVELIMITS_HPP
