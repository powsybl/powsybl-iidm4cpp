/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGELEVEL_HPP
#define POWSYBL_IIDM_VOLTAGELEVEL_HPP

#include <functional>

#include <powsybl/iidm/Container.hpp>

namespace powsybl {

namespace iidm {

class Network;
class Substation;
class VoltageLevelAdder;

class VoltageLevel : public Container {
public:
    virtual ~VoltageLevel() = default;

    double getHighVoltageLimit() const;

    double getLowVoltageLimit() const;

    double getNominalVoltage() const;

    const Network& getNetwork() const;

    Network& getNetwork();

    const Substation& getSubstation() const;

    Substation& getSubstation();

    VoltageLevel& setHighVoltageLimit(double highVoltageLimit);

    VoltageLevel& setLowVoltageLimit(double lowVoltageLimit);

    VoltageLevel& setNominalVoltage(double nominalVoltage);

protected:
    const std::string& getTypeDescription() const override;

protected:
    VoltageLevel(const std::string& id, const std::string& name, Substation& substation,
                 double nominalVoltage, double lowVoltageLimit, double highVoltagelimit);

private:
    std::reference_wrapper<Substation> m_substation;

    double m_highVoltageLimit;

    double m_lowVoltageLimit;

    double m_nominalVoltage;
};

}

}

#endif  // POWSYBL_IIDM_VOLTAGELEVEL_HPP
