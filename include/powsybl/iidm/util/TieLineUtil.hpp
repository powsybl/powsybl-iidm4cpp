/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_TIELINEUTIL_HPP
#define POWSYBL_IIDM_UTIL_TIELINEUTIL_HPP

#include <complex>

#include <powsybl/iidm/util/LinkData.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class BoundaryLine;

namespace TieLineUtil {

double getR(const BoundaryLine& bl1, const BoundaryLine& bl2);
double getX(const BoundaryLine& bl1, const BoundaryLine& bl2);
double getG1(const BoundaryLine& bl1, const BoundaryLine& bl2);
double getB1(const BoundaryLine& bl1, const BoundaryLine& bl2);
double getG2(const BoundaryLine& bl1, const BoundaryLine& bl2);
double getB2(const BoundaryLine& bl1, const BoundaryLine& bl2);

double getBoundaryV(const BoundaryLine& bl1, const BoundaryLine& bl2);
double getBoundaryAngle(const BoundaryLine& bl1, const BoundaryLine& bl2);

LinkData::BranchAdmittanceMatrix equivalentBranchAdmittanceMatrix(const BoundaryLine& bl1, const BoundaryLine& bl2);

bool zeroImpedanceLine(const LinkData::BranchAdmittanceMatrix& adm);

std::complex<double> voltageAtBoundaryNode(const BoundaryLine& bl1, const BoundaryLine& bl2);

stdcxx::CReference<BoundaryLine> getPairedBoundaryLine(const BoundaryLine& bl);
stdcxx::Reference<BoundaryLine> getPairedBoundaryLine(BoundaryLine& bl);

}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_TIELINEUTIL_HPP