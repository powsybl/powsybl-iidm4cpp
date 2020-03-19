/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DANGLINGLINEADDER_HPP
#define POWSYBL_IIDM_DANGLINGLINEADDER_HPP

#include <powsybl/iidm/InjectionAdder.hpp>
#include <powsybl/stdcxx/export.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class DanglingLine;
class Network;
class VoltageLevel;

class IIDM_DECLSPEC DanglingLineAdder : public InjectionAdder<DanglingLineAdder> {
public:
    ~DanglingLineAdder() noexcept override = default;

    DanglingLine& add();

    DanglingLineAdder& setB(double b);

    DanglingLineAdder& setG(double g);

    DanglingLineAdder& setP0(double p0);

    DanglingLineAdder& setQ0(double q0);

    DanglingLineAdder& setR(double r);

    DanglingLineAdder& setUcteXnodeCode(const std::string& ucteXnodeCode);

    DanglingLineAdder& setX(double x);

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit DanglingLineAdder(VoltageLevel& voltageLevel);

    friend class VoltageLevel;

private:
    double m_r = stdcxx::nan();

    double m_x = stdcxx::nan();

    double m_g = stdcxx::nan();

    double m_b = stdcxx::nan();

    double m_p0 = stdcxx::nan();

    double m_q0 = stdcxx::nan();

    std::string m_ucteXnodeCode;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DANGLINGLINEADDER_HPP
