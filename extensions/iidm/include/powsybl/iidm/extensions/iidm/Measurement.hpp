/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENT_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENT_HPP

#include <iosfwd>
#include <map>
#include <string>

#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class Measurements;

class Measurement {
public:
    /**
     * Specify which side of the equipment the measurement is applied on.
     */
    enum class Side : unsigned char {
        ONE,
        TWO,
        THREE
    };

    /**
     * Specify what is measured.
     */
    enum class Type : unsigned char {
        ANGLE,
        ACTIVE_POWER,
        APPARENT_POWER,
        REACTIVE_POWER,
        CURRENT,
        VOLTAGE,
        FREQUENCY,
        OTHER
    };

public:
    Measurement(Measurements& measurements, const std::string& id, const Measurement::Type& type, const std::map<std::string, std::string>& properties, double value, double standardDeviation, bool valid, const stdcxx::optional<Measurement::Side>& side);

    /**
     * Get ID of the measurement if it exists. It is optional (can be null).
     */
    const std::string& getId() const;

    /**
     * Return property value associated to the given property name if it exists, else return null.
     */
    std::string getProperty(const std::string& name) const;

    /**
     * Get all the property names with values applied to this measurement.
     */
    stdcxx::const_range<std::string> getPropertyNames() const;

    /**
     * Get which side the measurement is applied on (see {@link Side}).
     */
    const stdcxx::optional<Side>& getSide() const;

    /**
     * Get the standard deviation. Return NaN if unspecified.
     */
    double getStandardDeviation() const;

    /**
     * Get the type of measurement (specify what is measured, see {@link Type}).
     */
    const Type& getType() const;

    /**
     * Get measurement value.
     */
    double getValue() const;

    /**
     * Get validity status of the measurement.
     * If it is true (i.e. the measurement is valid), the measured value can not be NaN.
     */
    bool isValid() const;

    /**
     * Put a given property value associated with a given property name.
     */
    Measurement& putProperty(const std::string& name, const std::string& property);

    /**
     * Remove the measurement from the equipment.
     */
    void remove();

    /**
     * Remove the property value associated with the given property name if it exists. Else, do nothing.
     */
    Measurement& removeProperty(const std::string& name);

    /**
     * Set the standard deviation.
     */
    Measurement& setStandardDeviation(double standardDeviation);

    /**
     * Set validity status of the measurement.
     * If it is true (i.e. the measurement is valid), the measured value can not be NaN.
     */
    Measurement& setValid(bool valid);

    /**
     * Set measurement value.
     * Can not be NaN if the measurement is valid.
     */
    Measurement& setValue(double value);

private:
    Measurements& m_measurements;

    std::string m_id;

    Type m_type;

    std::map<std::string, std::string> m_properties;

    stdcxx::optional<Side> m_side;

    double m_value;

    double m_standardDeviation;

    bool m_valid;
};

std::ostream& operator<<(std::ostream& stream, const Measurement::Side& value);

std::ostream& operator<<(std::ostream& stream, const Measurement::Type& value);

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENT_HPP
