/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/TieLineUtil.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/TieLine.hpp>

#include <powsybl/iidm/util/BoundaryLineUtil.hpp>

namespace powsybl {

namespace iidm {

namespace TieLineUtil {

double getR(const BoundaryLine& bl1, const BoundaryLine& bl2) {
    LinkData::BranchAdmittanceMatrix adm = equivalentBranchAdmittanceMatrix(bl1, bl2);
    // Add 0.0 to avoid negative zero and check impedance before / 0
    return zeroImpedanceLine(adm) ? 0.0 : (std::complex<double>(-1) / adm.y12).real() + 0.0;
}
double getX(const BoundaryLine& bl1, const BoundaryLine& bl2) {
    LinkData::BranchAdmittanceMatrix adm = equivalentBranchAdmittanceMatrix(bl1, bl2);
    // Add 0.0 to avoid negative zero and check impedance before / 0
    return zeroImpedanceLine(adm) ? 0.0 : (std::complex<double>(-1) / adm.y12).imag() + 0.0;
}
double getG1(const BoundaryLine& bl1, const BoundaryLine& bl2) {
    LinkData::BranchAdmittanceMatrix adm = equivalentBranchAdmittanceMatrix(bl1, bl2);
    return (adm.y11 + adm.y12).real();
}
double getB1(const BoundaryLine& bl1, const BoundaryLine& bl2) {
    LinkData::BranchAdmittanceMatrix adm = equivalentBranchAdmittanceMatrix(bl1, bl2);
    return (adm.y11 + adm.y12).imag();
}
double getG2(const BoundaryLine& bl1, const BoundaryLine& bl2) {
    LinkData::BranchAdmittanceMatrix adm = equivalentBranchAdmittanceMatrix(bl1, bl2);
    return (adm.y22 + adm.y21).real();
}
double getB2(const BoundaryLine& bl1, const BoundaryLine& bl2) {
    LinkData::BranchAdmittanceMatrix adm = equivalentBranchAdmittanceMatrix(bl1, bl2);
    return (adm.y22 + adm.y21).imag();
}

double getBoundaryV(const BoundaryLine& bl1, const BoundaryLine& bl2) {
    std::complex<double> boundaryV = voltageAtBoundaryNode(bl1, bl2);
    return std::abs(boundaryV);
}
double getBoundaryAngle(const BoundaryLine& bl1, const BoundaryLine& bl2) {
    std::complex<double> boundaryV = voltageAtBoundaryNode(bl1, bl2);
    return std::atan2(boundaryV.imag(),boundaryV.real()) * stdcxx::toDegrees;
}

LinkData::BranchAdmittanceMatrix equivalentBranchAdmittanceMatrix(const BoundaryLine& bl1, const BoundaryLine& bl2) {

    LinkData::BranchAdmittanceMatrix adm1 = LinkData::calculateBranchAdmittance(bl1.getR(), bl1.getX(), 1.0, 0.0, 1.0, 0.0,
                                                std::complex<double>(bl1.getG(), bl1.getB()), std::complex<double>(0.0, 0.0));
    LinkData::BranchAdmittanceMatrix adm2 = LinkData::calculateBranchAdmittance(bl2.getR(), bl2.getX(), 1.0, 0.0, 1.0, 0.0,
                                                std::complex<double>(0.0, 0.0), std::complex<double>(bl2.getG(), bl2.getB()));

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

std::complex<double> voltageAtBoundaryNode(const BoundaryLine& bl1, const BoundaryLine& bl2) {
    double v1 = BoundaryLineUtil::getV(bl1);
    double t1 = BoundaryLineUtil::getTheta(bl1);
    double v2 = BoundaryLineUtil::getV(bl2);
    double t2 = BoundaryLineUtil::getTheta(bl2);
    std::complex<double> c1 = std::polar(v1, t1);
    std::complex<double> c2 = std::polar(v2, t2);

    LinkData::BranchAdmittanceMatrix adm1 = LinkData::calculateBranchAdmittance(bl1.getR(), bl1.getX(), 1.0, 0.0, 1.0, 0.0,
                std::complex<double>(bl1.getG(), bl1.getB()), std::complex<double>(0.0, 0.0));
    LinkData::BranchAdmittanceMatrix adm2 = LinkData::calculateBranchAdmittance(bl2.getR(), bl2.getX(), 1.0, 0.0, 1.0, 0.0,
                std::complex<double>(0.0, 0.0), std::complex<double>(bl2.getG(), bl2.getB()));

    if(zeroImpedanceLine(adm1)) {
        return c1;
    } else if(zeroImpedanceLine(adm2)) {
        return c2;
    } else {
        return -(adm1.y21 * c1 + adm2.y12 * c2 ) / (adm1.y22 + adm2.y11);
    }
}

stdcxx::CReference<BoundaryLine> getPairedBoundaryLine(const BoundaryLine& bl) {
    stdcxx::CReference<TieLine> crefTl = bl.getTieLine();
    if(static_cast<bool>(crefTl)) {
        const TieLine& tl = crefTl.get();
        if(stdcxx::areSame(tl.getBoundaryLine1(), bl)) {
            return stdcxx::cref(tl.getBoundaryLine2());
        } else {
            return stdcxx::cref(tl.getBoundaryLine1());
        }
    }
    return stdcxx::cref<BoundaryLine>();
}
stdcxx::Reference<BoundaryLine> getPairedBoundaryLine(BoundaryLine& bl) {
    stdcxx::Reference<TieLine> refTl = bl.getTieLine();
    if(static_cast<bool>(refTl)) {
        TieLine& tl = refTl.get();
        if(stdcxx::areSame(tl.getBoundaryLine1(), bl)) {
            return stdcxx::ref(tl.getBoundaryLine2());
        } else {
            return stdcxx::ref(tl.getBoundaryLine1());
        }
    }
    return stdcxx::ref<BoundaryLine>();
}

}  // namespace TieLineUtil

}  // namespace iidm

}  // namespace powsybl

