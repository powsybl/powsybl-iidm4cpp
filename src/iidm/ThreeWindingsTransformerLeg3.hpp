/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef IIDM_THREEWINDINGSTRANSFORMERLEG3_HPP
#define IIDM_THREEWINDINGSTRANSFORMERLEG3_HPP

#include <powsybl/iidm/ThreeWindingsTransformer.hpp>

namespace powsybl {

namespace iidm {

class ThreeWindingsTransformerLeg3 : public ThreeWindingsTransformer::Leg2or3 {
public: // LegBase
    stdcxx::CReference<Terminal> getTerminal() const override;

    stdcxx::Reference<Terminal> getTerminal() override;

public:
    ThreeWindingsTransformerLeg3(double r, double x, double ratedU);

    ~ThreeWindingsTransformerLeg3() noexcept override = default;

protected: // LegBase
    const std::string& getLegAttribute() const override;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // IIDM_THREEWINDINGSTRANSFORMERLEG3_HPP
