/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGELEVEL_HPP
#define POWSYBL_IIDM_VOLTAGELEVEL_HPP

#include <powsybl/iidm/Container.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/TopologyKind.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class BusBreakerView;
class BusView;
class Network;
class NodeBreakerView;
class Substation;
class Terminal;

class VoltageLevel : public Container {
public:
    virtual ~VoltageLevel() = default;

    virtual void attach(Terminal& terminal, bool test) = 0;

    virtual void clean() = 0;

    virtual bool connect(Terminal& terminal) = 0;

    virtual void detach(Terminal& terminal) = 0;

    virtual bool disconnect(Terminal& terminal) = 0;

    virtual const BusBreakerView& getBusBreakerView() const = 0;

    virtual BusBreakerView& getBusBreakerView() = 0;

    virtual const BusView& getBusView() const = 0;

    virtual BusView& getBusView() = 0;

    template <typename T>
    T& getConnectable(const std::string& id);

    double getHighVoltageLimit() const;

    double getLowVoltageLimit() const;

    const Network& getNetwork() const;

    Network& getNetwork();

    virtual const NodeBreakerView& getNodeBreakerView() const = 0;

    virtual NodeBreakerView& getNodeBreakerView() = 0;

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

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    stdcxx::Reference<Substation> m_substation;

    double m_highVoltageLimit;

    double m_lowVoltageLimit;

    double m_nominalVoltage;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/VoltageLevel.hxx>

#endif  // POWSYBL_IIDM_VOLTAGELEVEL_HPP
