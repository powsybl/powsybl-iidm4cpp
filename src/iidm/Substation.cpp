/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Substation.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/util/DistinctPredicate.hpp>
#include <powsybl/iidm/util/Substations.hpp>
#include <powsybl/iidm/util/VoltageLevels.hpp>
#include <powsybl/stdcxx/flattened.hpp>

namespace powsybl {

namespace iidm {

Substation::Substation(Network& network, const std::string& id, const std::string& name, bool fictitious, const stdcxx::optional<Country>& country, const std::string& tso, const std::set<std::string>& geographicalTags) :
    Container(id, name, fictitious, Container::Type::SUBSTATION),
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

const stdcxx::optional<Country>& Substation::getCountry() const {
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

unsigned long Substation::getThreeWindingsTransformerCount() const {
    return boost::size(getThreeWindingsTransformers());
}

stdcxx::const_range<ThreeWindingsTransformer> Substation::getThreeWindingsTransformers() const {
    const auto& mapper = [](const std::reference_wrapper<VoltageLevel>& voltageLevel) {
        return voltageLevel.get().getConnectables<ThreeWindingsTransformer>();
    };

    return m_voltageLevels | boost::adaptors::transformed(mapper) | stdcxx::flattened | boost::adaptors::filtered(DistinctPredicate());
}

stdcxx::range<ThreeWindingsTransformer> Substation::getThreeWindingsTransformers() {
    const auto& mapper = [](const std::reference_wrapper<VoltageLevel>& voltageLevel) {
        return voltageLevel.get().getConnectables<ThreeWindingsTransformer>();
    };

    return m_voltageLevels | boost::adaptors::transformed(mapper) | stdcxx::flattened | boost::adaptors::filtered(DistinctPredicate());
}

const std::string& Substation::getTso() const {
    return m_tso;
}

unsigned long Substation::getTwoWindingsTransformerCount() const {
    return boost::size(getTwoWindingsTransformers());
}

stdcxx::const_range<TwoWindingsTransformer> Substation::getTwoWindingsTransformers() const {
    const auto& mapper = [](const std::reference_wrapper<VoltageLevel>& voltageLevel) {
        return voltageLevel.get().getConnectables<TwoWindingsTransformer>();
    };

    return m_voltageLevels | boost::adaptors::transformed(mapper) | stdcxx::flattened | boost::adaptors::filtered(DistinctPredicate());
}

stdcxx::range<TwoWindingsTransformer> Substation::getTwoWindingsTransformers() {
    const auto& mapper = [](const std::reference_wrapper<VoltageLevel>& voltageLevel) {
        return voltageLevel.get().getConnectables<TwoWindingsTransformer>();
    };

    return m_voltageLevels | boost::adaptors::transformed(mapper) | stdcxx::flattened | boost::adaptors::filtered(DistinctPredicate());
}

const std::string& Substation::getTypeDescription() const {
    static std::string s_typeDescription = "Substation";

    return s_typeDescription;
}

stdcxx::const_range<VoltageLevel> Substation::getVoltageLevels() const {
    return m_voltageLevels;
}

stdcxx::range<VoltageLevel> Substation::getVoltageLevels() {
    return m_voltageLevels;
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

void Substation::remove() {
    Substations::checkRemovability(*this);

    for (VoltageLevel& vl : m_voltageLevels) {
        // Remove all branches, transformers and HVDC lines
        for (Connectable& connectable : vl.getConnectables()) {
            ConnectableType type = connectable.getType();
            if (VoltageLevels::MULTIPLE_TERMINALS_CONNECTABLE_TYPES.find(type) != VoltageLevels::MULTIPLE_TERMINALS_CONNECTABLE_TYPES.end()) {
                connectable.remove();
            } else if (type == ConnectableType::HVDC_CONVERTER_STATION) {
                const auto& hvdcLine = getNetwork().findHvdcLine(dynamic_cast<HvdcConverterStation&>(connectable));
                if (hvdcLine) {
                    hvdcLine.get().remove();
                }
            }
        }

        // Then remove the voltage level (bus, switches and injections) from the network
        vl.remove();
    }

    // Remove this substation from the network
    getNetwork().remove(*this);
}

void Substation::remove(const VoltageLevel& voltageLevel) {
    auto it = std::find_if(m_voltageLevels.begin(), m_voltageLevels.end(), [&voltageLevel](const std::reference_wrapper<VoltageLevel>& vl) {
        return stdcxx::areSame(voltageLevel, vl.get());
    });
    if (it != m_voltageLevels.end()) {
        m_voltageLevels.erase(it);
    }
}

Substation& Substation::setCountry(const stdcxx::optional<Country>& country) {
    m_country = country;
    return *this;
}

void Substation::setNetworkRef(Network& network) {
    m_network.set(network);
}

Substation& Substation::setTso(const std::string& tso) {
    m_tso = tso;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
