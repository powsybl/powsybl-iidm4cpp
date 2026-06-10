/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DroopCurveAdder.hpp>

#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/iidm/ValidationException.hpp>

namespace powsybl {

namespace iidm {

DroopCurveAdder::DroopCurveAdder(AcDcConverter& owner) :
    m_owner(owner) {
}

DroopCurveAdder& DroopCurveAdder::addSegment(double minV, double k) {
    if (std::isnan(k)) {
        throw ValidationException(m_owner, "Droop curve's segment : k is not set");
    }
    if (std::isnan(minV)) {
        throw ValidationException(m_owner, "Droop curve's segment : min V is not set");
    }
    m_segmentsToAdd.emplace(std::make_pair(minV, k));
    return *this;
}


DroopCurveAdder& DroopCurveAdder::setMaxV(double maxV) {
    m_maxV = maxV;
    return *this;
}

DroopCurve& DroopCurveAdder::add() {
    if(!m_segmentsToAdd.empty()) {
        if (std::isnan(m_maxV)) {
            throw ValidationException(m_owner, "Droop curve : max V is not set");
        }
        if(m_maxV < m_segmentsToAdd.rbegin()->first) {
            throw ValidationException(m_owner, stdcxx::format("Droop curve : max V (%1%) must be >= last segment's min V (%2%)", m_maxV, m_segmentsToAdd.rbegin()->first));
        }
    }

    std::unique_ptr<DroopCurve> ptrDroopCurve = std::unique_ptr<DroopCurve>(new DroopCurve(m_maxV));
    auto& curve = m_owner.setDroopCurve(std::move(ptrDroopCurve));

    for (const auto& pair : m_segmentsToAdd) {
        curve.addSegment(DroopCurve::Segment(curve, pair.first, pair.second));
    }

    return curve;
}



}  // namespace iidm

}  // namespace powsybl