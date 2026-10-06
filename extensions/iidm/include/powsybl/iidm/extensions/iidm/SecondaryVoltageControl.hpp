/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_SECONDARYVOLTAGECONTROL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_SECONDARYVOLTAGECONTROL_HPP

#include <powsybl/iidm/Extension.hpp>

#include <powsybl/iidm/extensions/iidm/ControlZone.hpp>

namespace powsybl {

namespace iidm {

class Network;

namespace extensions {

namespace iidm {

class SecondaryVoltageControl : public Extension {

public:
// Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

    const std::vector<ControlZone>& getControlZones() const;

private:
// Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

    SecondaryVoltageControl(Network& network, const std::vector<ControlZone>& controlZones);

    friend class SecondaryVoltageControlAdder;

    std::vector<ControlZone> m_controlZones;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_SECONDARYVOLTAGECONTROL_HPP
