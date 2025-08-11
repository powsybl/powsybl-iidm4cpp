/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_ABSTRACTHALFLINEBOUNDARY_HPP
#define POWSYBL_IIDM_UTIL_ABSTRACTHALFLINEBOUNDARY_HPP

#include <powsybl/iidm/Boundary.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class Terminal;
class TieLine;

namespace tie_line {

class HalfLine;

}  // namespace tie_line

namespace util {

namespace half_line {

class AbstractHalfLineBoundary : public Boundary {
    // side represents the network side.
    // side here is Side.ONE for the half line 1 of a tie line.
    // side is Side.TWO for the half line 2 of a tie line.

public:  // iidm::Boundary
    double getAngle() const override;

    double getP() const override;

    double getQ() const override;

    stdcxx::optional<Branch::Side> getSide() const override;

    double getV() const override;

    const VoltageLevel& getNetworkSideVoltageLevel() const override;

    VoltageLevel& getNetworkSideVoltageLevel() override;

public:
    AbstractHalfLineBoundary(tie_line::HalfLine& halfLine, const Branch::Side& side);

    AbstractHalfLineBoundary(const Boundary&) = delete;

    AbstractHalfLineBoundary(Boundary&&) = delete;

    ~AbstractHalfLineBoundary() noexcept override = default;

    AbstractHalfLineBoundary& operator=(const AbstractHalfLineBoundary&) = delete;

    AbstractHalfLineBoundary& operator=(AbstractHalfLineBoundary&&) noexcept = delete;

protected:
    const tie_line::HalfLine& getParent() const;

    tie_line::HalfLine& getParent();

private:
    const TieLine& getTieLine() const;

    TieLine& getTieLine();

private:
    tie_line::HalfLine& m_parent;

    Branch::Side m_side;
};

}  // namespace half_line

}  // namespace util

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_ABSTRACTHALFLINEBOUNDARY_HPP
