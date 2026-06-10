/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DROOPCURVEADDER_HPP
#define POWSYBL_IIDM_DROOPCURVEADDER_HPP

#include <powsybl/iidm/DroopCurve.hpp>

#include <map>

namespace powsybl {

namespace iidm {

class AcDcConverter;

class DroopCurveAdder {
public:
    ~DroopCurveAdder() noexcept = default;

    DroopCurve& add();

    DroopCurveAdder& addSegment(double minV, double k);

    DroopCurveAdder& setMaxV(double maxV);

private:
    explicit DroopCurveAdder(AcDcConverter& owner);
    friend class AcDcConverter;

private:
    AcDcConverter& m_owner;

    double m_maxV = stdcxx::nan();

    std::map<double, double> m_segmentsToAdd;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DROOPCURVEADDER_HPP