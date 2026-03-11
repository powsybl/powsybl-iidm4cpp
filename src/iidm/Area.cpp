/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Area.hpp>

#include <powsybl/iidm/AreaBoundary.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Boundary.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/indirected.hpp>

#include <powsybl/stdcxx/Predicate.hpp>

namespace powsybl {

namespace iidm {

Area::Area(Network& rootNetwork, Network& subNetwork, const std::string& id, const std::string& name, bool fictitious, const std::string& areaType, double interchangeTarget) :
    Area(rootNetwork, id, name, fictitious, areaType, interchangeTarget) {
    setSubNetworkRef(stdcxx::ref<Network>(subNetwork));
}

Area::Area(Network& network, const std::string& id, const std::string& name, bool fictitious, const std::string& areaType, double interchangeTarget) :
    Identifiable(id, name, fictitious),
    m_rootNetwork(network),
    m_areaType(areaType),
    m_interchangeTarget(network.getVariantManager().getVariantArraySize(), interchangeTarget) {
}

const Network& Area::getNetwork() const {
    return m_rootNetwork.get();
}

Network& Area::getNetwork() {
    return m_rootNetwork.get();
}

const Network& Area::getParentNetwork() const {
    if(static_cast<bool>(m_subNetwork)) {
        return m_subNetwork.get();
    }
    return getNetwork();
}

Network& Area::getParentNetwork() {
    if(static_cast<bool>(m_subNetwork)) {
        return m_subNetwork.get();
    }
    return getNetwork();
}

const IdentifiableType& Area::getType() const {
    static IdentifiableType s_type = IdentifiableType::AREA;
    return s_type;
}
const std::string& Area::getTypeDescription() const {
    static std::string s_typeDescription = "Area";
    return s_typeDescription;
}

void Area::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Identifiable::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_interchangeTarget[index] = m_interchangeTarget[sourceIndex];
    }
}

void Area::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Identifiable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_interchangeTarget.resize(m_interchangeTarget.size() + number, m_interchangeTarget[sourceIndex]);
}

void Area::reduceVariantArraySize(unsigned long number) {
    Identifiable::reduceVariantArraySize(number);

    m_interchangeTarget.resize(m_interchangeTarget.size() - number);
}

void Area::setNetworkRef(Network& network) {
    m_rootNetwork.set(network);
}

stdcxx::Reference<Network>& Area::getSubNetworkRef() {
    return m_subNetwork;
}
void Area::setSubNetworkRef(const stdcxx::Reference<Network>& subNetwork) {
    m_subNetwork = subNetwork;
}



const std::string& Area::getAreaType() const {
    return m_areaType;
}

stdcxx::const_range<VoltageLevel> Area::getVoltageLevels() const {
    return m_voltageLevels;
}
stdcxx::range<VoltageLevel> Area::getVoltageLevels() {
    return m_voltageLevels;
}

double Area::getInterchangeTarget() const {
    return m_interchangeTarget.at(getNetwork().getVariantIndex());
} 
Area& Area::setInterchangeTarget(double interchangeTarget) {
    m_interchangeTarget[getNetwork().getVariantIndex()] = interchangeTarget;
    return *this;
}

double Area::getAcInterchange() const {
    return getInterchange([](const AreaBoundary& areaBoundary) {
        return areaBoundary.isAc();
    });
}
double Area::getDcInterchange() const {
    return getInterchange([](const AreaBoundary& areaBoundary) {
        return !areaBoundary.isAc();
    });
}
double Area::getInterchange() const {
    return getInterchange([](const AreaBoundary& ) {
        return true;
    });
}
double Area::getInterchange(const stdcxx::Predicate<AreaBoundary>& predicate) const {
    double sum = 0.0;
    auto areaBoundaries = m_areaBoundaries | boost::adaptors::indirected | boost::adaptors::filtered(predicate);
    for (const auto& areaBoundary : areaBoundaries) {
        double value = areaBoundary.getP();
        if(!std::isnan(value)) {
            sum += value;
        }
    }
    return sum;
}

