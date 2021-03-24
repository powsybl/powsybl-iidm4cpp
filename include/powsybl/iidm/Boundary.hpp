/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BOUNDARY_HPP
#define POWSYBL_IIDM_BOUNDARY_HPP

namespace powsybl {

namespace iidm {

class Boundary {
public:
    Boundary() = default;

    Boundary(const Boundary&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of std::function marked noexcept (see half_line::Boundary)
    Boundary(Boundary&&) = default;  // NOSONAR

    virtual ~Boundary() noexcept = default;

    Boundary& operator=(const Boundary&) = default;

    Boundary& operator=(Boundary&&) noexcept = default;

    virtual double getAngle() const = 0;

    virtual double getP() const = 0;

    virtual double getQ() const = 0;

    virtual double getV() const = 0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BOUNDARY_HPP
