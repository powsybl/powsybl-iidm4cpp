/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/OperationalLimitsHolder.hpp>

#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/map.hpp>

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Identifiable.hpp>

namespace powsybl {

namespace iidm {

OperationalLimitsHolder::OperationalLimitsHolder(Identifiable& identifiable, std::string&& attributeName) :
    m_identifiable(identifiable),
    m_attributeName(std::move(attributeName)) {
}

std::string OperationalLimitsHolder::getMessageHeader() const {
    return m_identifiable.getMessageHeader();
}

stdcxx::const_range<OperationalLimits> OperationalLimitsHolder::getOperationalLimits() const {
    return m_operationalLimits | boost::adaptors::map_values | boost::adaptors::indirected;
}

stdcxx::range<OperationalLimits> OperationalLimitsHolder::getOperationalLimits() {
    return m_operationalLimits | boost::adaptors::map_values | boost::adaptors::indirected;
}

ActivePowerLimitsAdder OperationalLimitsHolder::newActivePowerLimits() {
    return ActivePowerLimitsAdder(*this);
}

ApparentPowerLimitsAdder OperationalLimitsHolder::newApparentPowerLimits() {
    return ApparentPowerLimitsAdder(*this);
}

CurrentLimitsAdder OperationalLimitsHolder::newCurrentLimits() {
    return CurrentLimitsAdder(*this);
}

}  // namespace iidm

}  // namespace powsybl
