/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TIELINEADDER_HPP
#define POWSYBL_IIDM_TIELINEADDER_HPP

#include <powsybl/iidm/BranchAdder.hpp>
#include <powsybl/iidm/HalfLineAdder.hpp>
#include <powsybl/iidm/TieLine.hpp>

namespace powsybl {

namespace iidm {

class Network;

class TieLineAdder : public BranchAdder<TieLineAdder> {
public:
    using HalfLineAdder = tie_line::HalfLineAdder;

public:
    explicit TieLineAdder(Network& network);

    ~TieLineAdder() noexcept override = default;

    TieLine& add();

    HalfLineAdder newHalfLine1();

    HalfLineAdder newHalfLine2();

    TieLineAdder& setUcteXnodeCode(const std::string& ucteXnodeCode);

protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    void setHalfLineAdder1(HalfLineAdder& adder);

    void setHalfLineAdder2(HalfLineAdder& adder);

    friend class tie_line::HalfLineAdder;

private:
    Network& m_network;

    stdcxx::optional<HalfLineAdder> m_halfLineAdder1;

    stdcxx::optional<HalfLineAdder> m_halfLineAdder2;

    std::string m_ucteXnodeCode;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TIELINEADDER_HPP
