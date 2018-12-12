/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELVARIANT_HPP
#define POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELVARIANT_HPP

#include "NodeBreakerVoltageLevelTopology.hpp"
#include "Variant.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

class VariantImpl : public Variant<VariantImpl> {
public: // Variant
    std::unique_ptr<VariantImpl> copy() const override;

public:
    explicit VariantImpl(NodeBreakerVoltageLevel& voltageLevel);

    ~VariantImpl() noexcept override = default;

    CalculatedBusBreakerTopology& getCalculatedBusBreakerTopology();

    CalculatedBusTopology& getCalculatedBusTopology();

private:
    NodeBreakerVoltageLevel& m_voltageLevel;

    CalculatedBusBreakerTopology m_calculatedBusBreakerTopology;

    CalculatedBusTopology m_calculatedBusTopology;
};

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELVARIANT_HPP