Area& Area::addVoltageLevel(VoltageLevel& voltageLevel) {
    auto it = std::find_if(m_voltageLevels.begin(), m_voltageLevels.end(), [&voltageLevel](const std::reference_wrapper<VoltageLevel>& vl) {
        return stdcxx::areSame(voltageLevel, vl.get());
    });
    //Add given voltage level only if not already added before
    if (it == m_voltageLevels.end()) {
        m_voltageLevels.emplace_back(std::ref(voltageLevel));
        voltageLevel.addArea(*this);
    }
    return *this;
}
Area& Area::removeVoltageLevel(VoltageLevel& voltageLevel) {
    //We ensure unicity in addVoltageLevel() but we try to remove several just in case
    const auto removeIt = std::remove_if(m_voltageLevels.begin(), m_voltageLevels.end(), [&voltageLevel](const std::reference_wrapper<VoltageLevel>& vl) {
        return stdcxx::areSame(voltageLevel, vl.get());
    });
    if(removeIt != m_voltageLevels.end()) { //at least one element to erase
        m_voltageLevels.erase(removeIt, m_voltageLevels.end());
        voltageLevel.removeArea(*this);
    }
    return *this;
}

AreaBoundaryAdder Area::newAreaBoundary() {
    return AreaBoundaryAdder(*this);
}

Area& Area::addAreaBoundary(const std::shared_ptr<AreaBoundary>& areaBoundary) {
    if(!areaBoundary) {
        return *this;
    }

    auto terminalRef = areaBoundary->getTerminal();
    auto boundaryRef = areaBoundary->getBoundary();
    if (static_cast<bool>(boundaryRef)) {
        checkBoundaryNetwork(boundaryRef.get().getDanglingLine().getParentNetwork(), stdcxx::format("Boundary of DanglingLine %1%", boundaryRef.get().getDanglingLine().getId()));
        boundaryRef.get().registerReferrer(*this);
    }
    if (static_cast<bool>(terminalRef) && static_cast<bool>(terminalRef.get().getConnectable())) {
        checkBoundaryNetwork(terminalRef.get().getConnectable().get().getParentNetwork(), stdcxx::format("Terminal of Connectable %1%", terminalRef.get().getConnectable().get().getId()));
        terminalRef.get().registerReferrer(*this);
    }

    m_areaBoundaries.push_back(areaBoundary);
    return *this;
}
void Area::checkBoundaryNetwork(const Network& network, const std::string& boundaryTypeAndId) {
    if(!stdcxx::areSame(network, getParentNetwork())) {
        throw PowsyblException(stdcxx::format("%1% cannot be added to Area %2% boundaries. It does not belong to the same network or subnetwork.", boundaryTypeAndId, getId()));
    }
}

Area& Area::removeAreaBoundary(Terminal& terminal) {
    return removeAreaBoundary(terminal, true);
}
Area& Area::removeAreaBoundary(Boundary& boundary) {
    return removeAreaBoundary(boundary, true);
}
Area& Area::removeAreaBoundary(Terminal& terminal, bool updateReferrer) {
    if(updateReferrer){
        terminal.unregisterReferrer(*this);
    }
    m_areaBoundaries.erase(std::remove_if(m_areaBoundaries.begin(), m_areaBoundaries.end(), [&terminal](const std::shared_ptr<AreaBoundary>& ptrAreaBoundary) {
        return (static_cast<bool>(ptrAreaBoundary) && static_cast<bool>(ptrAreaBoundary->getTerminal())) ? stdcxx::areSame(terminal, ptrAreaBoundary->getTerminal().get()) : false;
    }), m_areaBoundaries.end());

    return *this;
}
Area& Area::removeAreaBoundary(Boundary& boundary, bool updateReferrer) {
    if(updateReferrer) {
        boundary.unregisterReferrer(*this);
    }
    m_areaBoundaries.erase(std::remove_if(m_areaBoundaries.begin(), m_areaBoundaries.end(), [&boundary](const std::shared_ptr<AreaBoundary>& ptrAreaBoundary) {
        return (static_cast<bool>(ptrAreaBoundary) && static_cast<bool>(ptrAreaBoundary->getBoundary())) ? stdcxx::areSame(boundary, ptrAreaBoundary->getBoundary().get()) : false;
    }), m_areaBoundaries.end());

    return *this;
}

