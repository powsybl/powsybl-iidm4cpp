/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VSCCONVERTERSTATIONADDER_HPP
#define POWSYBL_IIDM_VSCCONVERTERSTATIONADDER_HPP

#include <powsybl/iidm/HvdcConverterStationAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class VscConverterStationAdder : public HvdcConverterStationAdder<VscConverterStationAdder> {
public:
    ~VscConverterStationAdder() noexcept override = default;

    VscConverterStation& add();

    VscConverterStationAdder& setReactivePowerSetpoint(double reactivePowerSetpoint);

    VscConverterStationAdder& setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal);

    VscConverterStationAdder& setVoltageRegulatorOn(bool voltageRegulatorOn);

    VscConverterStationAdder& setVoltageSetpoint(double voltageSetpoint);

protected: // HvdcConverterStationAdder
    void validate() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit VscConverterStationAdder(VoltageLevel& voltageLevel);

    friend class VoltageLevel;

private:
    stdcxx::optional<bool> m_voltageRegulatorOn;

    double m_reactivePowerSetpoint = stdcxx::nan();

    double m_voltageSetpoint = stdcxx::nan();

    stdcxx::Reference<Terminal> m_regulatingTerminal;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VSCCONVERTERSTATIONADDER_HPP
