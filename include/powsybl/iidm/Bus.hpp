/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUS_HPP
#define POWSYBL_IIDM_BUS_HPP

#include <functional>
#include <string>
#include <vector>

#include <powsybl/iidm/Identifiable.hpp>

namespace powsybl {

namespace iidm {

class Terminal;
class VoltageLevel;

class Bus : public Identifiable {
public:
    ~Bus() noexcept override = default;

    // TODO(mathbagu): begin, cbegin, cend, end to iterator over all connected equipments

    virtual double getAngle() const = 0;

    virtual unsigned long getConnectedTerminalCount() const = 0;

    virtual std::vector<std::reference_wrapper<Terminal> > getConnectedTerminals() const = 0;

    virtual double getV() const = 0;

    virtual VoltageLevel& getVoltageLevel() const = 0;

    virtual Bus& setAngle(double angle) = 0;

    virtual Bus& setV(double v) = 0;

protected:
    explicit Bus(const std::string& id);

    Bus(const std::string& id, const std::string& name);

private: // Identifiable
    const std::string& getTypeDescription() const override;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUS_HPP
