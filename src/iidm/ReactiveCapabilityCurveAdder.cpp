/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ReactiveCapabilityCurveAdder.hpp>

#include <powsybl/iidm/ReactiveLimitsHolder.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/logging/MessageFormat.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

ReactiveCapabilityCurveAdder::PointAdder::PointAdder(ReactiveCapabilityCurveAdder& adder) :
    m_adder(adder) {
}

ReactiveCapabilityCurveAdder& ReactiveCapabilityCurveAdder::PointAdder::endPoint() {
    const auto& owner = m_adder.getValidable();

    checkOptional(owner, m_p, "P is not set");
    checkOptional(owner, m_minQ, "min Q is not set");
    checkOptional(owner, m_maxQ, "max Q is not set");

    return m_adder.addPoint(ReactiveCapabilityCurve::Point(m_p, m_minQ, m_maxQ));
}

ReactiveCapabilityCurveAdder::PointAdder& ReactiveCapabilityCurveAdder::PointAdder::setMaxQ(double maxQ) {
    m_maxQ = maxQ;
    return *this;
}

ReactiveCapabilityCurveAdder::PointAdder& ReactiveCapabilityCurveAdder::PointAdder::setMinQ(double minQ) {
    m_minQ = minQ;
    return *this;
}

ReactiveCapabilityCurveAdder::PointAdder& ReactiveCapabilityCurveAdder::PointAdder::setP(double p) {
    m_p = p;
    return *this;
}

ReactiveCapabilityCurveAdder::ReactiveCapabilityCurveAdder(ReactiveLimitsHolder& owner):
    m_owner(owner) {
}

ReactiveCapabilityCurve& ReactiveCapabilityCurveAdder::add() {
    const auto& owner = dynamic_cast<Validable&>(m_owner);

    if (m_points.size() < 2) {
        throw ValidationException(owner, "A reactive capability curve should have at least two points");
    }

    std::unique_ptr<ReactiveCapabilityCurve> limits = stdcxx::make_unique<ReactiveCapabilityCurve>(std::move(m_points));
    m_owner.setReactiveLimits(std::move(limits));

    return m_owner.getReactiveLimits<ReactiveCapabilityCurve>();
}

ReactiveCapabilityCurveAdder& ReactiveCapabilityCurveAdder::addPoint(ReactiveCapabilityCurve::Point&& point) {
    const auto& owner = getValidable();

    if (m_points.find(point.getP()) != m_points.end()) {
        throw ValidationException(owner, logging::format("A point is already defined for active power %1%", point.getP()));
    }

    // TODO(mathbagu): to be activated in IIDM v1.1
    // if (point.getMaxQ() < point.getMinQ()) {
    //     throw ValidationException(owner, "Maximum reactive power is expected to be greater than or equal to minimum reactive power");
    // }

    m_points.insert(std::make_pair(point.getP(), point));

    return *this;
}

ReactiveCapabilityCurveAdder::PointAdder ReactiveCapabilityCurveAdder::beginPoint() {
    return PointAdder(*this);
}

const Validable& ReactiveCapabilityCurveAdder::getValidable() const {
    return dynamic_cast<const Validable&>(m_owner);
}

}  // namespace iidm

}  // namespace powsybl
