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

class DanglingLine;

namespace TieLineUtil {

double getR(const DanglingLine& dl1, const DanglingLine& dl2);
double getX(const DanglingLine& dl1, const DanglingLine& dl2);
double getG1(const DanglingLine& dl1, const DanglingLine& dl2);
double getB1(const DanglingLine& dl1, const DanglingLine& dl2);
double getG2(const DanglingLine& dl1, const DanglingLine& dl2);
double getB2(const DanglingLine& dl1, const DanglingLine& dl2);

double getBoundaryV(const DanglingLine& dl1, const DanglingLine& dl2);
double getBoundaryAngle(const DanglingLine& dl1, const DanglingLine& dl2);

LinkData::BranchAdmittanceMatrix equivalentBranchAdmittanceMatrix(const DanglingLine& dl1, const DanglingLine& dl2);

bool zeroImpedanceLine(const LinkData::BranchAdmittanceMatrix& adm);

std::complex<double> voltageAtBoundaryNode(const DanglingLine& dl1, const DanglingLine& dl2);

stdcxx::CReference<DanglingLine> getPairedDanglingLine(const DanglingLine& dl);
stdcxx::Reference<DanglingLine> getPairedDanglingLine(DanglingLine& dl);

}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_TIELINEUTIL_HPP