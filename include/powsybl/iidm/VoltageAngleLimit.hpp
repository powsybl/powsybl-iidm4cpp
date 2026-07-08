/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGEANGLELIMIT_HPP
#define POWSYBL_IIDM_VOLTAGEANGLELIMIT_HPP

#include <powsybl/iidm/OperationalLimits.hpp>

#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

// class OperationalLimitsOwner;
class Network;
class Terminal;

class VoltageAngleLimit : public OperationalLimits {

public:

    ~VoltageAngleLimit() noexcept = default;

    //OperationalLimits
    const LimitType& getLimitType() const override;
    void remove() override;

    //VoltageAngleLimit:
    const std::string& getId() const;

    const Terminal& getTerminalFrom() const;
    Terminal& getTerminalFrom();

    const Terminal& getTerminalTo() const;
    Terminal& getTerminalTo();

    double getLowLimit() const;
    double getHighLimit() const;

private:

    VoltageAngleLimit(Network& network, const std::string& id, Terminal& terminalFrom, Terminal& terminalTo, double lowLimit, double highLimit);
    friend class VoltageAngleLimitAdder;

    stdcxx::Reference<Network> m_network;
    friend class Network;

    std::string m_id;

    std::reference_wrapper<Terminal> m_fromTerminal;
    std::reference_wrapper<Terminal> m_toTerminal;

    double m_lowLimit;
    double m_highLimit;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VOLTAGEANGLELIMIT_HPP
