/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ControlZone.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/extensions/iidm/ControlUnit.hpp>
#include <powsybl/iidm/extensions/iidm/PilotPoint.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ControlZone::ControlZone(const std::string& name, const PilotPoint& pilotPoint, const std::vector<ControlUnit>& controlUnits) :
    m_pilotPoint(pilotPoint) {
    if (name.empty()) {
        throw PowsyblException("Zone name is not set");
    }
    m_name = name;
    if(controlUnits.empty()){
        throw PowsyblException("Empty control unit list");
    }
    m_controlUnits = controlUnits;
}

const std::string& ControlZone::getName() const {
    return m_name;
}
const PilotPoint& ControlZone::getPilotPoint() const {
    return m_pilotPoint;
}
PilotPoint& ControlZone::getPilotPoint() {
    return m_pilotPoint;
}
const std::vector<ControlUnit>& ControlZone::getControlUnits() const {
    return m_controlUnits;
}
std::vector<ControlUnit>& ControlZone::getControlUnits() {
    return m_controlUnits;
}
stdcxx::optional<ControlUnit> ControlZone::getControlUnit(const std::string id) const {
    for (auto controlUnit : m_controlUnits) {
        if(controlUnit.getId() == id) {
            return stdcxx::optional<ControlUnit>(controlUnit);
        }
    }
    return stdcxx::optional<ControlUnit>();
}
stdcxx::Reference<ControlUnit> ControlZone::getControlUnit(const std::string id) {
        for (auto& controlUnit : m_controlUnits) {
        if(controlUnit.getId() == id) {
            return stdcxx::ref(controlUnit);
        }
    }
    return stdcxx::Reference<ControlUnit>();
}

void ControlZone::setVariantManagerHolder(const VariantManagerHolder& variantManagerHolder) {
    m_pilotPoint.setVariantManagerHolder(variantManagerHolder);
    for (auto& controlUnit : m_controlUnits) {
        controlUnit.setVariantManagerHolder(variantManagerHolder);
    }
}

void ControlZone::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    m_pilotPoint.allocateVariantArrayElement(indexes, sourceIndex);
    for (auto& controlUnit : m_controlUnits) {
        controlUnit.allocateVariantArrayElement(indexes, sourceIndex);
    }
}

void ControlZone::deleteVariantArrayElement(unsigned long /*index*/) {
    //nothing to do
}

void ControlZone::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    m_pilotPoint.extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    for (auto& controlUnit : m_controlUnits) {
        controlUnit.extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
}

void ControlZone::reduceVariantArraySize(unsigned long number) {
    m_pilotPoint.reduceVariantArraySize(number);
    for (auto& controlUnit : m_controlUnits) {
        controlUnit.reduceVariantArraySize(number);
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl