/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/MeasurementAdder.hpp>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/extensions/iidm/MeasurementValidationUtil.hpp>
#include <powsybl/iidm/extensions/iidm/Measurements.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

MeasurementAdder::MeasurementAdder(Measurements& measurements) :
    m_measurements(measurements) {
}

Measurement& MeasurementAdder::add() {
    MeasurementValidationUtil::checkId(m_id, m_measurements);
    if (!m_type) {
        throw PowsyblException("Measurement type can not be null");
    }
    MeasurementValidationUtil::checkValue(m_value, m_valid);
    MeasurementValidationUtil::checkSide(*m_type, m_side, m_measurements.getExtendable<Connectable>().get());
    std::unique_ptr<Measurement> measurement = stdcxx::make_unique<Measurement>(m_measurements, m_id, *m_type, m_properties, m_value, m_standardDeviation, m_valid, m_side);
    m_measurements.add(std::move(measurement));
    return m_measurements.getMeasurement(m_id).get();
}

MeasurementAdder& MeasurementAdder::putProperty(const std::string& name, const std::string&  property) {
    m_properties[name] = property;
    return *this;
}

MeasurementAdder& MeasurementAdder::setId(const std::string& id) {
    m_id = id;
    return *this;
}

MeasurementAdder& MeasurementAdder::setSide(const Measurement::Side& side) {
    m_side = side;
    return *this;
}

MeasurementAdder& MeasurementAdder::setStandardDeviation(double standardDeviation) {
    m_standardDeviation = standardDeviation;
    return *this;
}

MeasurementAdder& MeasurementAdder::setType(const Measurement::Type& type) {
    m_type = type;
    return *this;
}

MeasurementAdder& MeasurementAdder::setValid(bool valid) {
    m_valid = valid;
    return *this;
}

MeasurementAdder& MeasurementAdder::setValue(double value) {
    m_value = value;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
