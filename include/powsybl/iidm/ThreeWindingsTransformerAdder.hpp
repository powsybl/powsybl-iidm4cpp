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
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class Substation;
class Terminal;
class VoltageLevel;

class ThreeWindingsTransformerAdder : public IdentifiableAdder<ThreeWindingsTransformerAdder> {
public:
    class LegAdder : public virtual Validable {
    public:  // Validable
        std::string getMessageHeader() const override;

    public:
        ~LegAdder() noexcept override = default;

        ThreeWindingsTransformerAdder& add();

        LegAdder& setB(double b);

        LegAdder& setBus(const std::string& bus);

        LegAdder& setConnectableBus(const std::string& connectableBus);

        LegAdder& setG(double g);

        LegAdder& setNode(unsigned long node);

        LegAdder& setR(double r);

        LegAdder& setRatedS(double ratedS);

        LegAdder& setRatedU(double ratedU);

        LegAdder& setVoltageLevel(const std::string& voltageLevelId);

        LegAdder& setX(double x);

    private:
        LegAdder(ThreeWindingsTransformerAdder& parent, unsigned long legNumber);

        LegAdder& operator=(const LegAdder& adder);

        ThreeWindingsTransformer::Leg build() const;

        std::unique_ptr<Terminal> checkAndGetTerminal(VoltageLevel& voltageLevel);

        VoltageLevel& checkAndGetVoltageLevel();

        friend class ThreeWindingsTransformerAdder;

    private:
        ThreeWindingsTransformerAdder& m_parent;

        unsigned long m_legNumber;

        std::string m_voltageLevelId;

        stdcxx::optional<unsigned long> m_node;

        std::string m_bus;

        std::string m_connectableBus;

        double m_r = stdcxx::nan();

        double m_x = stdcxx::nan();

        double m_g = stdcxx::nan();

        double m_b = stdcxx::nan();

        double m_ratedU = stdcxx::nan();

        double m_ratedS = stdcxx::nan();
    };

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
