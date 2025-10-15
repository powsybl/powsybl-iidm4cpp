/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/TieLineUtil.hpp>

#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/TieLine.hpp>

namespace powsybl {

namespace iidm {

namespace TieLineUtil {

double getR(const DanglingLine& dl1, const DanglingLine& dl2) {
    LinkData::BranchAdmittanceMatrix adm = equivalentBranchAdmittanceMatrix(dl1, dl2);
    // Add 0.0 to avoid negative zero and check impedance before / 0
    return zeroImpedanceLine(adm) ? 0.0 : (std::complex<double>(-1) / adm.y12).real() + 0.0;
}
double getX(const DanglingLine& dl1, const DanglingLine& dl2) {
    LinkData::BranchAdmittanceMatrix adm = equivalentBranchAdmittanceMatrix(dl1, dl2);
    // Add 0.0 to avoid negative zero and check impedance before / 0
    return zeroImpedanceLine(adm) ? 0.0 : (std::complex<double>(-1) / adm.y12).imag() + 0.0;
}
double getG1(const DanglingLine& dl1, const DanglingLine& dl2) {
    LinkData::BranchAdmittanceMatrix adm = equivalentBranchAdmittanceMatrix(dl1, dl2);
    return (adm.y11 + adm.y12).real();
}
double getB1(const DanglingLine& dl1, const DanglingLine& dl2) {
    LinkData::BranchAdmittanceMatrix adm = equivalentBranchAdmittanceMatrix(dl1, dl2);
    return (adm.y11 + adm.y12).imag();
}
double getG2(const DanglingLine& dl1, const DanglingLine& dl2) {
    LinkData::BranchAdmittanceMatrix adm = equivalentBranchAdmittanceMatrix(dl1, dl2);
    return (adm.y22 + adm.y21).real();
}
double getB2(const DanglingLine& dl1, const DanglingLine& dl2) {
    LinkData::BranchAdmittanceMatrix adm = equivalentBranchAdmittanceMatrix(dl1, dl2);
    return (adm.y22 + adm.y21).imag();
}

LinkData::BranchAdmittanceMatrix equivalentBranchAdmittanceMatrix(const DanglingLine& dl1, const DanglingLine& dl2) {

    LinkData::BranchAdmittanceMatrix adm1 = LinkData::calculateBranchAdmittance(dl1.getR(), dl1.getX(), 1.0, 0.0, 1.0, 0.0,
                                                std::complex<double>(dl1.getG(), dl1.getB()), std::complex<double>(0.0, 0.0));
    LinkData::BranchAdmittanceMatrix adm2 = LinkData::calculateBranchAdmittance(dl2.getR(), dl2.getX(), 1.0, 0.0, 1.0, 0.0,
                                                std::complex<double>(0.0, 0.0), std::complex<double>(dl2.getG(), dl2.getB()));

    if(zeroImpedanceLine(adm2)) {
        return adm1;
    } else if(zeroImpedanceLine(adm1)) {
        return adm2;
    } else {
        return LinkData::kronChain(adm1, TwoSides::TWO, adm2, TwoSides::ONE);
    }
}

bool zeroImpedanceLine(const LinkData::BranchAdmittanceMatrix& adm) {
    if (adm.y12.real() == 0.0 && adm.y12.imag() == 0.0) {
        return true;
    } else {
        return (adm.y21.real() == 0.0 && adm.y22.imag() == 0.0);
    }
}

stdcxx::CReference<DanglingLine> getPairedDanglingLine(const DanglingLine& dl) {
    stdcxx::CReference<TieLine> crefTl = dl.getTieLine();
    if(static_cast<bool>(crefTl)) {
        const TieLine& tl = crefTl.get();
        if(stdcxx::areSame(tl.getDanglingLine1(), dl)) {
            return stdcxx::cref(tl.getDanglingLine2());
        } else {
            return stdcxx::cref(tl.getDanglingLine1());
        }
    }
    return stdcxx::cref<DanglingLine>();
}
stdcxx::Reference<DanglingLine> getPairedDanglingLine(DanglingLine& dl) {
    stdcxx::Reference<TieLine> refTl = dl.getTieLine();
    if(static_cast<bool>(refTl)) {
        TieLine& tl = refTl.get();
        if(stdcxx::areSame(tl.getDanglingLine1(), dl)) {
            return stdcxx::ref(tl.getDanglingLine2());
        } else {
            return stdcxx::ref(tl.getDanglingLine1());
        }
    }
    return stdcxx::ref<DanglingLine>();
}

}  // namespace TieLineUtil

}  // namespace iidm

}  // namespace powsybl

