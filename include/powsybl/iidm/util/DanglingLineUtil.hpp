/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_DANGLINGLINEUTIL_HPP
#define POWSYBL_IIDM_UTIL_DANGLINGLINEUTIL_HPP

namespace powsybl {

namespace iidm {

class DanglingLine;

namespace DanglingLineUtil {

    /** We prefer to use P0 and Q0 if the dangling line is not paired and P0 and Q0 are valid, but we cannot retrieve
    * P, Q, angle and voltage at boundary if the dangling line has a generation part: a previous global load flow
    * run is needed, especially if the generation is regulating voltage.
    * This could be improved later.
    */
    bool useHypothesis(const DanglingLine& danglingLine);

    bool zeroImpedance(const DanglingLine& danglingLine);

    double getBoundaryBusU(const DanglingLine& danglingLine);

    double getBoundaryBusTheta(const DanglingLine& danglingLine);

    double getV(const DanglingLine& danglingLine);

    double getTheta(const DanglingLine& danglingLine);

}  // namespace DanglingLineUtil

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_DANGLINGLINEUTIL_HPP