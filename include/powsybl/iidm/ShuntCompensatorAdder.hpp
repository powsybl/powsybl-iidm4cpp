/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SHUNTCOMPENSATORADDER_HPP
#define POWSYBL_IIDM_SHUNTCOMPENSATORADDER_HPP

#include <powsybl/iidm/InjectionAdder.hpp>
#include <powsybl/iidm/ShuntCompensatorLinearModelAdder.hpp>
#include <powsybl/iidm/ShuntCompensatorModelAdder.hpp>
#include <powsybl/iidm/ShuntCompensatorNonLinearModelAdder.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class Network;
class ShuntCompensator;
class VoltageLevel;

class ShuntCompensatorAdder : public InjectionAdder<ShuntCompensatorAdder> {
public:
    using ShuntCompensatorLinearModelAdder = shunt_compensator::ShuntCompensatorLinearModelAdder;

    using ShuntCompensatorNonLinearModelAdder = shunt_compensator::ShuntCompensatorNonLinearModelAdder;

public:
    ShuntCompensatorAdder(ShuntCompensatorAdder&&) = default;

    ~ShuntCompensatorAdder() noexcept override = default;

    ShuntCompensator& add();

    ShuntCompensatorLinearModelAdder newLinearModel();

    ShuntCompensatorNonLinearModelAdder newNonLinearModel();

    ShuntCompensatorAdder& setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal);

    ShuntCompensatorAdder& setSectionCount(unsigned long sectionCount);

    ShuntCompensatorAdder& setTargetDeadband(double targetDeadband);

    ShuntCompensatorAdder& setTargetV(double targetV);

    ShuntCompensatorAdder& setVoltageRegulatorOn(bool voltageRegulatorOn);

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit ShuntCompensatorAdder(VoltageLevel& voltageLevel);

    void setShuntCompensatorModelBuilder(std::unique_ptr<ShuntCompensatorModelAdder>&& adder);

    friend class VoltageLevel;

    friend class shunt_compensator::ShuntCompensatorLinearModelAdder;

    friend class shunt_compensator::ShuntCompensatorNonLinearModelAdder;

private:
    stdcxx::optional<unsigned long> m_sectionCount;

    double m_targetDeadband = stdcxx::nan();

    double m_targetV = stdcxx::nan();

    stdcxx::Reference<Terminal> m_regulatingTerminal;

    bool m_voltageRegulatorOn = false;

    std::unique_ptr<ShuntCompensatorModelAdder> m_modelBuilder;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATORADDER_HPP
