/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef IIDM_THREEWINDINGSTRANSFORMERLEG2_HPP
#define IIDM_THREEWINDINGSTRANSFORMERLEG2_HPP

#include <powsybl/iidm/ThreeWindingsTransformer.hpp>

namespace powsybl {

namespace iidm {

class ThreeWindingsTransformerLeg2 : public ThreeWindingsTransformer::Leg2or3 {
public: // LegBase
    stdcxx::CReference<Terminal> getTerminal() const override;

    stdcxx::Reference<Terminal> getTerminal() override;

    std::string toString() const override;

public:
    ThreeWindingsTransformerLeg2(double r, double x, double ratedU);

    ~ThreeWindingsTransformerLeg2() noexcept override = default;

protected: // LegBase
    const std::string& getTypeDescription() const override;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // IIDM_THREEWINDINGSTRANSFORMERLEG2_HPP
