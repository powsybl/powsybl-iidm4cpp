/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/SecondaryVoltageControl.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/extensions/iidm/ControlZone.hpp>

#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

SecondaryVoltageControl::SecondaryVoltageControl(Network& network, const std::vector<ControlZone>& controlZones) : 
    AbstractMultiVariantIdentifiableExtension(network) {
    if(controlZones.empty()) {
        throw PowsyblException("Empty control zone list");
    }
    m_controlZones = controlZones;
    for (auto& controlZone : m_controlZones) {
        controlZone.setVariantManagerHolder(network);
    } 
}

const std::string& SecondaryVoltageControl::getName() const {
    static std::string s_name = "secondaryVoltageControl";
    return s_name;
}

const std::type_index& SecondaryVoltageControl::getType() const {
    static std::type_index s_type = typeid(SecondaryVoltageControl);
    return s_type;
}

const std::vector<ControlZone>& SecondaryVoltageControl::getControlZones() const {
    return m_controlZones;
}
std::vector<ControlZone>& SecondaryVoltageControl::getControlZones() {
    return m_controlZones;
}

stdcxx::optional<ControlZone> SecondaryVoltageControl::getControlZone(const std::string name) const {
    for (auto controlZone : m_controlZones) {
        if(controlZone.getName() == name) {
            return stdcxx::optional<ControlZone>(controlZone);
        }
    }
    return stdcxx::optional<ControlZone>();
}
stdcxx::Reference<ControlZone> SecondaryVoltageControl::getControlZone(const std::string name) {
    for (auto& controlZone : m_controlZones) {
        if(controlZone.getName() == name) {
            return stdcxx::ref(controlZone);
        }
    }
    return stdcxx::Reference<ControlZone>();
}

void SecondaryVoltageControl::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Network>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Network>()));
    }
}

void SecondaryVoltageControl::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (auto& controlZone : m_controlZones) {
        controlZone.allocateVariantArrayElement(indexes, sourceIndex);
    }
}

void SecondaryVoltageControl::deleteVariantArrayElement(unsigned long /*index*/) {
    //nothing to do;
}

void SecondaryVoltageControl::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    for (auto& controlZone : m_controlZones) {
        controlZone.extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
}

void SecondaryVoltageControl::reduceVariantArraySize(unsigned long number) {
    for (auto& controlZone : m_controlZones) {
        controlZone.reduceVariantArraySize(number);
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl