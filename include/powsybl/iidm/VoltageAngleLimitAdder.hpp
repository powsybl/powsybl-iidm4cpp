/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGEANGLELIMITADDER_HPP
#define POWSYBL_IIDM_VOLTAGEANGLELIMITADDER_HPP

#include <powsybl/iidm/OperationalLimitsAdder.hpp>
#include <powsybl/iidm/VoltageAngleLimit.hpp>

#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Network;

class VoltageAngleLimitAdder : public OperationalLimitsAdder<VoltageAngleLimit> {

public:
    explicit VoltageAngleLimitAdder(Network& network);

    VoltageAngleLimitAdder(const VoltageAngleLimitAdder&) = default;

    ~VoltageAngleLimitAdder() noexcept override = default;

    VoltageAngleLimitAdder& setId(const std::string& id);

    VoltageAngleLimitAdder& from(const stdcxx::Reference<Terminal>& terminal);
    VoltageAngleLimitAdder& to(const stdcxx::Reference<Terminal>& terminal);

    VoltageAngleLimitAdder& setLowLimit(double lowLimit);
    VoltageAngleLimitAdder& setHighLimit(double highLimit);

    VoltageAngleLimit& add() override;

private:

    Network& m_network;

    std::string m_id;

    stdcxx::Reference<Terminal> m_terminalFrom;
    stdcxx::Reference<Terminal> m_terminalTo;

    double m_lowLimit = stdcxx::nan();
    double m_highLimit = stdcxx::nan();

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VOLTAGEANGLELIMITADDER_HPP
