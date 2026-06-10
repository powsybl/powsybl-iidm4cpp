/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DroopCurve.hpp>

namespace powsybl {

namespace iidm {

DroopCurve::Segment::Segment(DroopCurve& parentCurve, double minV, double k) :
    m_parentCurve(parentCurve),
    m_minV(minV),
    m_k(k) {
}

double DroopCurve::Segment::getK() const {
    return m_k;
}
double DroopCurve::Segment::getMinV() const {
    return m_minV;
}
double DroopCurve::Segment::getMaxV() const {
    auto segments = m_parentCurve.getSegments();
    if(segments.empty()) {
        return stdcxx::nan(); //should not be reached
    }

    for(const auto& segment : segments) {
        if(segment.getMinV() > m_minV) {
            return segment.getMinV();
        }
    }
    return m_parentCurve.getMaxV();
}

DroopCurve::DroopCurve(double maxV) :
    m_maxV(maxV) {
}

void DroopCurve::addSegment(const Segment& segmentToAdd) {
    m_segments.emplace(segmentToAdd);
}

stdcxx::const_range<DroopCurve::Segment> DroopCurve::getSegments() const {
    return m_segments;
}

double DroopCurve::getK(double v) const {
    if (m_segments.empty()){
        return 0.0;
    }

    for (auto segmentIt = m_segments.crbegin(); segmentIt!=m_segments.crend(); segmentIt++ ) {
        if(v >= segmentIt->getMinV()) {
            return segmentIt->getK();
        }
    }

    return m_segments.begin()->getK();
}

double DroopCurve::getMaxV() const {
    return m_maxV;
}

}  // namespace iidm

}  // namespace powsybl
