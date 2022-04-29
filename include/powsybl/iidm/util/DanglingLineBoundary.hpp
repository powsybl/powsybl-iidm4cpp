/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DANGLINGLINEBOUNDARY_HPP
#define POWSYBL_IIDM_DANGLINGLINEBOUNDARY_HPP

#include <powsybl/iidm/Boundary.hpp>
#include <powsybl/iidm/HalfLine.hpp>
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

public:  // iidm::Boundary
    double getAngle() const override;

    const Connectable& getConnectable() const override;

    Connectable& getConnectable() override;

    double getP() const override;

    double getQ() const override;

    stdcxx::optional<Branch::Side> getSide() const override;

    double getV() const override;

    const VoltageLevel& getVoltageLevel() const override;

    VoltageLevel& getVoltageLevel() override;

public:
    explicit Boundary(DanglingLine& parent);

    Boundary(const Boundary&) = delete;

    Boundary(Boundary&&) noexcept = delete;

    ~Boundary() noexcept override = default;

    Boundary& operator=(const Boundary&) = delete;

    Boundary& operator=(Boundary&&) noexcept = delete;

private:
    static bool valid(double p0, double q0);

private:
    DanglingLine& m_parent;
};

}  // namespace dangling_line

}  // namespace util

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DANGLINGLINEBOUNDARY_HPP
