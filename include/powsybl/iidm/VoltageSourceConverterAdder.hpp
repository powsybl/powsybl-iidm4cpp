/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGESOURCECONVERTERADDER_HPP
#define POWSYBL_IIDM_VOLTAGESOURCECONVERTERADDER_HPP

#include <powsybl/iidm/AcDcConverterAdder.hpp>
#include <powsybl/iidm/VoltageSourceConverter.hpp>

namespace powsybl {

namespace iidm {

class VoltageSourceConverterAdder : public AcDcConverterAdder<VoltageSourceConverter, VoltageSourceConverterAdder> {

public:
    explicit VoltageSourceConverterAdder(VoltageLevel& voltageLevel);
    ~VoltageSourceConverterAdder() noexcept override = default;

    VoltageSourceConverter& add() override;

    VoltageSourceConverterAdder& setVoltageRegulatorOn(bool voltageRegulatorOn);

    VoltageSourceConverterAdder& setVoltageSetpoint(double voltageSetpoint);

    VoltageSourceConverterAdder& setReactivePowerSetpoint(double reactivePowerSetpoint);

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    bool m_voltageRegulatorOn = false;
    double m_reactivePowerSetpoint = stdcxx::nan();
    double m_voltageSetpoint = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VOLTAGESOURCECONVERTERADDER_HPP
