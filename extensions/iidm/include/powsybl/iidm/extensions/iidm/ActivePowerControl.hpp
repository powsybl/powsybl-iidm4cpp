/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROL_HPP

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/extensions/iidm/IidmExport.hpp>

namespace powsybl {

namespace iidm {

class Battery;
class Generator;

namespace extensions {

namespace iidm {

class EXT_IIDM_EXPORT ActivePowerControl : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    ActivePowerControl(Battery& battery, bool participate, double droop);

    ActivePowerControl(Generator& generator, bool participate, double droop);

    ~ActivePowerControl() noexcept override = default;

    double getDroop() const;

    bool isParticipate() const;

    ActivePowerControl& setDroop(double droop);

    ActivePowerControl& setParticipate(bool participate);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    bool m_participate;

    double m_droop;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROL_HPP
