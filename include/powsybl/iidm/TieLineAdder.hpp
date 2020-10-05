/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TIELINEADDER_HPP
#define POWSYBL_IIDM_TIELINEADDER_HPP

#include <powsybl/iidm/BranchAdder.hpp>

namespace powsybl {

namespace iidm {

class Network;
class TieLine;

class TieLineAdder : public BranchAdder<TieLineAdder> {
public: // IdentifiableAdder
    TieLineAdder& setId(const std::string& id) override;

    TieLineAdder& setName(const std::string& name) override;

public:
    explicit TieLineAdder(Network& network);

    ~TieLineAdder() noexcept override = default;

    TieLine& add();

    TieLineAdder& line1();

    TieLineAdder& line2();

    TieLineAdder& setFictitious(bool fictitious) override;

    TieLineAdder& setB1(double b1);

    TieLineAdder& setB2(double b2);

    TieLineAdder& setG1(double g1);

    TieLineAdder& setG2(double g2);

    TieLineAdder& setR(double r);

    TieLineAdder& setUcteXnodeCode(const std::string& ucteXnodeCode);

    TieLineAdder& setX(double x);

    TieLineAdder& setXnodeP(double xnodeP);

    TieLineAdder& setXnodeQ(double xnodeQ);

protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    TieLine::HalfLine& getActiveHalf() const;

private:
    Network& m_network;

    TieLine::HalfLine m_half1;

    TieLine::HalfLine m_half2;

    stdcxx::Reference<TieLine::HalfLine> m_activeHalf;

    std::string m_ucteXnodeCode;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TIELINEADDER_HPP
