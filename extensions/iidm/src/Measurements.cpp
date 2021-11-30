/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/Measurements.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/indirected.hpp>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/extensions/iidm/Measurement.hpp>
#include <powsybl/iidm/extensions/iidm/MeasurementAdder.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

Measurements::Measurements(Identifiable& identifiable) :
    Extension(identifiable) {
}

void Measurements::add(std::unique_ptr<Measurement>&& measurement) {
    m_measurements.emplace_back(std::move(measurement));
}

void Measurements::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Identifiable>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Identifiable>()));
    }
}

void Measurements::cleanIfEmpty() {
    if (getMeasurements().empty()) {
        getExtendable().get().removeExtension<Measurements>();
    }
}

stdcxx::CReference<Measurement> Measurements::getMeasurement(const std::string& id) const {
    auto it = std::find_if(m_measurements.begin(), m_measurements.end(), [&id](const std::unique_ptr<Measurement>& measurement) {
        return measurement->getId() == id;
    });
    return it != m_measurements.end() ? stdcxx::cref(*it) : stdcxx::cref<Measurement>();
}

stdcxx::Reference<Measurement> Measurements::getMeasurement(const std::string& id) {
    return stdcxx::ref(const_cast<const Measurements*>(this)->getMeasurement(id));
}

stdcxx::const_range<Measurement> Measurements::getMeasurements() const {
    return m_measurements | boost::adaptors::indirected;
}

stdcxx::const_range<Measurement> Measurements::getMeasurements(const Measurement::Type& type) const {
    const auto& filter = [&type](const std::unique_ptr<Measurement>& measurement) {
        return measurement->getType() == type;
    };

    return m_measurements | boost::adaptors::filtered(filter) | boost::adaptors::indirected;
}

stdcxx::range<Measurement> Measurements::getMeasurements(const Measurement::Type& type) {
    const auto& filter = [&type](const std::unique_ptr<Measurement>& measurement) {
        return measurement->getType() == type;
    };

    return m_measurements | boost::adaptors::filtered(filter) | boost::adaptors::indirected;
}

const std::string& Measurements::getName() const {
    static std::string s_name = "measurements";
    return s_name;
}

const std::type_index& Measurements::getType() const {
    static std::type_index s_type = typeid(Measurements);
    return s_type;
}

MeasurementAdder Measurements::newMeasurement() {
    return MeasurementAdder(*this);
}

void Measurements::remove(const Measurement& discreteMeasurement) {
    auto it = std::find_if(m_measurements.begin(), m_measurements.end(), [&discreteMeasurement](const std::unique_ptr<Measurement>& ptr) {
        return stdcxx::areSame(*ptr, discreteMeasurement);
    });

    if (it != m_measurements.end()) {
        m_measurements.erase(it);
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
