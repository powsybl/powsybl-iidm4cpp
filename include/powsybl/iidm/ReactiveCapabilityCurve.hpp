/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_REACTIVECAPABILITYCURVE_HPP
#define POWSYBL_IIDM_REACTIVECAPABILITYCURVE_HPP

#include <map>

#include <powsybl/iidm/ReactiveLimits.hpp>

namespace powsybl {

namespace iidm {

class ReactiveCapabilityCurve : public ReactiveLimits {
public:
    class Point {
    public:
        Point(double p, double minQ, double maxQ);

        ~Point() noexcept = default;

        double getMaxQ() const;

        double getMinQ() const;

        double getP() const;

    private:
        double m_p;

        double m_minQ;

        double m_maxQ;
    };

public: // ReactiveLimits
    virtual ReactiveLimitsKind getKind() const override;

    virtual double getMaxQ(double p) const override;

    virtual double getMinQ(double p) const override;

public:
    explicit ReactiveCapabilityCurve(std::map<double, Point>&& points);

    ReactiveCapabilityCurve(const ReactiveCapabilityCurve&) = default;

    virtual ~ReactiveCapabilityCurve() = default;

    ReactiveCapabilityCurve& operator=(const ReactiveCapabilityCurve&) = default;

    double getMaxP() const;

    double getMinP() const;

    unsigned long getPointCount() const;

    const std::map<double, Point>& getPoints() const;

private:
    std::map<double, Point> m_points;
};

}  // namespace iidm

}  // namespace powsybl

#endif //POWSYBL_IIDM_REACTIVECAPABILITYCURVE_HPP
