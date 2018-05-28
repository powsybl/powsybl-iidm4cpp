/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ReactiveCapabilityCurve.hpp>

#include <cassert>
#include <powsybl/AssertionError.hpp>

namespace powsybl {

namespace iidm {

ReactiveCapabilityCurve::Point::Point(double p, double minQ, double maxQ):
    m_p(p),
    m_minQ(minQ),
    m_maxQ(maxQ) {
}

double ReactiveCapabilityCurve::Point::getMaxQ() const {
    return m_maxQ;
}

double ReactiveCapabilityCurve::Point::getMinQ() const {
    return m_minQ;
}

double ReactiveCapabilityCurve::Point::getP() const {
    return m_p;
}

ReactiveCapabilityCurve::ReactiveCapabilityCurve(std::map<double, Point>&& points):
    m_points(std::move(points)) {

    assert(m_points.size() >= 2);
}

ReactiveLimitsKind ReactiveCapabilityCurve::getKind() const {
    return ReactiveLimitsKind::CURVE;
}

double ReactiveCapabilityCurve::getMaxP() const {
    return m_points.rbegin()->first;
}

double ReactiveCapabilityCurve::getMaxQ(double p) const {
    assert(m_points.size() >= 2);

    const auto& it = m_points.lower_bound(p);
    if (it != m_points.end() && it->first == p) {
        return it->second.getMaxQ();
    } else {
        const auto& it1 = (it == m_points.cbegin() ? m_points.cend() : std::prev(it));
        const auto& it2 = it;

        if (it1 == m_points.end() && it2 != m_points.end()) {
            return it2->second.getMaxQ();
        } else if (it1 != m_points.end() && it2 == m_points.end()) {
            return it1->second.getMaxQ();
        } else if (it1 != m_points.end() && it2 != m_points.end()) {
            const auto& p1 = it1->second;
            const auto& p2 = it2->second;

            return p1.getMaxQ() + (p2.getMaxQ() - p1.getMaxQ()) / (p2.getP() - p1.getP()) * (p - p1.getP());
        } else {
            throw AssertionError("Should not happen");
        }
    }
}

double ReactiveCapabilityCurve::getMinP() const {
    return m_points.begin()->first;
}

double ReactiveCapabilityCurve::getMinQ(double p) const {
    assert(m_points.size() >= 2);

    const auto& it = m_points.lower_bound(p);
    if (it != m_points.end() && it->first == p) {
        return it->second.getMinQ();
    } else {
        const auto& it1 = (it == m_points.cbegin() ? m_points.cend() : std::prev(it));
        const auto& it2 = it;

        if (it1 == m_points.end() && it2 != m_points.end()) {
            return it2->second.getMinQ();
        } else if (it1 != m_points.end() && it2 == m_points.end()) {
            return it1->second.getMinQ();
        } else if (it1 != m_points.end() && it2 != m_points.end()) {
            const auto& p1 = it1->second;
            const auto& p2 = it2->second;

            return p1.getMinQ() + (p2.getMinQ() - p1.getMinQ()) / (p2.getP() - p1.getP()) * (p - p1.getP());
        } else {
            throw AssertionError("Should not happen");
        }
    }
}

unsigned long ReactiveCapabilityCurve::getPointCount() const {
    return m_points.size();
}

const std::map<double, ReactiveCapabilityCurve::Point>& ReactiveCapabilityCurve::getPoints() const {
    return m_points;
}

}  // namespace iidm

}  // namespace powsybl
