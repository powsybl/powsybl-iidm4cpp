/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_THREEWINDINGSTRANSFORMER_HPP
#define POWSYBL_IIDM_THREEWINDINGSTRANSFORMER_HPP

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/PhaseTapChangerHolder.hpp>
#include <powsybl/iidm/RatioTapChangerHolder.hpp>

namespace powsybl {

namespace iidm {

class Substation;

class ThreeWindingsTransformer : public Connectable {
public:
    enum class Side : unsigned char {
        ONE,
        TWO,
        THREE
    };

public:
    class Leg : public virtual RatioTapChangerHolder, public virtual PhaseTapChangerHolder {
    public:  // Validable
        std::string getMessageHeader() const override;

    public:  // TapChangerHolder
        const Network& getNetwork() const override;

        Network& getNetwork() override;

    public:  // RatioTapChangerHolder
        stdcxx::CReference<RatioTapChanger> getRatioTapChanger() const override;

        stdcxx::Reference<RatioTapChanger> getRatioTapChanger() override;

        RatioTapChangerAdder newRatioTapChanger() override;

    public:  // PhaseTapChanger
        stdcxx::CReference<PhaseTapChanger> getPhaseTapChanger() const override;

        stdcxx::Reference<PhaseTapChanger> getPhaseTapChanger() override;

        PhaseTapChangerAdder newPhaseTapChanger() override;

    public:
        Leg(unsigned long legNumber, double r, double x, double g, double b, double ratedU, double ratedS);

        ~Leg() noexcept override = default;

        double getB() const;

        stdcxx::CReference<CurrentLimits> getCurrentLimits() const;

        stdcxx::Reference<CurrentLimits> getCurrentLimits();

        double getG() const;

        double getR() const;

        double getRatedS() const;

        double getRatedU() const;

        virtual stdcxx::CReference<Terminal> getTerminal() const;

        virtual stdcxx::Reference<Terminal> getTerminal();

        double getX() const;

        CurrentLimitsAdder<const std::nullptr_t, Leg> newCurrentLimits();

        Leg& setB(double b);

        Leg& setG(double g);

        Leg& setR(double r);

        Leg& setRatedS(double ratedS);

        Leg& setRatedU(double ratedU);

        Leg& setX(double x);

        std::string toString() const;

    private:  // TapChangerHolder
        unsigned long getRegulatingTapChangerCount() const override;

    private:  // RatioTapChangerHolder
        void setRatioTapChanger(std::unique_ptr<RatioTapChanger> ratioTapChanger) override;

    private:  // PhaseTapChangerHolder
        void setPhaseTapChanger(std::unique_ptr<PhaseTapChanger> phaseTapChanger) override;

    private:
        const std::string& getTypeDescription() const;

        void setCurrentLimits(std::nullptr_t side, std::unique_ptr<CurrentLimits> limits);

        void setTransformer(ThreeWindingsTransformer& transformer);

    private:
        friend class CurrentLimitsAdder<const std::nullptr_t, Leg>;

        friend class ThreeWindingsTransformerAdder;

    private:
        stdcxx::Reference<ThreeWindingsTransformer> m_transformer;

        unsigned long m_legNumber;

        double m_r;

        double m_x;

        double m_g;

        double m_b;

        double m_ratedU;

        double m_ratedS;

        std::unique_ptr<CurrentLimits> m_limits;

        std::unique_ptr<PhaseTapChanger> m_phaseTapChanger;

        std::unique_ptr<RatioTapChanger> m_ratioTapChanger;
    };

public:
    ThreeWindingsTransformer(const std::string& id, const std::string& name, std::unique_ptr<Leg> leg1, std::unique_ptr<Leg> leg2, std::unique_ptr<Leg> leg3, double ratedU0);

    ~ThreeWindingsTransformer() noexcept override = default;

    const Leg& getLeg1() const;

    Leg& getLeg1();

    const Leg& getLeg2() const;

    Leg& getLeg2();

    const Leg& getLeg3() const;

    Leg& getLeg3();

    double getRatedU0() const;

    Side getSide(const Terminal& terminal) const;

    const Substation& getSubstation() const;

    Substation& getSubstation();

    const Terminal& getTerminal(const Side& side) const;

    Terminal& getTerminal(const Side& side);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    friend class ThreeWindingsTransformerAdder;

private:
    std::unique_ptr<Leg> m_leg1;

    std::unique_ptr<Leg> m_leg2;

    std::unique_ptr<Leg> m_leg3;

    double m_ratedU0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_THREEWINDINGSTRANSFORMER_HPP
