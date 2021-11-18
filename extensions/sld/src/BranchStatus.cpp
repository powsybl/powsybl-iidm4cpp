/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/sld/BranchStatus.hpp>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/Enum.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<extensions::sld::BranchStatus::Status>() {
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

std::ostream& operator<<(std::ostream& stream, const BranchStatus::Status& status) {
    stream << Enum::toString(status);
    return stream;
}

BranchStatus::BranchStatus(Connectable& connectable, const Status& status) :
    Extension(connectable),
    m_status(status) {
}

void BranchStatus::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Connectable>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Connectable>()));
    }
}

const std::string& BranchStatus::getName() const {
    static std::string s_name = "branchStatus";
    return s_name;
}

const BranchStatus::Status& BranchStatus::getStatus() const {
    return m_status;
}

const std::type_index& BranchStatus::getType() const {
    static std::type_index s_type = typeid(BranchStatus);
    return s_type;
}

BranchStatus& BranchStatus::setStatus(const Status& status) {
    m_status = status;
    return *this;
}

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
