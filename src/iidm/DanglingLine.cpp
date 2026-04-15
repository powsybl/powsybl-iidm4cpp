/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DanglingLine.hpp>

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/util/DanglingLineBoundary.hpp>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

DanglingLine::DanglingLine(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious,
                           double p0, double q0, double r, double x, double g, double b, const std::string& pairingKey,
                           std::unique_ptr<Generation>&& generation) :
    Identifiable(id, name, fictitious),
    m_b(checkB(*this, b)),
    m_g(checkG(*this, g)),
    m_r(checkR(*this, r)),
    m_x(checkX(*this, x)),
    m_p0(network.getVariantManager().getVariantArraySize(), p0),
    m_q0(network.getVariantManager().getVariantArraySize(), q0),
    m_pairingKey(pairingKey),
    m_generation(std::move(generation)),
    m_boundary(stdcxx::make_unique<util::dangling_line::Boundary>(*this)),
    m_operationalLimitsGroups(*this, "limits") {

    if (m_generation) {
        m_generation->attach(*this);
    }
    ValidationLevel vl = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    if (stdcxx::isInstanceOf<Network>(network)) {
        auto& n = dynamic_cast<Network&>(network);
        vl = n.getMinimumValidationLevel();
    }
    checkP0(*this, p0, vl);
    checkQ0(*this, q0, vl);

    m_tieLine.reset();
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

double DanglingLine::getB() const {
    return m_b;
}

const Boundary& DanglingLine::getBoundary() const {
    return *m_boundary;
}

Boundary& DanglingLine::getBoundary() {
    return *m_boundary;
}

stdcxx::CReference<TieLine> DanglingLine::getTieLine() const {
    return stdcxx::cref<TieLine>(m_tieLine);
}
stdcxx::Reference<TieLine> DanglingLine::getTieLine() {
    return m_tieLine;
}

DanglingLine& DanglingLine::setTieLine(TieLine& tieLine) {
    m_tieLine = tieLine;
    return *this;
}

void DanglingLine::remove() {
    if(static_cast<bool>(m_tieLine)) {
        throw PowsyblException(stdcxx::format("Parent tie line %1% should be removed before the child dangling line", m_tieLine.get().getId()));
    }
    if(m_boundary) {
        m_boundary->notifyRemoval();
    }
    Injection::remove();
}
void DanglingLine::removeTieLine(){
    m_tieLine.reset();
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

bool DanglingLine::isPaired() const {
    return static_cast<bool>(m_tieLine);
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

const IdentifiableType& DanglingLine::getType() const {
    static IdentifiableType s_type = IdentifiableType::DANGLING_LINE;
    return s_type;
}

const std::string& DanglingLine::getTypeDescription() const {
    static std::string s_typeDescription = "Dangling line";

    return s_typeDescription;
}

const std::string& DanglingLine::getPairingKey() const {
    return m_pairingKey;
}

double DanglingLine::getX() const {
    return m_x;
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
    checkP0(*this, p0, getNetwork().getMinimumValidationLevel());

    m_p0[getNetwork().getVariantIndex()] = p0;

    getNetwork().invalidateValidationLevel();

    return *this;
}

DanglingLine& DanglingLine::setQ0(double q0) {
    checkQ0(*this, q0, getNetwork().getMinimumValidationLevel());

    m_q0[getNetwork().getVariantIndex()] = q0;

    getNetwork().invalidateValidationLevel();

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

DanglingLine& DanglingLine::setPairingKey(const std::string& pairingKey) {
    if(isPaired()) {
        throw ValidationException(*this, "pairing key cannot be set if dangling line is paired.");
    }
    m_pairingKey = pairingKey;

    return *this;
}

stdcxx::const_range<OperationalLimitsGroup> DanglingLine::getOperationalLimitsGroups() const {
    return m_operationalLimitsGroups.getOperationalLimitsGroups();
}
stdcxx::range<OperationalLimitsGroup> DanglingLine::getOperationalLimitsGroups() {
    return m_operationalLimitsGroups.getOperationalLimitsGroups();
}
const stdcxx::optional<std::string>& DanglingLine::getSelectedOperationalLimitsGroupId() const {
    return m_operationalLimitsGroups.getSelectedOperationalLimitsGroupId();
}
stdcxx::CReference<OperationalLimitsGroup> DanglingLine::getOperationalLimitsGroup(const std::string& id) const {
    return m_operationalLimitsGroups.getOperationalLimitsGroup(id);
}
stdcxx::Reference<OperationalLimitsGroup> DanglingLine::getOperationalLimitsGroup(const std::string& id) {
    return m_operationalLimitsGroups.getOperationalLimitsGroup(id);
}
stdcxx::CReference<OperationalLimitsGroup> DanglingLine::getSelectedOperationalLimitsGroup() const {
    return m_operationalLimitsGroups.getSelectedOperationalLimitsGroup();
}
stdcxx::Reference<OperationalLimitsGroup> DanglingLine::getSelectedOperationalLimitsGroup() {
    return m_operationalLimitsGroups.getSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& DanglingLine::newOperationalLimitsGroup(const std::string& id) {
    return m_operationalLimitsGroups.newOperationalLimitsGroup(id);
}
void DanglingLine::setSelectedOperationalLimitsGroup(const std::string& id) {
    m_operationalLimitsGroups.setSelectedOperationalLimitsGroup(id);
}
void DanglingLine::removeOperationalLimitsGroup(const std::string& id) {
    m_operationalLimitsGroups.removeOperationalLimitsGroup(id);
}
void DanglingLine::cancelSelectedOperationalLimitsGroup() {
    m_operationalLimitsGroups.cancelSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& DanglingLine::getOrCreateSelectedOperationalLimitsGroup() {
    return m_operationalLimitsGroups.getOrCreateSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& DanglingLine::getOrCreateSelectedOperationalLimitsGroup(const std::string& id) {
    return FlowsLimitsHolder::getOrCreateSelectedOperationalLimitsGroup(id);
}

ActivePowerLimitsAdder DanglingLine::newActivePowerLimits() {
    return getOrCreateSelectedOperationalLimitsGroup().newActivePowerLimits();
}
ApparentPowerLimitsAdder DanglingLine::newApparentPowerLimits() {
    return getOrCreateSelectedOperationalLimitsGroup().newApparentPowerLimits();
}
CurrentLimitsAdder DanglingLine::newCurrentLimits() {
    return getOrCreateSelectedOperationalLimitsGroup().newCurrentLimits();
}

void DanglingLine::applySolvedValues() {
    setGenerationTargetPToP();
    setGenerationTargetQToQ();
    setGenerationTargetVToV();
}
void DanglingLine::setGenerationTargetPToP() {
    if(m_generation) {
        double terminalP = getTerminal().getP();
        if(!std::isnan(terminalP)) {
            m_generation->setTargetP(-terminalP);
        }
    }
}
void DanglingLine::setGenerationTargetQToQ() {
    if(m_generation) {
        double terminalQ = getTerminal().getQ();
        if(!std::isnan(terminalQ)) {
            m_generation->setTargetQ(-terminalQ);
        }
    }
}
void DanglingLine::setGenerationTargetVToV() {
    if(m_generation) {
        auto bus = getTerminal().getBusView().getBus();
        if(static_cast<bool>(bus) && !std::isnan(bus.get().getV())) {
            m_generation->setTargetV(bus.get().getV());
        }
    }
}

}  // namespace iidm

}  // namespace powsybl
