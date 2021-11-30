/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/DiscreteMeasurement.hpp>

#include <boost/range/adaptor/map.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurementValidationUtil.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurements.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<extensions::iidm::DiscreteMeasurement::Type>() {
    static std::initializer_list<std::string> s_typesNames {
        "TAP_POSITION",
        "SWITCH_POSITION",
        "SHUNT_COMPENSATOR_SECTION",
        "OTHER"
    };
    return s_typesNames;
}

template <>
const std::initializer_list<std::string>& getNames<extensions::iidm::DiscreteMeasurement::TapChanger>() {
    static std::initializer_list<std::string> s_tapChangersNames {
        "RATIO_TAP_CHANGER",
        "PHASE_TAP_CHANGER",
        "RATIO_TAP_CHANGER_1",
        "PHASE_TAP_CHANGER_1",
        "RATIO_TAP_CHANGER_2",
        "PHASE_TAP_CHANGER_2",
        "RATIO_TAP_CHANGER_3",
        "PHASE_TAP_CHANGER_3"
    };
    return s_tapChangersNames;
}

template <>
const std::initializer_list<std::string>& getNames<extensions::iidm::DiscreteMeasurement::ValueType>() {
    static std::initializer_list<std::string> s_valueTypesNames {
        "BOOLEAN",
        "INT",
        "STRING"
    };
    return s_valueTypesNames;
}

}  // namespace Enum

namespace extensions {

namespace iidm {

std::ostream& operator<<(std::ostream& stream, const DiscreteMeasurement::Type& type) {
    stream << Enum::toString(type);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const DiscreteMeasurement::TapChanger& tapChanger) {
    stream << Enum::toString(tapChanger);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const DiscreteMeasurement::ValueType& valueType) {
    stream << Enum::toString(valueType);
    return stream;
}

DiscreteMeasurement::DiscreteMeasurement(DiscreteMeasurements& discreteMeasurements, const std::string& id, const Type& type,
                                         const stdcxx::optional<TapChanger>& tapChanger, const std::map<std::string, std::string>& properties,
                                         const ValueType& valueType, const boost::any& value, bool valid) :
    m_discreteMeasurements(discreteMeasurements),
    m_id(id),
    m_type(type),
    m_tapChanger(tapChanger),
    m_properties(properties),
    m_valueType(valueType),
    m_value(value),
    m_valid(valid) {
}

const std::string& DiscreteMeasurement::getId() const {
    return m_id;
}

std::string DiscreteMeasurement::getProperty(const std::string& name) const {
    const auto& it = m_properties.find(name);
    return it != m_properties.cend() ? it->second : "";
}

stdcxx::const_range<std::string> DiscreteMeasurement::getPropertyNames() const {
    return m_properties | boost::adaptors::map_keys;
}

stdcxx::optional<DiscreteMeasurement::TapChanger> DiscreteMeasurement::getTapChanger() const {
    return m_tapChanger;
}

const DiscreteMeasurement::Type& DiscreteMeasurement::getType() const {
    return m_type;
}

bool DiscreteMeasurement::getValueAsBoolean() const {
    if (m_valueType == ValueType::BOOLEAN) {
        return boost::any_cast<bool>(m_value);
    }
    throw PowsyblException(stdcxx::format("Value type is not BOOLEAN but is: %1%", m_valueType));
}

int DiscreteMeasurement::getValueAsInt() const {
    if (m_valueType == ValueType::INT) {
        return boost::any_cast<int>(m_value);
    }
    throw PowsyblException(stdcxx::format("Value type is not INT but is: %1%", m_valueType));
}

std::string DiscreteMeasurement::getValueAsString() const {
    if (m_valueType == ValueType::STRING) {
        return boost::any_cast<std::string>(m_value);
    }
    throw PowsyblException(stdcxx::format("Value type is not STRING but is: %1%", m_valueType));
}

const DiscreteMeasurement::ValueType& DiscreteMeasurement::getValueType() const {
    return m_valueType;
}

bool DiscreteMeasurement::isValid() const {
    return m_valid;
}

DiscreteMeasurement& DiscreteMeasurement::putProperty(const std::string& name, const std::string& property) {
    if (name.empty()) {
        throw PowsyblException("Unexpected empty property name");
    }
    m_properties[name] = property;
    return *this;
}

void DiscreteMeasurement::remove() {
    m_discreteMeasurements.remove(*this);
}

DiscreteMeasurement& DiscreteMeasurement::removeProperty(const std::string& name) {
    m_properties.erase(name);
    return *this;
}

DiscreteMeasurement& DiscreteMeasurement::setValid(bool valid) {
    m_valid = valid;
    return *this;
}

DiscreteMeasurement& DiscreteMeasurement::setValue(const std::string& value) {
    DiscreteMeasurementValidationUtil::checkValue(value, isValid());
    m_value = value;
    m_valueType = ValueType::STRING;
    return *this;
}

DiscreteMeasurement& DiscreteMeasurement::setValue(const char* value) {
    DiscreteMeasurementValidationUtil::checkValue(value, isValid());
    m_value = std::string(value);
    m_valueType = ValueType::STRING;
    return *this;
}

DiscreteMeasurement& DiscreteMeasurement::setValue(bool value) {
    m_value = value;
    m_valueType = ValueType::BOOLEAN;
    return *this;
}

DiscreteMeasurement& DiscreteMeasurement::setValue(int value) {
    m_value = value;
    m_valueType = ValueType::INT;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
