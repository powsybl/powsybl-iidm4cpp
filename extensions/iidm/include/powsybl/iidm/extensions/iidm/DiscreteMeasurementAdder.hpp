/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTADDER_HPP

#include <string>

#include <powsybl/iidm/extensions/iidm/DiscreteMeasurement.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class DiscreteMeasurementAdder {
public:
    /**
     * Constructor
     */
    explicit DiscreteMeasurementAdder(DiscreteMeasurements& discreteMeasurements);

    /**
     * Copy constructor
     */
    DiscreteMeasurementAdder(const DiscreteMeasurementAdder&) = default;

    /**
     * Move constructor
     */
    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of std::map is not marked noexcept
    DiscreteMeasurementAdder(DiscreteMeasurementAdder&&) = default;  // NOSONAR

    /**
     * Destructor
     */
    ~DiscreteMeasurementAdder() noexcept = default;

    /**
     * Copy assignment operator
     */
    DiscreteMeasurementAdder& operator=(const DiscreteMeasurementAdder&) = delete;

    /**
     * Move assignment operator
     */
    DiscreteMeasurementAdder& operator=(DiscreteMeasurementAdder&&) = delete;

    DiscreteMeasurement& add();

    DiscreteMeasurementAdder& putProperty(const std::string& name, const std::string& value);

    DiscreteMeasurementAdder& setId(const std::string& id);

    DiscreteMeasurementAdder& setTapChanger(const DiscreteMeasurement::TapChanger& tapChanger);

    DiscreteMeasurementAdder& setType(const DiscreteMeasurement::Type& type);

    DiscreteMeasurementAdder& setValid(bool valid);

    DiscreteMeasurementAdder& setValue(const std::string& value);

    DiscreteMeasurementAdder& setValue(const char* value);

    DiscreteMeasurementAdder& setValue(bool value);

    DiscreteMeasurementAdder& setValue(int value);

    DiscreteMeasurementAdder& setEnsureIdUnicity(bool idUnicity);

private:
    DiscreteMeasurements& m_discreteMeasurements;

    std::map<std::string, std::string> m_properties;

    std::string m_id;

    DiscreteMeasurement::Type m_type;

    stdcxx::optional<DiscreteMeasurement::TapChanger> m_tapChanger;

    boost::any m_value;

    bool m_valid = true;

    bool m_idUnicity = false;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTADDER_HPP
