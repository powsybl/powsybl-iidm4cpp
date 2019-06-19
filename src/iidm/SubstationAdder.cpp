/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/SubstationAdder.hpp>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

SubstationAdder::SubstationAdder(Network& network) :
    m_network(network) {
}

Substation& SubstationAdder::add() {
    std::unique_ptr<Substation> ptrSubstation = stdcxx::make_unique<Substation>(m_network, getId(), getName(), m_country, m_tso, m_geographicalTags);
    auto& substation = m_network.checkAndAdd<Substation>(std::move(ptrSubstation));

    return substation;
}

SubstationAdder& SubstationAdder::addGeographicalTag(const std::string& geographicalTag) {
    m_geographicalTags.insert(geographicalTag);
    return *this;
}

Network& SubstationAdder::getNetwork() {
    return m_network;
}

const std::string& SubstationAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Substation";

    return s_typeDescription;
}

SubstationAdder& SubstationAdder::setCountry(const Country& country) {
    m_country = country;
    return *this;
}

SubstationAdder& SubstationAdder::setGeographicalTags(const std::initializer_list<std::string>& geographicalTags) {
    m_geographicalTags = std::set<std::string>(geographicalTags.begin(), geographicalTags.end());
    return *this;
}

SubstationAdder& SubstationAdder::setTso(const std::string& tso) {
    m_tso = tso;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
