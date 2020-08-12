/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TwoWindingsTransformer.hpp>

#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

TwoWindingsTransformer::TwoWindingsTransformer(const std::string& id, const std::string& name, bool fictitious, Substation& substation, double r, double x, double g, double b, double ratedU1, double ratedU2, double ratedS) :
    Branch(id, name, fictitious, ConnectableType::TWO_WINDINGS_TRANSFORMER),
    m_substation(substation),
    m_r(checkR(*this, r)),
    m_x(checkX(*this, x)),
    m_g(checkG(*this, g)),
    m_b(checkB(*this, b)),
    m_ratedU1(checkRatedU1(*this, ratedU1)),
    m_ratedU2(checkRatedU2(*this, ratedU2)),
    m_ratedS(checkRatedS(*this, ratedS)) {
}

void TwoWindingsTransformer::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Branch::allocateVariantArrayElement(indexes, sourceIndex);

    if (m_ratioTapChanger) {
        m_ratioTapChanger->allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_phaseTapChanger) {
        m_phaseTapChanger->allocateVariantArrayElement(indexes, sourceIndex);
    }
}

void TwoWindingsTransformer::deleteVariantArrayElement(unsigned long index) {
    Branch::deleteVariantArrayElement(index);

    if (m_ratioTapChanger) {
        m_ratioTapChanger->deleteVariantArrayElement(index);
    }
    if (m_phaseTapChanger) {
        m_phaseTapChanger->deleteVariantArrayElement(index);
    }
}

void TwoWindingsTransformer::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Branch::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    if (m_ratioTapChanger) {
        m_ratioTapChanger->extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_phaseTapChanger) {
        m_phaseTapChanger->extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
}

double TwoWindingsTransformer::getB() const {
    return m_b;
}

double TwoWindingsTransformer::getG() const {
    return m_g;
}

const Network& TwoWindingsTransformer::getNetwork() const {
    return Branch::getNetwork();
}

Network& TwoWindingsTransformer::getNetwork() {
    return Branch::getNetwork();
}

const PhaseTapChanger& TwoWindingsTransformer::getPhaseTapChanger() const {
    if (!m_phaseTapChanger) {
        throw PowsyblException("Phase tap changer not set");
    }
    return *m_phaseTapChanger;
}

PhaseTapChanger& TwoWindingsTransformer::getPhaseTapChanger() {
    if (!m_phaseTapChanger) {
        throw PowsyblException("Phase tap changer not set");
    }
    return *m_phaseTapChanger;
}

double TwoWindingsTransformer::getR() const {
    return m_r;
}

double TwoWindingsTransformer::getRatedS() const {
    return m_ratedS;
}

double TwoWindingsTransformer::getRatedU1() const {
    return m_ratedU1;
}

double TwoWindingsTransformer::getRatedU2() const {
    return m_ratedU2;
}

const RatioTapChanger& TwoWindingsTransformer::getRatioTapChanger() const {
    if (!m_ratioTapChanger) {
        throw PowsyblException("Ratio tap changer not set");
    }
    return *m_ratioTapChanger;
}

RatioTapChanger& TwoWindingsTransformer::getRatioTapChanger() {
    if (!m_ratioTapChanger) {
        throw PowsyblException("Ratio tap changer not set");
    }
    return *m_ratioTapChanger;
}

stdcxx::CReference<Substation> TwoWindingsTransformer::getSubstation() const {
    return stdcxx::cref<Substation>(m_substation);
}

stdcxx::Reference<Substation> TwoWindingsTransformer::getSubstation() {
    return m_substation;
}

const std::string& TwoWindingsTransformer::getTypeDescription() const {
    static std::string s_typeDescription = "2 windings transformer";

    return s_typeDescription;
}

double TwoWindingsTransformer::getX() const {
    return m_x;
}

bool TwoWindingsTransformer::hasPhaseTapChanger() const {
    return static_cast<bool>(m_phaseTapChanger);
}

bool TwoWindingsTransformer::hasRatioTapChanger() const {
    return static_cast<bool>(m_ratioTapChanger);
}

PhaseTapChangerAdder TwoWindingsTransformer::newPhaseTapChanger() {
    return PhaseTapChangerAdder(*this);
}

RatioTapChangerAdder TwoWindingsTransformer::newRatioTapChanger() {
    return RatioTapChangerAdder(*this);
}

void TwoWindingsTransformer::reduceVariantArraySize(unsigned long number) {
    Branch::reduceVariantArraySize(number);

    if (m_ratioTapChanger) {
        m_ratioTapChanger->reduceVariantArraySize(number);
    }
    if (m_phaseTapChanger) {
        m_phaseTapChanger->reduceVariantArraySize(number);
    }
}

TwoWindingsTransformer& TwoWindingsTransformer::setB(double b) {
    m_b = checkB(*this, b);
    return *this;
}

TwoWindingsTransformer& TwoWindingsTransformer::setG(double g) {
    m_g = checkG(*this, g);
    return *this;
}

void TwoWindingsTransformer::setPhaseTapChanger(std::unique_ptr<PhaseTapChanger> phaseTapChanger) {
    m_phaseTapChanger = std::move(phaseTapChanger);
}

TwoWindingsTransformer& TwoWindingsTransformer::setR(double r) {
    m_r = checkR(*this, r);
    return *this;
}

TwoWindingsTransformer& TwoWindingsTransformer::setRatedS(double ratedS) {
    m_ratedS = checkRatedS(*this, ratedS);
    return *this;
}

TwoWindingsTransformer& TwoWindingsTransformer::setRatedU1(double ratedU1) {
    m_ratedU1 = checkRatedU1(*this, ratedU1);
    return *this;
}

TwoWindingsTransformer& TwoWindingsTransformer::setRatedU2(double ratedU2) {
    m_ratedU2 = checkRatedU2(*this, ratedU2);
    return *this;
}

void TwoWindingsTransformer::setRatioTapChanger(std::unique_ptr<RatioTapChanger> ratioTapChanger) {
    m_ratioTapChanger = std::move(ratioTapChanger);
}

unsigned long TwoWindingsTransformer::getRegulatingTapChangerCount() const {
    unsigned long count = 0;
    count += (hasPhaseTapChanger() && m_phaseTapChanger->isRegulating()) ? 1 : 0;
    count += (hasRatioTapChanger() && m_ratioTapChanger->isRegulating()) ? 1 : 0;

    return count;
}

TwoWindingsTransformer& TwoWindingsTransformer::setX(double x) {
    m_x = checkX(*this, x);
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
