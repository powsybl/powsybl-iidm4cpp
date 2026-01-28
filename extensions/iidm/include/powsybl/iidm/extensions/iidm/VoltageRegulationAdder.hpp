/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEREGULATIONADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEREGULATIONADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <vector>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class VoltageRegulationAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit VoltageRegulationAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    VoltageRegulationAdder(const VoltageRegulationAdder&) = default;

    /**
     * Move constructor
     */
    VoltageRegulationAdder(VoltageRegulationAdder&&) noexcept = default;

    /**
     * Destructor
     */
    ~VoltageRegulationAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    VoltageRegulationAdder& operator=(const VoltageRegulationAdder&) = delete;

    /**
     * Move assignment operator
     */
    VoltageRegulationAdder& operator=(VoltageRegulationAdder&&) = delete;

    VoltageRegulationAdder& withRegulatingTerminal(const stdcxx::Reference<Terminal>& terminal);
    VoltageRegulationAdder& withTargetV(double targetV);
    VoltageRegulationAdder& withVoltageRegulatorOn(bool voltageRegulatorOn);

private:  // ExtensionAdder
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

    stdcxx::Reference<Terminal> m_regulatingTerminal;
    double m_targetV = stdcxx::nan();
    stdcxx::optional<bool> m_voltageRegulatorOn;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEREGULATIONADDER_HPP
