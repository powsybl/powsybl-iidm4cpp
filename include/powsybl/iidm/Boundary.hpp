/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BOUNDARY_HPP
#define POWSYBL_IIDM_BOUNDARY_HPP

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class Connectable;
class VoltageLevel;

class Boundary {
public:
    Boundary() = default;

    Boundary(const Boundary&) = default;

    Boundary(Boundary&&) noexcept = default;

    virtual ~Boundary() noexcept = default;

    Boundary& operator=(const Boundary&) = default;

    Boundary& operator=(Boundary&&) noexcept = default;

    /**
     * Get the angle of the boundary fictitious bus.
     */
    virtual double getAngle() const = 0;

    /**
     * Get the equipment the boundary is associated to.
     */
    virtual const Connectable& getConnectable() const = 0;

    /**
     * Get the equipment the boundary is associated to.
     */
    virtual Connectable& getConnectable() = 0;

    /**
     * Get the active power at the fictitious terminal going from the boundary fictitious bus to the network.
     */
    virtual double getP() const = 0;

    /**
     * Get the reactive power at the fictitious terminal going from the boundary fictitious bus to the network.
     */
    virtual double getQ() const = 0;

    /**
     * A Boundary could be associated with one side of a branch to determine P and Q.
     * Get the branch side the boundary refers to.
     */
    virtual stdcxx::optional<Branch::Side> getSide() const = 0;

    /**
     * Get the voltage of the boundary fictitious bus.
     */
    virtual double getV() const = 0;

    /**
     * Get the voltage level at network side.
     */
    virtual const VoltageLevel& getVoltageLevel() const = 0;

    /**
     * Get the voltage level at network side.
     */
    virtual VoltageLevel& getVoltageLevel() = 0;

protected:
    static double getAngle(const stdcxx::CReference<Bus>& bus);

    static double getV(const stdcxx::CReference<Bus>& bus);
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BOUNDARY_HPP
