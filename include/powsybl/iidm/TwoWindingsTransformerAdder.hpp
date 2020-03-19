/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TWOWINDINGSTRANSFORMERADDER_HPP
#define POWSYBL_IIDM_TWOWINDINGSTRANSFORMERADDER_HPP

#include <powsybl/iidm/BranchAdder.hpp>
#include <powsybl/stdcxx/export.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class Network;
class Substation;
class TwoWindingsTransformer;

class IIDM_DECLSPEC TwoWindingsTransformerAdder : public BranchAdder<TwoWindingsTransformerAdder> {
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

    double m_r = stdcxx::nan();

    double m_x = stdcxx::nan();

    double m_g = stdcxx::nan();

    double m_b = stdcxx::nan();

    double m_ratedU1 = stdcxx::nan();

    double m_ratedU2 = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TWOWINDINGSTRANSFORMERADDER_HPP
