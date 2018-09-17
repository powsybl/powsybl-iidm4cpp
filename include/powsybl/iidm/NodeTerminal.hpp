/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODETERMINAL_HPP
#define POWSYBL_IIDM_NODETERMINAL_HPP

#include <powsybl/iidm/Terminal.hpp>

namespace powsybl {

namespace iidm {

class NodeTerminal : public Terminal {
public: // Terminal
    double getAngle() const override;

    double getV() const override;

    bool isConnected() const override;

public:
    NodeTerminal(MultiStateObject& network, unsigned long node);

    ~NodeTerminal() noexcept override = default;

    unsigned long getNode() const;

    NodeTerminal& setAngle(double angle);

    NodeTerminal& setV(double v);

protected: // Stateful
    void allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteStateArrayElement(unsigned long index) override;

    void extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceStateArraySize(unsigned long number) override;

private:
    NodeTerminal(const NodeTerminal& nodeTerminal) = delete;

    NodeTerminal(NodeTerminal&& nodeTerminal) = delete;

    NodeTerminal& operator=(const NodeTerminal& nodeTerminal) = delete;

    NodeTerminal& operator=(NodeTerminal&& nodeTerminal) = delete;

private:
    unsigned long m_node;

    std::vector<double> m_v;

    std::vector<double> m_angle;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NODETERMINAL_HPP
