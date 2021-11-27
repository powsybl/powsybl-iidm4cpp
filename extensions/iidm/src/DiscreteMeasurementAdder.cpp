/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/DiscreteMeasurementAdder.hpp>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurementValidationUtil.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurements.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

DiscreteMeasurementAdder::DiscreteMeasurementAdder(DiscreteMeasurements& discreteMeasurements) :
    m_discreteMeasurements(discreteMeasurements) {
}

DiscreteMeasurement& DiscreteMeasurementAdder::add() {
    DiscreteMeasurementValidationUtil::checkId(m_id, m_discreteMeasurements);
    DiscreteMeasurementValidationUtil::checkType(m_type, m_discreteMeasurements.getExtendable<Identifiable>().get());
    DiscreteMeasurementValidationUtil::checkTapChanger(m_tapChanger, m_type, m_discreteMeasurements.getExtendable<Identifiable>().get());
    DiscreteMeasurementValidationUtil::checkValue(m_value, m_valid);

    auto discreteMeasurement = std::unique_ptr<DiscreteMeasurement>(
        new DiscreteMeasurement(m_discreteMeasurements, m_id, m_type, m_tapChanger, m_properties, DiscreteMeasurementValidationUtil::getValueType(m_value), m_value, m_valid));
    m_discreteMeasurements.add(std::move(discreteMeasurement));

    return m_discreteMeasurements.getDiscreteMeasurement(m_id);
}

DiscreteMeasurementAdder& DiscreteMeasurementAdder::putProperty(const std::string& name, const std::string& value) {
    if (name.empty()) {
        throw PowsyblException("Unexpected empty property name");
    }
    m_properties[name] = value;
    return *this;
}

DiscreteMeasurementAdder& DiscreteMeasurementAdder::setId(const std::string& id) {
    m_id = id;

    return *this;
}

DiscreteMeasurementAdder& DiscreteMeasurementAdder::setTapChanger(const DiscreteMeasurement::TapChanger& tapChanger) {
    m_tapChanger = tapChanger;

    return *this;
}

DiscreteMeasurementAdder& DiscreteMeasurementAdder::setType(const DiscreteMeasurement::Type& type) {
    m_type = type;

    return *this;
}

DiscreteMeasurementAdder& DiscreteMeasurementAdder::setValid(bool valid) {
    m_valid = valid;

    return *this;
}

DiscreteMeasurementAdder& DiscreteMeasurementAdder::setValue(const std::string& value) {
    m_value = value;

    return *this;
}

DiscreteMeasurementAdder& DiscreteMeasurementAdder::setValue(const char* value) {
    m_value = std::string(value);

    return *this;
}

DiscreteMeasurementAdder& DiscreteMeasurementAdder::setValue(bool value) {
    m_value = value;

    return *this;
}

DiscreteMeasurementAdder& DiscreteMeasurementAdder::setValue(int value) {
    m_value = value;

    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
