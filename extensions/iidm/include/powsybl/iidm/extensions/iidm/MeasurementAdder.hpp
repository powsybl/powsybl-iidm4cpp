/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENTADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENTADDER_HPP

#include <map>
#include <string>

#include <powsybl/iidm/extensions/iidm/Measurement.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class Measurements;

class MeasurementAdder {
public:
    /**
     * Constructor
     */
    explicit MeasurementAdder(Measurements& measurements);

    /**
     * Copy constructor
     */
    MeasurementAdder(const MeasurementAdder&) = default;

    /**
     * Move constructor
     */
    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of std::map is not marked noexcept
    MeasurementAdder(MeasurementAdder&&) = default;  // NOSONAR

    /**
     * Destructor
     */
    ~MeasurementAdder() noexcept = default;

    /**
     * Copy assignment operator
     */
    MeasurementAdder& operator=(const MeasurementAdder&) = delete;

    /**
     * Move assignment operator
     */
    MeasurementAdder& operator=(MeasurementAdder&&) = delete;

    Measurement& add();

    MeasurementAdder& putProperty(const std::string& name, const std::string& value);

    MeasurementAdder& setId(const std::string& id);

    MeasurementAdder& setSide(const Measurement::Side& side);

    MeasurementAdder& setStandardDeviation(double standardDeviation);

    MeasurementAdder& setType(const Measurement::Type& type);

    MeasurementAdder& setValid(bool valid);

    MeasurementAdder& setValue(double value);

private:
    Measurements& m_measurements;

    std::map<std::string, std::string> m_properties;

    std::string m_id;

    stdcxx::optional<Measurement::Type> m_type;

    double m_value = stdcxx::nan();

    double m_standardDeviation = stdcxx::nan();

    bool m_valid = true;

    stdcxx::optional<Measurement::Side> m_side;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENTADDER_HPP
