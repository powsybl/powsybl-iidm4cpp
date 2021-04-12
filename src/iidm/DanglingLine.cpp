/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DanglingLine.hpp>

#include <powsybl/iidm/ActivePowerLimits.hpp>
#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimits.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/OperationalLimitsHolder.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VariantManager.hpp>

#include "DanglingLineBoundary.hpp"

namespace powsybl {

namespace iidm {

DanglingLine::DanglingLine(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious,
                           double p0, double q0, double r, double x, double g, double b, const std::string& ucteXnodeCode,
                           std::unique_ptr<Generation>&& generation) :
    Injection(id, name, fictitious, ConnectableType::DANGLING_LINE),
    m_b(checkB(*this, b)),
    m_g(checkG(*this, g)),
    m_r(checkR(*this, r)),
    m_x(checkX(*this, x)),
    m_p0(network.getVariantManager().getVariantArraySize(), checkP0(*this, p0)),
    m_q0(network.getVariantManager().getVariantArraySize(), checkQ0(*this, q0)),
    m_ucteXnodeCode(ucteXnodeCode),
    m_generation(std::move(generation)),
    m_operationalLimitsHolder(*this, "limits"),
    m_boundary(stdcxx::make_unique<dangling_line::Boundary>(*this)) {

    if (m_generation) {
        m_generation->attach(*this);
    }
}

void DanglingLine::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Injection::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_p0[index] = m_p0[sourceIndex];
        m_q0[index] = m_q0[sourceIndex];
    }

    if (m_generation) {
        m_generation->allocateVariantArrayElement(indexes, sourceIndex);
    }
}

void DanglingLine::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Injection::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_p0.resize(m_p0.size() + number, m_p0[sourceIndex]);
    m_q0.resize(m_q0.size() + number, m_q0[sourceIndex]);

    if (m_generation) {
        m_generation->extendVariantArraySize(number, sourceIndex);
    }
}

stdcxx::CReference<ActivePowerLimits> DanglingLine::getActivePowerLimits() const {
    return m_operationalLimitsHolder.getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}

stdcxx::Reference<ActivePowerLimits> DanglingLine::getActivePowerLimits() {
    return m_operationalLimitsHolder.getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}

stdcxx::CReference<ApparentPowerLimits> DanglingLine::getApparentPowerLimits() const {
    return m_operationalLimitsHolder.getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}

stdcxx::Reference<ApparentPowerLimits> DanglingLine::getApparentPowerLimits() {
    return m_operationalLimitsHolder.getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}

double DanglingLine::getB() const {
    return m_b;
}

const Boundary& DanglingLine::getBoundary() const {
    return *m_boundary;
}

Boundary& DanglingLine::getBoundary() {
    return *m_boundary;
}

stdcxx::CReference<CurrentLimits> DanglingLine::getCurrentLimits() const {
    return m_operationalLimitsHolder.getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}

stdcxx::Reference<CurrentLimits> DanglingLine::getCurrentLimits() {
    return m_operationalLimitsHolder.getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}

double DanglingLine::getG() const {
    return m_g;
}

stdcxx::CReference<DanglingLine::Generation> DanglingLine::getGeneration() const {
    return stdcxx::cref<Generation>(m_generation);
}

stdcxx::Reference<DanglingLine::Generation> DanglingLine::getGeneration() {
    return stdcxx::ref<Generation>(m_generation);
}

stdcxx::const_range<OperationalLimits> DanglingLine::getOperationalLimits() const {
    return m_operationalLimitsHolder.getOperationalLimits();
}

stdcxx::range<OperationalLimits> DanglingLine::getOperationalLimits() {
    return m_operationalLimitsHolder.getOperationalLimits();
}

double DanglingLine::getP0() const {
    return m_p0.at(getNetwork().getVariantIndex());
}

double DanglingLine::getQ0() const {
    return m_q0.at(getNetwork().getVariantIndex());
}

double DanglingLine::getR() const {
    return m_r;
}

const std::string& DanglingLine::getTypeDescription() const {
    static std::string s_typeDescription = "Dangling line";

    return s_typeDescription;
}

const std::string& DanglingLine::getUcteXnodeCode() const {
    return m_ucteXnodeCode;
}

double DanglingLine::getX() const {
    return m_x;
}

ActivePowerLimitsAdder DanglingLine::newActivePowerLimits() {
    return m_operationalLimitsHolder.newActivePowerLimits();
}

ApparentPowerLimitsAdder DanglingLine::newApparentPowerLimits() {
    return m_operationalLimitsHolder.newApparentPowerLimits();
}

CurrentLimitsAdder DanglingLine::newCurrentLimits() {
    return m_operationalLimitsHolder.newCurrentLimits();
}

void DanglingLine::reduceVariantArraySize(unsigned long number) {
    Injection::reduceVariantArraySize(number);

    m_p0.resize(m_p0.size() - number);
    m_q0.resize(m_q0.size() - number);

    if (m_generation) {
        m_generation->reduceVariantArraySize(number);
    }
}

DanglingLine& DanglingLine::setB(double b) {
    m_b = checkB(*this, b);

    return *this;
}

DanglingLine& DanglingLine::setG(double g) {
    m_g = checkG(*this, g);

    return *this;
}

DanglingLine& DanglingLine::setP0(double p0) {
    m_p0[getNetwork().getVariantIndex()] = checkP0(*this, p0);

    return *this;
}

DanglingLine& DanglingLine::setQ0(double q0) {
    m_q0[getNetwork().getVariantIndex()] = checkQ0(*this, q0);

    return *this;
}

DanglingLine& DanglingLine::setR(double r) {
    m_r = checkR(*this, r);

    return *this;
}

DanglingLine& DanglingLine::setX(double x) {
    m_x = checkX(*this, x);

    return *this;
}

}  // namespace iidm

}  // namespace powsybl
