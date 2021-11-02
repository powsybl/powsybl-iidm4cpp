/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/Measurement.hpp>

#include <boost/range/adaptor/map.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/extensions/iidm/Measurements.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<extensions::iidm::Measurement::Side>() {
    static std::initializer_list<std::string> s_sidesNames {
        "ONE",
        "TWO",
        "THREE"
    };
    return s_sidesNames;
}

template <>
const std::initializer_list<std::string>& getNames<extensions::iidm::Measurement::Type>() {
    static std::initializer_list<std::string> s_typesNames {
        "ANGLE",
        "ACTIVE_POWER",
        "APPARENT_POWER",
        "REACTIVE_POWER",
        "CURRENT",
        "VOLTAGE",
        "FREQUENCY",
        "OTHER"
    };
    return s_typesNames;
}

}  // namespace Enum

namespace extensions {

namespace iidm {

std::ostream& operator<<(std::ostream& stream, const Measurement::Side& value) {
    stream << Enum::toString(value);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Measurement::Type& value) {
    stream << Enum::toString(value);
    return stream;
}

Measurement::Measurement(Measurements& measurements, const std::string& id, const Measurement::Type& type, const std::map<std::string, std::string>& properties,
                         double value, double standardDeviation, bool valid, const stdcxx::optional<Measurement::Side>& side) :
    m_measurements(measurements),
    m_id(id),
    m_type(type),
    m_properties(properties),
    m_side(side),
    m_value(value),
    m_standardDeviation(standardDeviation),
    m_valid(valid) {
}

const std::string& Measurement::getId() const {
    return m_id;
}

std::string Measurement::getProperty(const std::string& name) const {
    auto it = m_properties.find(name);
    return it != m_properties.end() ? it->second : "";
}

stdcxx::const_range<std::string> Measurement::getPropertyNames() const {
    return m_properties | boost::adaptors::map_keys;
}

const stdcxx::optional<Measurement::Side>& Measurement::getSide() const {
    return m_side;
}

double Measurement::getStandardDeviation() const {
    return m_standardDeviation;
}

const Measurement::Type& Measurement::getType() const {
    return m_type;
}

double Measurement::getValue() const {
    return m_value;
}

bool Measurement::isValid() const {
    return m_valid;
}

Measurement& Measurement::putProperty(const std::string& name, const std::string& property) {
    if (name.empty()) {
        throw PowsyblException("Unexpected empty property name");
    }
    m_properties[name] = property;
    return *this;
}

void Measurement::remove() {
    m_measurements.remove(*this);
}

Measurement& Measurement::removeProperty(const std::string& name) {
    m_properties.erase(name);
    return *this;
}

Measurement& Measurement::setStandardDeviation(double standardDeviation) {
    m_standardDeviation = standardDeviation;
    return *this;
}

Measurement& Measurement::setValid(bool valid) {
    m_valid = valid;
    return *this;
}

Measurement& Measurement::setValue(double value) {
    m_value = value;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
