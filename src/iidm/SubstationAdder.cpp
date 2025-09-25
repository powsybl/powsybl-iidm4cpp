/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/SubstationAdder.hpp>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

SubstationAdder::SubstationAdder(Network& network) :
    m_rootNetwork(network) {
}

SubstationAdder::SubstationAdder(Network& rootNetwork, Network& subNetworkRef) :
    m_rootNetwork(rootNetwork),
    m_subNetworkRef(stdcxx::ref<Network>(subNetworkRef)) {

}

Substation& SubstationAdder::add() {
    std::unique_ptr<Substation> ptrSubstation;
    
    if(static_cast<bool>(m_subNetworkRef)) {
        ptrSubstation = stdcxx::make_unique<Substation>(m_rootNetwork, m_subNetworkRef.get(), checkAndGetUniqueId(), getName(), isFictitious(), m_country, m_tso, m_geographicalTags);
    } else {
        ptrSubstation = stdcxx::make_unique<Substation>(m_rootNetwork, checkAndGetUniqueId(), getName(), isFictitious(), m_country, m_tso, m_geographicalTags);
    }
    auto& substation = m_rootNetwork.checkAndAdd<Substation>(std::move(ptrSubstation));

    return substation;
}

SubstationAdder& SubstationAdder::addGeographicalTag(const std::string& geographicalTag) {
    m_geographicalTags.insert(geographicalTag);
    return *this;
}

const Network& SubstationAdder::getNetwork() const {
    return m_rootNetwork;
}

Network& SubstationAdder::getNetwork() {
    return m_rootNetwork;
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
