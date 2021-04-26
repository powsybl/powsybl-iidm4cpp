/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_HALFLINEBOUNDARY_HPP
#define POWSYBL_IIDM_HALFLINEBOUNDARY_HPP

#include <powsybl/iidm/Boundary.hpp>

namespace powsybl {

namespace iidm {

namespace tie_line {

class HalfLine;

}  // namespace tie_line

class Bus;
class Terminal;
class TieLine;

namespace half_line {

class Boundary : public iidm::Boundary {
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
    Boundary(tie_line::HalfLine& halfLine, const Branch::Side& side);

    Boundary(const Boundary&) = delete;

    Boundary(Boundary&&) = delete;

    ~Boundary() noexcept override = default;

    Boundary& operator=(const Boundary&) = delete;

    Boundary& operator=(Boundary&&) noexcept = delete;

private:
    const TieLine& getTieLine() const;

    TieLine& getTieLine();

private:
    tie_line::HalfLine& m_parent;

    Branch::Side m_side;
};

}  // namespace half_line

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_HALFLINEBOUNDARY_HPP

