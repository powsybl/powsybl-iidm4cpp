/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_SECONDARYVOLTAGECONTROLADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_SECONDARYVOLTAGECONTROLADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>
#include <powsybl/iidm/extensions/iidm/SecondaryVoltageControl.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ControlZoneAdder;

class SecondaryVoltageControlAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit SecondaryVoltageControlAdder(Extendable& extendable);

    ~SecondaryVoltageControlAdder() noexcept override = default;
    SecondaryVoltageControlAdder(const SecondaryVoltageControlAdder&) = default;
    SecondaryVoltageControlAdder(SecondaryVoltageControlAdder&&) = default;
    SecondaryVoltageControlAdder& operator=(const SecondaryVoltageControlAdder&) = delete;
    SecondaryVoltageControlAdder& operator=(SecondaryVoltageControlAdder&&) = delete;

    //emplace the given controlZone in m_controlZones
    SecondaryVoltageControlAdder& addControlZone(const ControlZone& controlZone);

    //retrieve a new control zone adder
    ControlZoneAdder newControlZone();

protected:
    //ExtensionAdder
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:

    std::vector<ControlZone> m_controlZones;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_SECONDARYVOLTAGECONTROLADDER_HPP