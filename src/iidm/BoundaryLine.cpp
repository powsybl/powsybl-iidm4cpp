/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/BoundaryLine.hpp>

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/util/BoundaryLineBoundary.hpp>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

BoundaryLine::BoundaryLine(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious,
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
    m_boundary(stdcxx::make_unique<util::boundary_line::Boundary>(*this)),
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

void BoundaryLine::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Injection::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_p0[index] = m_p0[sourceIndex];
        m_q0[index] = m_q0[sourceIndex];
    }

    if (m_generation) {
        m_generation->allocateVariantArrayElement(indexes, sourceIndex);
    }
}

void BoundaryLine::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Injection::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_p0.resize(m_p0.size() + number, m_p0[sourceIndex]);
    m_q0.resize(m_q0.size() + number, m_q0[sourceIndex]);

    if (m_generation) {
        m_generation->extendVariantArraySize(number, sourceIndex);
    }
}

double BoundaryLine::getB() const {
    return m_b;
}

const Boundary& BoundaryLine::getBoundary() const {
    return *m_boundary;
}

Boundary& BoundaryLine::getBoundary() {
    return *m_boundary;
}

stdcxx::CReference<TieLine> BoundaryLine::getTieLine() const {
    return stdcxx::cref<TieLine>(m_tieLine);
}
stdcxx::Reference<TieLine> BoundaryLine::getTieLine() {
    return m_tieLine;
}

BoundaryLine& BoundaryLine::setTieLine(TieLine& tieLine) {
    m_tieLine = tieLine;
    return *this;
}

void BoundaryLine::remove() {
    if(static_cast<bool>(m_tieLine)) {
        throw PowsyblException(stdcxx::format("Parent tie line %1% should be removed before the child boundary line", m_tieLine.get().getId()));
    }
    if(m_boundary) {
        m_boundary->notifyRemoval();
    }
    Injection::remove();
}
void BoundaryLine::removeTieLine(){
    m_tieLine.reset();
}

double BoundaryLine::getG() const {
    return m_g;
}

stdcxx::CReference<BoundaryLine::Generation> BoundaryLine::getGeneration() const {
    return stdcxx::cref<Generation>(m_generation);
}

stdcxx::Reference<BoundaryLine::Generation> BoundaryLine::getGeneration() {
    return stdcxx::ref<Generation>(m_generation);
}

bool BoundaryLine::isPaired() const {
    return static_cast<bool>(m_tieLine);
}

double BoundaryLine::getP0() const {
    return m_p0.at(getNetwork().getVariantIndex());
}

double BoundaryLine::getQ0() const {
    return m_q0.at(getNetwork().getVariantIndex());
}

double BoundaryLine::getR() const {
    return m_r;
}

const IdentifiableType& BoundaryLine::getType() const {
    static IdentifiableType s_type = IdentifiableType::BOUNDARY_LINE;
    return s_type;
}

const std::string& BoundaryLine::getTypeDescription() const {
    static std::string s_typeDescription = "Boundary line";

    return s_typeDescription;
}

const std::string& BoundaryLine::getPairingKey() const {
    return m_pairingKey;
}

double BoundaryLine::getX() const {
    return m_x;
}

void BoundaryLine::reduceVariantArraySize(unsigned long number) {
    Injection::reduceVariantArraySize(number);

    m_p0.resize(m_p0.size() - number);
    m_q0.resize(m_q0.size() - number);

    if (m_generation) {
        m_generation->reduceVariantArraySize(number);
    }
}

BoundaryLine& BoundaryLine::setB(double b) {
    m_b = checkB(*this, b);

    return *this;
}

BoundaryLine& BoundaryLine::setG(double g) {
    m_g = checkG(*this, g);

    return *this;
}

BoundaryLine& BoundaryLine::setP0(double p0) {
    checkP0(*this, p0, getNetwork().getMinimumValidationLevel());

    m_p0[getNetwork().getVariantIndex()] = p0;

    getNetwork().invalidateValidationLevel();

    return *this;
}

BoundaryLine& BoundaryLine::setQ0(double q0) {
    checkQ0(*this, q0, getNetwork().getMinimumValidationLevel());

    m_q0[getNetwork().getVariantIndex()] = q0;

    getNetwork().invalidateValidationLevel();

    return *this;
}

BoundaryLine& BoundaryLine::setR(double r) {
    m_r = checkR(*this, r);

    return *this;
}

