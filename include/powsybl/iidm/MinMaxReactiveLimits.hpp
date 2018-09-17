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
    virtual ReactiveLimitsKind getKind() const override;

    virtual double getMaxQ(double p) const override;

    virtual double getMinQ(double p) const override;

public:
    MinMaxReactiveLimits(double minQ, double maxQ);

    MinMaxReactiveLimits(const MinMaxReactiveLimits&) = default;

    virtual ~MinMaxReactiveLimits() noexcept = default;

    MinMaxReactiveLimits& operator=(const MinMaxReactiveLimits&) = default;

    double getMaxQ() const;

    double getMinQ() const;

private:
    double m_minQ;

    double m_maxQ;
};

}

}

#endif //POWSYBL_IIDM_MINMAXREACTIVELIMITS_HPP
