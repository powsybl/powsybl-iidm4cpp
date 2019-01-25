/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TWOWINDINGSTRANSFORMERADDER_HPP
#define POWSYBL_IIDM_TWOWINDINGSTRANSFORMERADDER_HPP

#include <powsybl/iidm/BranchAdder.hpp>

namespace powsybl {

namespace iidm {

class Network;
class Substation;
class TwoWindingsTransformer;

class TwoWindingsTransformerAdder : public BranchAdder<TwoWindingsTransformerAdder> {
public:
    explicit TwoWindingsTransformerAdder(Substation& substation);

    ~TwoWindingsTransformerAdder() noexcept override = default;

    TwoWindingsTransformer& add();

    TwoWindingsTransformerAdder& setB(double b);

    TwoWindingsTransformerAdder& setG(double g);

    TwoWindingsTransformerAdder& setR(double r);

    TwoWindingsTransformerAdder& setRatedU1(double ratedU1);

    TwoWindingsTransformerAdder& setRatedU2(double ratedU2);

    TwoWindingsTransformerAdder& setX(double x);

protected: // IdentifiableAdder
    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    Substation& m_substation;

    double m_r;

    double m_x;

    double m_g;

    double m_b;

    double m_ratedU1;

    double m_ratedU2;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TWOWINDINGSTRANSFORMERADDER_HPP