BoundaryLine& BoundaryLine::setX(double x) {
    m_x = checkX(*this, x);

    return *this;
}

BoundaryLine& BoundaryLine::setPairingKey(const std::string& pairingKey) {
    if(isPaired()) {
        throw ValidationException(*this, "pairing key cannot be set if boundary line is paired.");
    }
    m_pairingKey = pairingKey;

    return *this;
}

stdcxx::const_range<OperationalLimitsGroup> BoundaryLine::getOperationalLimitsGroups() const {
    return m_operationalLimitsGroups.getOperationalLimitsGroups();
}
stdcxx::range<OperationalLimitsGroup> BoundaryLine::getOperationalLimitsGroups() {
    return m_operationalLimitsGroups.getOperationalLimitsGroups();
}
const stdcxx::optional<std::string>& BoundaryLine::getSelectedOperationalLimitsGroupId() const {
    return m_operationalLimitsGroups.getSelectedOperationalLimitsGroupId();
}
stdcxx::CReference<OperationalLimitsGroup> BoundaryLine::getOperationalLimitsGroup(const std::string& id) const {
    return m_operationalLimitsGroups.getOperationalLimitsGroup(id);
}
stdcxx::Reference<OperationalLimitsGroup> BoundaryLine::getOperationalLimitsGroup(const std::string& id) {
    return m_operationalLimitsGroups.getOperationalLimitsGroup(id);
}
stdcxx::CReference<OperationalLimitsGroup> BoundaryLine::getSelectedOperationalLimitsGroup() const {
    return m_operationalLimitsGroups.getSelectedOperationalLimitsGroup();
}
stdcxx::Reference<OperationalLimitsGroup> BoundaryLine::getSelectedOperationalLimitsGroup() {
    return m_operationalLimitsGroups.getSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& BoundaryLine::newOperationalLimitsGroup(const std::string& id) {
    return m_operationalLimitsGroups.newOperationalLimitsGroup(id);
}
void BoundaryLine::setSelectedOperationalLimitsGroup(const std::string& id) {
    m_operationalLimitsGroups.setSelectedOperationalLimitsGroup(id);
}
void BoundaryLine::removeOperationalLimitsGroup(const std::string& id) {
    m_operationalLimitsGroups.removeOperationalLimitsGroup(id);
}
void BoundaryLine::cancelSelectedOperationalLimitsGroup() {
    m_operationalLimitsGroups.cancelSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& BoundaryLine::getOrCreateSelectedOperationalLimitsGroup() {
    return m_operationalLimitsGroups.getOrCreateSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& BoundaryLine::getOrCreateSelectedOperationalLimitsGroup(const std::string& id) {
    return FlowsLimitsHolder::getOrCreateSelectedOperationalLimitsGroup(id);
}

ActivePowerLimitsAdder BoundaryLine::newActivePowerLimits() {
    return getOrCreateSelectedOperationalLimitsGroup().newActivePowerLimits();
}
ApparentPowerLimitsAdder BoundaryLine::newApparentPowerLimits() {
    return getOrCreateSelectedOperationalLimitsGroup().newApparentPowerLimits();
}
CurrentLimitsAdder BoundaryLine::newCurrentLimits() {
    return getOrCreateSelectedOperationalLimitsGroup().newCurrentLimits();
}

void BoundaryLine::applySolvedValues() {
    setGenerationTargetPToP();
    setGenerationTargetQToQ();
    setGenerationTargetVToV();
}
void BoundaryLine::setGenerationTargetPToP() {
    if(m_generation) {
        double terminalP = getTerminal().getP();
        if(!std::isnan(terminalP)) {
            m_generation->setTargetP(-terminalP);
        }
    }
}
void BoundaryLine::setGenerationTargetQToQ() {
    if(m_generation) {
        double terminalQ = getTerminal().getQ();
        if(!std::isnan(terminalQ)) {
            m_generation->setTargetQ(-terminalQ);
        }
    }
}
void BoundaryLine::setGenerationTargetVToV() {
    if(m_generation) {
        auto bus = getTerminal().getBusView().getBus();
        if(static_cast<bool>(bus) && !std::isnan(bus.get().getV())) {
            m_generation->setTargetV(bus.get().getV());
        }
    }
}

}  // namespace iidm

}  // namespace powsybl
