/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

Network::Network(const std::string& id, const std::string& sourceFormat) :
    Container(id, id, Container::Type::NETWORK),
    m_sourceFormat(checkNotEmpty(*this, sourceFormat, "Source format is empty")),
    m_forecastDistance(0),
    m_objectStore(),
    m_stateManager(*this) {
}

BusbarSection& Network::getBusbarSection(const std::string& id) const {
    return m_objectStore.get<BusbarSection>(id);
}

unsigned long Network::getBusbarSectionCount() const {
    return m_objectStore.getObjectCount<BusbarSection>();
}

Connectable& Network::getConnectable(const std::string& id) const {
    Identifiable& identifiable = m_objectStore.get<Identifiable>(id);
    assert(stdcxx::isInstanceOf<Connectable>(identifiable));

    return dynamic_cast<Connectable&>(identifiable);
}

int Network::getForecastDistance() const {
    return m_forecastDistance;
}

Load& Network::getLoad(const std::string& id) const {
    return m_objectStore.get<Load>(id);
}

unsigned long Network::getLoadCount() const {
    return m_objectStore.getObjectCount<Load>();
}

const std::string& Network::getSourceFormat() const {
    return m_sourceFormat;
}

unsigned long Network::getStateIndex() const {
    return m_stateManager.getStateIndex();
}

StateManager& Network::getStateManager() const {
    return const_cast<StateManager&>(m_stateManager);
}

Substation& Network::getSubstation(const std::string& id) const {
    return m_objectStore.get<Substation>(id);
}

unsigned long Network::getSubstationCount() const {
    return m_objectStore.getObjectCount<Substation>();
}

const std::string& Network::getTypeDescription() const {
    static std::string s_typeDescription = "Network";

    return s_typeDescription;
}

VoltageLevel& Network::getVoltageLevel(const std::string& id) const {
    return m_objectStore.get<VoltageLevel>(id);
}

unsigned long Network::getVoltageLevelCount() const {
    return m_objectStore.getObjectCount<VoltageLevel>();
}

SubstationAdder Network::newSubstation() {
    return SubstationAdder(*this);
}

void Network::remove(Identifiable& identifiable) {
    m_objectStore.remove(identifiable);
}

Network& Network::setForecastDistance(int forecastDistance) {
    m_forecastDistance = checkForecastDistance(*this, forecastDistance);
    return *this;
}

}

}
