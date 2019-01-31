/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LINEADDER_HPP
#define POWSYBL_IIDM_LINEADDER_HPP

#include <powsybl/iidm/BranchAdder.hpp>

namespace powsybl {

namespace iidm {

class Line;
class Network;

class LineAdder : public BranchAdder<LineAdder> {
public:
    explicit LineAdder(Network& network);

    ~LineAdder() noexcept override = default;

    Line& add();

    LineAdder& setB1(double b1);

    LineAdder& setB2(double b2);

    LineAdder& setG1(double g1);

    LineAdder& setG2(double g2);

    LineAdder& setR(double r);

    LineAdder& setX(double x);

protected: // IdentifiableAdder
    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    Network& m_network;

    double m_b1;

    double m_b2;

    double m_g1;

    double m_g2;

    double m_r;

    double m_x;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LINEADDER_HPP
