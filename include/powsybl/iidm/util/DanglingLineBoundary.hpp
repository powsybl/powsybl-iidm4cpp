/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DANGLINGLINEBOUNDARY_HPP
#define POWSYBL_IIDM_DANGLINGLINEBOUNDARY_HPP

#include <powsybl/iidm/Boundary.hpp>

#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class DanglingLine;

namespace util {

namespace dangling_line {

class Boundary : public iidm::Boundary {
    // for SV use: side represents the network side, that is always
    // Side.ONE for a dangling line.
    // DanglingLine model has shunt admittance on network side only, hence splitShuntAdmittance argument in SV methods must be set to false

public:  // iidm::Boundary
    double getAngle() const override;

    const DanglingLine& getDanglingLine() const override;

    DanglingLine& getDanglingLine() override;

    double getP() const override;

    double getQ() const override;

    double getV() const override;

    const VoltageLevel& getNetworkSideVoltageLevel() const override;

    VoltageLevel& getNetworkSideVoltageLevel() override;

public:
    explicit Boundary(DanglingLine& parent);

    Boundary(const Boundary&) = delete;

    Boundary(Boundary&&) noexcept = delete;

    ~Boundary() noexcept override = default;

    Boundary& operator=(const Boundary&) = delete;

    Boundary& operator=(Boundary&&) noexcept = delete;

private:
    DanglingLine& m_parent;
};

}  // namespace dangling_line

}  // namespace util

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DANGLINGLINEBOUNDARY_HPP
