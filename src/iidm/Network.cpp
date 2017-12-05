/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

Network::Network(const std::string& id, const std::string& sourceFormat) :
    Container(id, id, Container::Type::NETWORK),
    m_sourceFormat(checkNotEmpty(*this, sourceFormat, "Source format is empty")),
    m_forecastDistance(0) {
}

const std::string& Network::getTypeDescription() const {
    static std::string s_typeDescription = "Network";

    return s_typeDescription;
}

int Network::getForecastDistance() const {
    return m_forecastDistance;
}

Network& Network::setForecastDistance(int forecastDistance) {
    m_forecastDistance = checkForecastDistance(*this, forecastDistance);
    return *this;
}

const std::string& Network::getSourceFormat() const {
    return m_sourceFormat;
}

Substation& Network::getSubstation(const std::string& id) const {
    return m_objectStore.get<Substation>(id);
}

unsigned int Network::getSubstationCount() const {
    return m_objectStore.getObjectCount<Substation>();
}


VoltageLevel& Network::getVoltageLevel(const std::string& id) const {
    return m_objectStore.get<VoltageLevel>(id);
}

unsigned int Network::getVoltageLevelCount() const {
    return m_objectStore.getObjectCount<VoltageLevel>();
}

SubstationAdder Network::newSubstation() {
    return SubstationAdder(*this);
}



}

}
