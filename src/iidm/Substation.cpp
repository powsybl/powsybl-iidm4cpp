/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Substation.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/join.hpp>

#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/util/DistinctPredicate.hpp>

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
    unsigned long res = 0UL;
    for (const auto& it : m_voltageLevels) {
        res += it.get().getConnectableCount<ThreeWindingsTransformer>();
    }
    return res / 3UL;
}

stdcxx::const_range<ThreeWindingsTransformer> Substation::getThreeWindingsTransformers() const {
    stdcxx::const_range<ThreeWindingsTransformer> range;

    for (const auto& it : m_voltageLevels) {
        range = boost::range::join(range, it.get().getConnectables<ThreeWindingsTransformer>());
    }

    return range | boost::adaptors::filtered(DistinctPredicate());
}

stdcxx::range<ThreeWindingsTransformer> Substation::getThreeWindingsTransformers() {
    stdcxx::range<ThreeWindingsTransformer> range;

    for (const auto& it : m_voltageLevels) {
        range = boost::range::join(range, it.get().getConnectables<ThreeWindingsTransformer>());
    }

    return range | boost::adaptors::filtered(DistinctPredicate());
}

const std::string& Substation::getTso() const {
    return m_tso;
}

unsigned long Substation::getTwoWindingsTransformerCount() const {
    unsigned long res = 0UL;
    for (const auto& it : m_voltageLevels) {
        res += it.get().getConnectableCount<TwoWindingsTransformer>();
    }
    return res / 2UL;
}

stdcxx::const_range<TwoWindingsTransformer> Substation::getTwoWindingsTransformers() const {
    stdcxx::const_range<TwoWindingsTransformer> range;

    for (const auto& it : m_voltageLevels) {
        range = boost::range::join(range, it.get().getConnectables<TwoWindingsTransformer>());
    }

    return range | boost::adaptors::filtered(DistinctPredicate());
}

stdcxx::range<TwoWindingsTransformer> Substation::getTwoWindingsTransformers() {
    stdcxx::range<TwoWindingsTransformer> range;

    for (const auto& it : m_voltageLevels) {
        range = boost::range::join(range, it.get().getConnectables<TwoWindingsTransformer>());
    }

    return range | boost::adaptors::filtered(DistinctPredicate());
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

Substation& Substation::setCountry(const stdcxx::optional<Country>& country) {
    m_country = country;
    return *this;
}

Substation& Substation::setFictitious(bool fictitious) {
    Container::setFictitious(fictitious);
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
