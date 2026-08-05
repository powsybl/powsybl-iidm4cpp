/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BOUNDARYLINEBOUNDARY_HPP
#define POWSYBL_IIDM_BOUNDARYLINEBOUNDARY_HPP

#include <powsybl/iidm/Boundary.hpp>

#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class BoundaryLine;

namespace util {

namespace boundary_line {

class Boundary : public iidm::Boundary {
    // for SV use: side represents the network side, that is always
    // Side.ONE for a boundary line.
    // BoundaryLine model has shunt admittance on network side only, hence splitShuntAdmittance argument in SV methods must be set to false

public:  // iidm::Boundary
    double getAngle() const override;

    const BoundaryLine& getBoundaryLine() const override;

    BoundaryLine& getBoundaryLine() override;

    double getP() const override;

    double getQ() const override;

    double getV() const override;

    double getI() const override;

    const VoltageLevel& getNetworkSideVoltageLevel() const override;

    VoltageLevel& getNetworkSideVoltageLevel() override;

public:
    explicit Boundary(BoundaryLine& parent);

    Boundary(const Boundary&) = delete;

    Boundary(Boundary&&) noexcept = delete;

    ~Boundary() noexcept override = default;

    Boundary& operator=(const Boundary&) = delete;

    Boundary& operator=(Boundary&&) noexcept = delete;

private:
    BoundaryLine& m_parent;
};

}  // namespace boundary_line

}  // namespace util

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BOUNDARYLINEBOUNDARY_HPP
