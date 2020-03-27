/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BATTERYADDER_HPP
#define POWSYBL_IIDM_BATTERYADDER_HPP

#include <powsybl/iidm/InjectionAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class Battery;
class Network;
class VoltageLevel;

class BatteryAdder : public InjectionAdder<BatteryAdder> {
public:
    ~BatteryAdder() noexcept override = default;

    Battery& add();

    BatteryAdder& setMaxP(double maxP);

    BatteryAdder& setMinP(double minP);

    BatteryAdder& setP0(double p0);

    BatteryAdder& setQ0(double q0);

protected: // IdentifiableAdder
    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit BatteryAdder(VoltageLevel& voltageLevel);

    friend class VoltageLevel;

private:
    VoltageLevel& m_voltageLevel;

    double m_p0 = stdcxx::nan();

    double m_q0 = stdcxx::nan();

    double m_minP = stdcxx::nan();

    double m_maxP = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BATTERYADDER_HPP
