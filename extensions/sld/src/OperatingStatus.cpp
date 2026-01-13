/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/sld/OperatingStatus.hpp>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/Enum.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<extensions::sld::OperatingStatus::Status>() {
    static std::initializer_list<std::string> s_names {
        "IN_OPERATION",
        "PLANNED_OUTAGE",
        "FORCED_OUTAGE"
    };
    return s_names;
}

}  // namespace Enum

namespace extensions {

namespace sld {

std::ostream& operator<<(std::ostream& stream, const OperatingStatus::Status& status) {
    stream << Enum::toString(status);
    return stream;
}

OperatingStatus::OperatingStatus(Identifiable& identifiable, const Status& status) :
    Extension(identifiable),
    m_status(status) {
    if (!isAllowedIdentifiable(identifiable)) {
        throw PowsyblException(stdcxx::format("Operating status extension is not allowed on identifiable type: %1%", identifiable.getType()));
    }
}

void OperatingStatus::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Identifiable>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Identifiable>()));
    }
}

bool OperatingStatus::isAllowedIdentifiable(Identifiable& identifiable) {
    IdentifiableType identType = identifiable.getType();
    if(identType == IdentifiableType::BUSBAR_SECTION ||
        identType == IdentifiableType::LINE || 
        identType == IdentifiableType::HVDC_LINE || 
        identType == IdentifiableType::TIE_LINE || 
        identType == IdentifiableType::DANGLING_LINE || 
        identType == IdentifiableType::TWO_WINDINGS_TRANSFORMER || 
        identType == IdentifiableType::THREE_WINDINGS_TRANSFORMER) {
            return true;
        }
    return false;
}

const std::string& OperatingStatus::getName() const {
    static std::string s_name = "operatingStatus";
    return s_name;
}

const OperatingStatus::Status& OperatingStatus::getStatus() const {
    return m_status;
}

const std::type_index& OperatingStatus::getType() const {
    static std::type_index s_type = typeid(OperatingStatus);
    return s_type;
}

OperatingStatus& OperatingStatus::setStatus(const Status& status) {
    m_status = status;
    return *this;
}

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
