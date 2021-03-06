/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_REACTIVECAPABILITYCURVEADDER_HPP
#define POWSYBL_IIDM_REACTIVECAPABILITYCURVEADDER_HPP

#include <powsybl/iidm/ReactiveCapabilityCurve.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class ReactiveLimitsHolder;
class Validable;

class ReactiveCapabilityCurveAdder {
public:
    class PointAdder {
    public:
        ReactiveCapabilityCurveAdder& endPoint();

        PointAdder& setMaxQ(double maxQ);

        PointAdder& setMinQ(double minQ);

        PointAdder& setP(double p);

    private:
        explicit PointAdder(ReactiveCapabilityCurveAdder& adder);

        friend ReactiveCapabilityCurveAdder;

    private:
        ReactiveCapabilityCurveAdder& m_adder;

        double m_p = stdcxx::nan();

        double m_minQ = stdcxx::nan();

        double m_maxQ = stdcxx::nan();
    };

public:
    explicit ReactiveCapabilityCurveAdder(ReactiveLimitsHolder& owner);

    ~ReactiveCapabilityCurveAdder() noexcept = default;

    ReactiveCapabilityCurve& add();

    PointAdder beginPoint();

private:
    ReactiveCapabilityCurveAdder& addPoint(ReactiveCapabilityCurve::Point&& point);

    const Validable& getValidable() const;

private:
    ReactiveLimitsHolder& m_owner;

    std::map<double, ReactiveCapabilityCurve::Point> m_points;
};

}  // namespace iidm

}  // namespace powsybl

#endif //POWSYBL_IIDM_REACTIVECAPABILITYCURVEADDER_HPP
