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

    virtual double getAngle() const = 0;

    virtual const Connectable& getConnectable() const = 0;

    virtual Connectable& getConnectable() = 0;

    virtual double getP() const = 0;

    virtual double getQ() const = 0;

    virtual stdcxx::optional<Branch::Side> getSide() const = 0;

    virtual double getV() const = 0;

    virtual const VoltageLevel& getVoltageLevel() const = 0;

    virtual VoltageLevel& getVoltageLevel() = 0;

protected:
    static double getAngle(const stdcxx::CReference<Bus>& bus);

    static double getV(const stdcxx::CReference<Bus>& bus);
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BOUNDARY_HPP
