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
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/TopologyKind.hpp>

namespace powsybl {

namespace iidm {

class Network;
class Substation;
class Terminal;

class VoltageLevel : public Container {
public: // Identifiable
    const std::string& getTypeDescription() const override;

public:
    virtual ~VoltageLevel() = default;

    virtual void attach(Terminal& terminal, bool test) = 0;

    virtual void clean() = 0;

    virtual bool connect(Terminal& terminal) = 0;

    virtual void detach(Terminal& terminal) = 0;

    virtual bool disconnect(Terminal& terminal) = 0;

    double getHighVoltageLimit() const;

    double getLowVoltageLimit() const;

    const Network& getNetwork() const;

    Network& getNetwork();

    double getNominalVoltage() const;

    const Substation& getSubstation() const;

    Substation& getSubstation();

    virtual const TopologyKind& getTopologyKind() const = 0;

    virtual void invalidateCache() = 0;

    LoadAdder newLoad();

    VoltageLevel& setHighVoltageLimit(double highVoltageLimit);

    VoltageLevel& setLowVoltageLimit(double lowVoltageLimit);

    VoltageLevel& setNominalVoltage(double nominalVoltage);

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
