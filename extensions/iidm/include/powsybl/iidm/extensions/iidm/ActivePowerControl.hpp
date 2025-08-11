/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROL_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Battery;
class Generator;

namespace extensions {

namespace iidm {

class ActivePowerControl : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    ~ActivePowerControl() noexcept override = default;

    double getDroop() const;

    bool isParticipate() const;

    double getParticipationFactor() const;

    ActivePowerControl& setDroop(double droop);

    ActivePowerControl& setParticipate(bool participate);

    ActivePowerControl& setParticipationFactor(double participationFactor);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    ActivePowerControl(Battery& battery, bool participate, double droop, double participationFactor);

    ActivePowerControl(Generator& generator, bool participate, double droop, double participationFactor);

    friend class ActivePowerControlAdder;

private:
    bool m_participate;

    double m_droop;

    double m_participationFactor;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROL_HPP
