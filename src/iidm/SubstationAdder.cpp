/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/SubstationAdder.hpp>

#include <powsybl/iidm/Substation.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

SubstationAdder::SubstationAdder(Network& network) :
    m_network(network) {
}

Substation& SubstationAdder::add() {
    checkOptional(*this, m_country, getMessageHeader());

    std::unique_ptr<Substation> substation(new Substation(m_network, m_id, m_name, *m_country, m_tso));
    m_network.getObjectStore().checkAndAdd<Substation>(std::move(substation));

    return m_network.getSubstation(m_id);
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

SubstationAdder& SubstationAdder::setTso(const std::string& tso) {
    m_tso = tso;
    return *this;
}

}

}
