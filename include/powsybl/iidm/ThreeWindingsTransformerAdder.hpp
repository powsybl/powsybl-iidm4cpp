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

namespace powsybl {

namespace iidm {

class Substation;
class Terminal;
class VoltageLevel;

class ThreeWindingsTransformerAdder : public IdentifiableAdder<ThreeWindingsTransformerAdder> {
public:
    template <typename L>
    class LegAdder : public virtual Validable {
    public:
        ~LegAdder() noexcept override = default;

        ThreeWindingsTransformerAdder& add();

        L& setBus(const std::string& bus);

        L& setConnectableBus(const std::string& connectableBus);

        L& setNode(unsigned long node);

        L& setR(double r);

        L& setRatedU(double ratedU);

        L& setVoltageLevel(const std::string& voltageLevelId);

        L& setX(double x);

    protected:
        explicit LegAdder(ThreeWindingsTransformerAdder& parent);

        std::unique_ptr<Terminal> checkAndGetTerminal();

        VoltageLevel& checkAndGetVoltageLevel();

        virtual void checkParams() const;

        virtual L& clear();

        double getR() const;

        double getRatedU() const;

        double getX() const;

    private:
        ThreeWindingsTransformerAdder& m_parent;

        std::string m_voltageLevelId;

        stdcxx::optional<unsigned long> m_node;

        std::string m_bus;

        std::string m_connectableBus;

        double m_r;

        double m_x;

        double m_ratedU;
    };

public:
    class Leg1Adder : public LegAdder<Leg1Adder> {
    public:  // Validable
        std::string getMessageHeader() const override;

    public:
        ~Leg1Adder() noexcept override = default;

        Leg1Adder& setB(double b);

        Leg1Adder& setG(double g);

    protected:  // LegAdder
        void checkParams() const override;

        Leg1Adder& clear() override;


    protected:
        std::unique_ptr<ThreeWindingsTransformer::Leg1> checkAndGetLeg() const;

    private:
        explicit Leg1Adder(ThreeWindingsTransformerAdder& parent);

        friend class ThreeWindingsTransformerAdder;

    private:
        double m_g;

        double m_b;
    };

public:
    class Leg2Adder : public LegAdder<Leg2Adder> {
    public:  // Validable
        std::string getMessageHeader() const override;

    public:
        ~Leg2Adder() noexcept override = default;

    protected:
        std::unique_ptr<ThreeWindingsTransformer::Leg2or3> checkAndGetLeg() const;

    private:
        explicit Leg2Adder(ThreeWindingsTransformerAdder& parent);

        friend class ThreeWindingsTransformerAdder;
    };

public:
    class Leg3Adder : public LegAdder<Leg3Adder> {
    public:  // Validable
        std::string getMessageHeader() const override;

    public:
        ~Leg3Adder() noexcept override = default;

    protected:
        std::unique_ptr<ThreeWindingsTransformer::Leg2or3> checkAndGetLeg() const;

    private:
        explicit Leg3Adder(ThreeWindingsTransformerAdder& parent);

        friend class ThreeWindingsTransformerAdder;
    };

public:
    explicit ThreeWindingsTransformerAdder(Substation& substation);

    ~ThreeWindingsTransformerAdder() noexcept override = default;

    ThreeWindingsTransformer& add();

    Leg1Adder& newLeg1();

    Leg2Adder& newLeg2();

    Leg3Adder& newLeg3();

protected: // IdentifiableAdder
    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    Substation& getSubstation();

private:
    Substation& m_substation;

    Leg1Adder m_adder1;

    Leg2Adder m_adder2;

    Leg3Adder m_adder3;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/ThreeWindingsTransformerAdder.hxx>

#endif  // POWSYBL_IIDM_THREEWINDINGSTRANSFORMERADDER_HPP
