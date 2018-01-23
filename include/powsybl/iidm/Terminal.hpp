/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TERMINAL_HPP
#define POWSYBL_IIDM_TERMINAL_HPP

#include <functional>
#include <vector>

#include <powsybl/stdcxx.hpp>
#include <powsybl/iidm/MultiStateObject.hpp>
#include <powsybl/iidm/Stateful.hpp>

namespace powsybl {

namespace iidm {

class Connectable;
class VoltageLevel;

class Terminal : public Stateful {
public:
    virtual ~Terminal() = default;

    bool connect();

    bool disconnect();

    virtual double getAngle() const = 0;

    double getI() const;

    double getP() const;

    double getQ() const;

    virtual double getV() const = 0;

    const VoltageLevel& getVoltageLevel() const;

    VoltageLevel& getVoltageLevel();

    Terminal& setConnectable(const stdcxx::optional_reference_wrapper<Connectable>& connectable);

    Terminal& setP(double p);

    Terminal& setQ(double q);

    Terminal& setVoltageLevel(const stdcxx::optional_reference_wrapper<VoltageLevel>& voltageLevel);

protected: // Stateful
    void allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteStateArrayElement(unsigned long index) override;

    void extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceStateArraySize(unsigned long number) override;

    friend class Connectable;

protected:
    Terminal(MultiStateObject& network);

protected:
    std::reference_wrapper<MultiStateObject> m_network;

    stdcxx::optional_reference_wrapper<Connectable> m_connectable;

    stdcxx::optional_reference_wrapper<VoltageLevel> m_voltageLevel;

    std::vector<double> m_p;

    std::vector<double> m_q;
};

}

}

#endif  // POWSYBL_IIDM_TERMINAL_HPP
