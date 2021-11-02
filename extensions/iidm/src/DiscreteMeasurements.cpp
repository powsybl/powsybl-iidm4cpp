/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/DiscreteMeasurements.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/indirected.hpp>

#include <powsybl/iidm/Extendable.hpp>
#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurementAdder.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

DiscreteMeasurements::DiscreteMeasurements(Identifiable& identifiable) :
    Extension(identifiable) {
}

void DiscreteMeasurements::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Identifiable>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Identifiable>()));
    }
}

void DiscreteMeasurements::cleanIfEmpty() {
    if (getDiscreteMeasurements().empty()) {
        getExtendable().get().removeExtension<DiscreteMeasurements>();
    }
}

DiscreteMeasurements& DiscreteMeasurements::add(std::unique_ptr<DiscreteMeasurement>&& discreteMeasurement) {
    m_discreteMeasurements.emplace_back(std::move(discreteMeasurement));
    return *this;
}

stdcxx::CReference<DiscreteMeasurement> DiscreteMeasurements::getDiscreteMeasurement(const std::string& id) const {
    auto it = std::find_if(m_discreteMeasurements.begin(), m_discreteMeasurements.end(), [&id](const std::unique_ptr<DiscreteMeasurement>& measurement) {
        return measurement->getId() == id;
    });
    return it != m_discreteMeasurements.end() ? stdcxx::cref(*it) : stdcxx::cref<DiscreteMeasurement>();
}

stdcxx::Reference<DiscreteMeasurement> DiscreteMeasurements::getDiscreteMeasurement(const std::string& id) {
    return stdcxx::ref(const_cast<const DiscreteMeasurements*>(this)->getDiscreteMeasurement(id));
}

stdcxx::const_range<DiscreteMeasurement> DiscreteMeasurements::getDiscreteMeasurements() const {
    return m_discreteMeasurements | boost::adaptors::indirected;
}

stdcxx::range<DiscreteMeasurement> DiscreteMeasurements::getDiscreteMeasurements() {
    return m_discreteMeasurements | boost::adaptors::indirected;
}

stdcxx::const_range<DiscreteMeasurement> DiscreteMeasurements::getDiscreteMeasurements(const DiscreteMeasurement::Type& type) const {
    const auto& filter = [&type](const std::unique_ptr<DiscreteMeasurement>& measurement) {
        return measurement->getType() == type;
    };

    return m_discreteMeasurements | boost::adaptors::filtered(filter) | boost::adaptors::indirected;
}

stdcxx::range<DiscreteMeasurement> DiscreteMeasurements::getDiscreteMeasurements(const DiscreteMeasurement::Type& type) {
    const auto& filter = [&type](const std::unique_ptr<DiscreteMeasurement>& measurement) {
        return measurement->getType() == type;
    };

    return m_discreteMeasurements | boost::adaptors::filtered(filter) | boost::adaptors::indirected;
}

const std::string& DiscreteMeasurements::getName() const {
    static std::string s_name = "discreteMeasurements";
    return s_name;
}

const std::type_index& DiscreteMeasurements::getType() const {
    static std::type_index s_type = typeid(DiscreteMeasurements);
    return s_type;
}

DiscreteMeasurementAdder DiscreteMeasurements::newDiscreteMeasurement() {
    return DiscreteMeasurementAdder(*this);
}

void DiscreteMeasurements::remove(const DiscreteMeasurement& discreteMeasurement) {
    auto it = std::find_if(m_discreteMeasurements.begin(), m_discreteMeasurements.end(), [&discreteMeasurement](const std::unique_ptr<DiscreteMeasurement>& ptr) {
        return stdcxx::areSame(*ptr, discreteMeasurement);
    });

    if (it != m_discreteMeasurements.end()) {
        m_discreteMeasurements.erase(it);
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