std::shared_ptr<AreaBoundary> Area::getAreaBoundary(const Terminal& terminal) {
    auto it = std::find_if(m_areaBoundaries.begin(), m_areaBoundaries.end(), [&terminal](const std::shared_ptr<AreaBoundary>& ptrAreaBoundary) {
        return (static_cast<bool>(ptrAreaBoundary) && static_cast<bool>(ptrAreaBoundary->getTerminal())) ? stdcxx::areSame(terminal, ptrAreaBoundary->getTerminal().get()) : false;
    });
    return it != m_areaBoundaries.end() ? *it : std::shared_ptr<AreaBoundary>();
}
stdcxx::CReference<AreaBoundary> Area::getAreaBoundary(const Terminal& terminal) const {
    auto it = std::find_if(m_areaBoundaries.begin(), m_areaBoundaries.end(), [&terminal](const std::shared_ptr<AreaBoundary>& ptrAreaBoundary) {
        return (static_cast<bool>(ptrAreaBoundary) && static_cast<bool>(ptrAreaBoundary->getTerminal())) ? stdcxx::areSame(terminal, ptrAreaBoundary->getTerminal().get()) : false;
    });
    return (it != m_areaBoundaries.end() && static_cast<bool>(*it)) ? stdcxx::cref(**it) : stdcxx::cref<AreaBoundary>();
}
std::shared_ptr<AreaBoundary> Area::getAreaBoundary(const Boundary& boundary) {
    auto it = std::find_if(m_areaBoundaries.begin(), m_areaBoundaries.end(), [&boundary](const std::shared_ptr<AreaBoundary>& ptrAreaBoundary) {
        return (static_cast<bool>(ptrAreaBoundary) && static_cast<bool>(ptrAreaBoundary->getBoundary())) ? stdcxx::areSame(boundary, ptrAreaBoundary->getBoundary().get()) : false;
    });
    return it != m_areaBoundaries.end() ? *it : std::shared_ptr<AreaBoundary>();
}
stdcxx::CReference<AreaBoundary> Area::getAreaBoundary(const Boundary& boundary) const {
    auto it = std::find_if(m_areaBoundaries.begin(), m_areaBoundaries.end(), [&boundary](const std::shared_ptr<AreaBoundary>& ptrAreaBoundary) {
        return (static_cast<bool>(ptrAreaBoundary) && static_cast<bool>(ptrAreaBoundary->getBoundary())) ? stdcxx::areSame(boundary, ptrAreaBoundary->getBoundary().get()) : false;
    });
    return (it != m_areaBoundaries.end() && static_cast<bool>(*it)) ? stdcxx::cref(**it) : stdcxx::cref<AreaBoundary>();
}

std::vector<std::shared_ptr<AreaBoundary>>& Area::getAreaBoundaries() {
    return m_areaBoundaries;
}
stdcxx::const_range<AreaBoundary> Area::getAreaBoundaries() const {
    return m_areaBoundaries | boost::adaptors::indirected;
}

void Area::remove() {
    for (VoltageLevel& vl : m_voltageLevels) { //Remove this Area from every VoltageLevel
        vl.removeArea(*this);
    }

    for (auto& areaBoundary : m_areaBoundaries) {
        if(!areaBoundary) {
            continue;
        }

        auto terminalRef = areaBoundary->getTerminal();
        auto boundaryRef = areaBoundary->getBoundary();
        if (static_cast<bool>(boundaryRef)) {
            boundaryRef.get().unregisterReferrer(*this);
        }
        if (static_cast<bool>(terminalRef) && static_cast<bool>(terminalRef.get().getConnectable())) {
            terminalRef.get().unregisterReferrer(*this);
        }
    }

    getNetwork().remove(*this);
}

void Area::onReferencedRemoval(Boundary& removedReference) {
    removeAreaBoundary(removedReference, false);
}
void Area::onReferencedRemoval(Terminal& removedReference) {
    removeAreaBoundary(removedReference, false);
}

void Area::onReferencedReplacement(Boundary& /*oldReference*/, Boundary& /*newReference*/) {
    //Not implemented, since not used
    throw AssertionError("Not implemented");
}
void Area::onReferencedReplacement(Terminal& oldReference, Terminal& newReference) {
    for(auto& areaBoundary : m_areaBoundaries) {
        if(!areaBoundary) {
            continue;
        }
        areaBoundary->replaceTerminal(oldReference, newReference);
    }
}

}  // namespace iidm

}  // namespace powsybl
