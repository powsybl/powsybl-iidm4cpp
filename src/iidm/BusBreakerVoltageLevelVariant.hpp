/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELVARIANT_HPP
#define POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELVARIANT_HPP

#include <powsybl/iidm/Variant.hpp>

#include "BusBreakerVoltageLevelTopology.hpp"

namespace powsybl {

namespace iidm {

namespace bus_breaker_voltage_level {

class VariantImpl : public Variant<BusBreakerVoltageLevel, VariantImpl> {
public: // Variant
    std::unique_ptr<VariantImpl> copy() const override;

public:
    explicit VariantImpl(BusBreakerVoltageLevel& voltageLevel);

    ~VariantImpl() noexcept override = default;

    CalculatedBusTopology& getCalculatedBusTopology();

private:
    CalculatedBusTopology m_calculatedBusTopology;
};

using VariantArray = iidm::VariantArray<BusBreakerVoltageLevel, VariantImpl>;

}  // namespace bus_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELVARIANT_HPP
