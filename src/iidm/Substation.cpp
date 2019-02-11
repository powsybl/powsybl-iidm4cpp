/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>

namespace powsybl {

namespace iidm {

Substation::Substation(Network& network, const std::string& id, const std::string& name, const Country& country, const std::string& tso, const std::set<std::string>& geographicalTags) :
    Container(id, name, Container::Type::SUBSTATION),
    m_network(network),
    m_country(country),
    m_tso(tso),
    m_geographicalTags(geographicalTags) {
}

Substation& Substation::addGeographicalTag(const std::string& geographicalTag) {
    m_geographicalTags.insert(geographicalTag);
    return *this;
}

void Substation::addVoltageLevel(VoltageLevel& voltageLevel) {
    m_voltageLevels.emplace_back(std::ref(voltageLevel));
}

const Country& Substation::getCountry() const {
    return m_country;
}

const std::set<std::string>& Substation::getGeographicalTags() const {
    return m_geographicalTags;
}

const Network& Substation::getNetwork() const {
    return m_network.get();
}

Network& Substation::getNetwork() {
    return m_network.get();
}

const std::string& Substation::getTso() const {
    return m_tso;
}

const std::string& Substation::getTypeDescription() const {
    static std::string s_typeDescription = "Substation";

    return s_typeDescription;
}

ThreeWindingsTransformerAdder Substation::newThreeWindingsTransformer() {
    return ThreeWindingsTransformerAdder(*this);
}

TwoWindingsTransformerAdder Substation::newTwoWindingsTransformer() {
    return TwoWindingsTransformerAdder(*this);
}

VoltageLevelAdder Substation::newVoltageLevel() {
    return VoltageLevelAdder(*this);
}

Substation& Substation::setCountry(const Country& country) {
    m_country = country;
    return *this;
}

Substation& Substation::setTso(const std::string& tso) {
    m_tso = tso;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
