/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LINEADDER_HPP
#define POWSYBL_IIDM_LINEADDER_HPP

#include <powsybl/iidm/BranchAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class Line;
class Network;

class LineAdder : public BranchAdder<Line, LineAdder> {
public:
    explicit LineAdder(Network& network);
    explicit LineAdder(Network& network, const std::string& subNetworkId);

    explicit LineAdder(Network& network, const Line& copyLine);
    explicit LineAdder(Network& network, const std::string& subNetworkId, const Line& copyLine);

    ~LineAdder() noexcept override = default;

    Line& add() override;

    LineAdder& setB1(double b1);

    LineAdder& setB2(double b2);

    LineAdder& setG1(double g1);

    LineAdder& setG2(double g2);

    LineAdder& setR(double r);

    LineAdder& setX(double x);

protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

    void initFromLine(const Line& copyLine);

private:
    Network& m_network;
    std::string m_subnetworkId;
    stdcxx::CReference<Line> m_copyLine;

    double m_b1 = 0.0;

    double m_b2 = 0.0;

    double m_g1 = 0.0;

    double m_g2 = 0.0;

    double m_r = stdcxx::nan();

    double m_x = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LINEADDER_HPP
