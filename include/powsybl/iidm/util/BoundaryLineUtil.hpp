/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_BOUNDARYLINEUTIL_HPP
#define POWSYBL_IIDM_UTIL_BOUNDARYLINEUTIL_HPP

namespace powsybl {

namespace iidm {

class BoundaryLine;

namespace BoundaryLineUtil {

    /** We prefer to use P0 and Q0 if the boundary line is not paired and P0 and Q0 are valid, but we cannot retrieve
    * P, Q, angle and voltage at boundary if the boundary line has a generation part: a previous global load flow
    * run is needed, especially if the generation is regulating voltage.
    * This could be improved later.
    */
    bool useHypothesis(const BoundaryLine& boundaryLine);

    bool zeroImpedance(const BoundaryLine& boundaryLine);

    double getBoundaryBusU(const BoundaryLine& boundaryLine);

    double getBoundaryBusTheta(const BoundaryLine& boundaryLine);

    double getV(const BoundaryLine& boundaryLine);

    double getTheta(const BoundaryLine& boundaryLine);

}  // namespace BoundaryLineUtil

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_BOUNDARYLINEUTIL_HPP