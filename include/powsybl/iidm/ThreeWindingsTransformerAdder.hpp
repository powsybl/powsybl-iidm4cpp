/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_THREEWINDINGSTRANSFORMERADDER_HPP
#define POWSYBL_IIDM_THREEWINDINGSTRANSFORMERADDER_HPP

#include <powsybl/iidm/IdentifiableAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerLegAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class Substation;
class Terminal;
class VoltageLevel;

class ThreeWindingsTransformerAdder : public IdentifiableAdder<ThreeWindingsTransformerAdder> {
public:
    using LegAdder = three_windings_transformer::LegAdder;

public:
    explicit ThreeWindingsTransformerAdder(Substation& substation);

    ~ThreeWindingsTransformerAdder() noexcept override = default;

    ThreeWindingsTransformer& add();

    LegAdder newLeg1();

    LegAdder newLeg2();

    LegAdder newLeg3();

    ThreeWindingsTransformerAdder& setRatedU0(double ratedU0);

protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    Substation& getSubstation();

    void setLegAdder1(LegAdder& legAdder);

    void setLegAdder2(LegAdder& legAdder);

    void setLegAdder3(LegAdder& legAdder);

    friend class three_windings_transformer::LegAdder;

private:
    Substation& m_substation;

    stdcxx::optional<LegAdder> m_adder1;

    stdcxx::optional<LegAdder> m_adder2;

    stdcxx::optional<LegAdder> m_adder3;

    double m_ratedU0 = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_THREEWINDINGSTRANSFORMERADDER_HPP
