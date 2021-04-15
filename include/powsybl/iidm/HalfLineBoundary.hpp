/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_HALFLINEBOUNDARY_HPP
#define POWSYBL_IIDM_HALFLINEBOUNDARY_HPP

#include <functional>

#include <powsybl/iidm/Boundary.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

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
    using TerminalSupplier = std::function<const Terminal&()>;

public:
    Boundary(TieLine& tieLine, const Branch::Side& side, const TerminalSupplier& terminalSupplier);

    Boundary(const Boundary&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of std::function marked noexcept
    Boundary(Boundary&&) = default;  // NOSONAR

    ~Boundary() noexcept override = default;

    Boundary& operator=(const Boundary&) = delete;

    Boundary& operator=(Boundary&&) noexcept = delete;

private:
    TieLine& m_parent;

    Branch::Side m_side;

    TerminalSupplier m_terminalSupplier;
};

}  // namespace half_line

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_HALFLINEBOUNDARY_HPP

