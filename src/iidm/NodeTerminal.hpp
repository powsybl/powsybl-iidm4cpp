/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODETERMINAL_HPP
#define POWSYBL_IIDM_NODETERMINAL_HPP

#include <powsybl/iidm/Terminal.hpp>

#include "NodeTerminalViews.hpp"

namespace powsybl {

namespace iidm {

class NodeTerminal : public Terminal {
public:
    using BusBreakerView = terminal::BusBreakerView;

    using BusView = terminal::BusView;

    using NodeBreakerView = terminal::NodeBreakerView;

public: // Terminal
    double getAngle() const override;

    const BusBreakerView& getBusBreakerView() const override;

    BusBreakerView& getBusBreakerView() override;

    const BusView& getBusView() const override;

    BusView& getBusView() override;

    const NodeBreakerView& getNodeBreakerView() const override;

    NodeBreakerView& getNodeBreakerView() override;

    double getV() const override;

    bool isConnected() const override;

public:
    NodeTerminal(VariantManagerHolder& network, unsigned long node);

    NodeTerminal(const NodeTerminal& nodeTerminal) = delete;

    NodeTerminal(NodeTerminal&& nodeTerminal) = delete;

    ~NodeTerminal() noexcept override = default;

    NodeTerminal& operator=(const NodeTerminal& nodeTerminal) = delete;

    NodeTerminal& operator=(NodeTerminal&& nodeTerminal) = delete;

    long getConnectedComponentNumber() const;

    unsigned long getNode() const;

    long getSynchronousComponentNumber() const;

    NodeTerminal& setAngle(double angle);

    void setConnectedComponentNumber(long connectedComponentNumber);

    void setSynchronousComponentNumber(long componentNumber);

    NodeTerminal& setV(double v);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private:
    unsigned long m_node;

    std::vector<double> m_v;

    std::vector<double> m_angle;

    std::vector<long> m_connectedComponentNumber;

    std::vector<long> m_synchronousComponentNumber;

    node_terminal::NodeBreakerViewImpl m_nodeBreakerView;

    node_terminal::BusBreakerViewImpl m_busBreakerView;

    node_terminal::BusViewImpl m_busView;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NODETERMINAL_HPP
