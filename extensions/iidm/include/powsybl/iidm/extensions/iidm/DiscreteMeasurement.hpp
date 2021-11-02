/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENT_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENT_HPP

#include <map>
#include <string>

#include <boost/any.hpp>

#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class DiscreteMeasurements;

class DiscreteMeasurement {
public:
    /**
     * Specify what is measured.
     */
    enum class Type : unsigned char {
        TAP_POSITION,
        SWITCH_POSITION,
        SHUNT_COMPENSATOR_SECTION,
        OTHER
    };

    /**
     * If it is the modelization of a tap position (or any discrete measurement on a tap changer),
     * can explicit which tap changer of the transformer it is applied on.
     */
    enum class TapChanger : unsigned char {
        RATIO_TAP_CHANGER,
        PHASE_TAP_CHANGER,
        RATIO_TAP_CHANGER_1,
        PHASE_TAP_CHANGER_1,
        RATIO_TAP_CHANGER_2,
        PHASE_TAP_CHANGER_2,
        RATIO_TAP_CHANGER_3,
        PHASE_TAP_CHANGER_3
    };

    /**
     * Explicit what type of discrete value is used for this measurement (boolean, int or string).
     */
    enum class ValueType : unsigned char {
        BOOLEAN,
        INT,
        STRING
    };

public:
    DiscreteMeasurement(DiscreteMeasurements& discreteMeasurements, const std::string& id, const Type& type,
                        const stdcxx::optional<TapChanger>& tapChanger, const std::map<std::string, std::string>& properties,
                        const ValueType& valueType, const boost::any& value, bool valid);

    /**
     * Get ID of the measurement if it exists. It is optional (can be empty).
     */
    const std::string& getId() const;

    /**
     * Return property value associated to the given property name if it exists, else return an empty string.
     */
    std::string getProperty(const std::string& name) const;

    /**
     * Get all the property names with values applied to this measurement.
     */
    stdcxx::const_range<std::string> getPropertyNames() const;

    /**
     * Get the tap changer the measurement is applied on if necessary (see {@link TapChanger}).
     * Must be an invalid reference if the measurement is not applied to a tap changer. Throw an exception if the extended equipment is not a transformer.
     * Must be a valid reference if the measurement is not applied to a tap changer, else throw an exception.
     */
    stdcxx::optional<TapChanger> getTapChanger() const;

    /**
     * Get the type of the measurement (specify what it is measured, see {@link Type}).
     */
    const Type& getType() const;

    /**
     * Get value as boolean if the type of measured discrete value is BOOLEAN (see {@link ValueType}).
     * Else throw an exception.
     */
    bool getValueAsBoolean() const;

    /**
     * Get value as int if the type of measured discrete value is INT (see {@link ValueType}).
     * Else throw an exception.
     */
    int getValueAsInt() const;

    /**
     * Get value as String if the type of measured discrete value is STRING (see {@link ValueType}).
     * Else throw an exception.
     */
    std::string getValueAsString() const;

    /**
     * Get the type of measured discrete value (int, boolean or string, see {@link ValueType}).
     */
    const ValueType& getValueType() const;

    /**
     * Get validity status of the measurement.
     * If it is true (i.e. the measurement is valid), the discrete measured value can not be null.
     */
    bool isValid() const;

    /**
     * Put a given property value associated with a given property name.
     */
    DiscreteMeasurement& putProperty(const std::string& name, const std::string& property);

    /**
     * Remove the measurement from the equipment.
     */
    void remove();

    /**
     * Remove the property value associated with the given property name if it exists. Else, do nothing.
     */
    DiscreteMeasurement& removeProperty(const std::string& name);

    /**
     * Set validity status of the measurement.
     * If it is true (i.e. the measurement is valid), the discrete measured value can not be null.
     */
    DiscreteMeasurement& setValid(bool valid);

    /**
     * Set the discrete measured value and set the value type as STRING (see {@link ValueType}).
     * Can not be empty if the measurement is valid.
     */
    DiscreteMeasurement& setValue(const std::string& value);

    DiscreteMeasurement& setValue(const char* value);

    /**
     * Set the discrete measured value and set the value type as INT (see {@link ValueType}).
     */
    DiscreteMeasurement& setValue(int value);

    /**
     * Set the discrete measured value and set the value type as BOOLEAN (see {@link ValueType}).
     */
    DiscreteMeasurement& setValue(bool value);

private:
    DiscreteMeasurements& m_discreteMeasurements;

    std::string m_id;

    Type m_type;

    stdcxx::optional<TapChanger> m_tapChanger;

    std::map<std::string, std::string> m_properties;

    ValueType m_valueType;

    boost::any m_value;

    bool m_valid;
};

std::ostream& operator<<(std::ostream& stream, const DiscreteMeasurement::Type& type);

std::ostream& operator<<(std::ostream& stream, const DiscreteMeasurement::TapChanger& tapChanger);

std::ostream& operator<<(std::ostream& stream, const DiscreteMeasurement::ValueType& valueType);

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENT_HPP
