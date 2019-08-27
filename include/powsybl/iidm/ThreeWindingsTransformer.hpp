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
#include <powsybl/iidm/RatioTapChangerHolder.hpp>

namespace powsybl {

namespace iidm {

class Substation;

class ThreeWindingsTransformer : public Connectable {
public:
    enum class Side : unsigned int {
        ONE,
        TWO,
        THREE
    };

public:
    template <typename L>
    class LegBase : public virtual Validable {
    public: // Validable
        std::string getMessageHeader() const override;

    public :
        LegBase(double r, double x, double ratedU);

        ~LegBase() noexcept override = default;

        stdcxx::CReference<CurrentLimits> getCurrentLimits() const;

        stdcxx::Reference<CurrentLimits> getCurrentLimits();

        double getR() const;

        double getRatedU() const;

        virtual stdcxx::CReference<Terminal> getTerminal() const;

        virtual stdcxx::Reference<Terminal> getTerminal();

        double getX() const;

        CurrentLimitsAdder<const std::nullptr_t, LegBase<L>> newCurrentLimits();

        L& setR(double r);

        L& setRatedU(double ratedU);

        L& setX(double x);

    protected:
        stdcxx::CReference<Terminal> getTerminal(unsigned long index) const;

        stdcxx::Reference<Terminal> getTerminal(unsigned long index);

        stdcxx::CReference<ThreeWindingsTransformer> getTransformer() const;

        stdcxx::Reference<ThreeWindingsTransformer>& getTransformer();

        virtual const std::string& getTypeDescription() const = 0;

    private:
        void setCurrentLimits(std::nullptr_t side, std::unique_ptr<CurrentLimits> limits);

        void setTransformer(ThreeWindingsTransformer& transformer);

    private:
        friend class CurrentLimitsAdder<const std::nullptr_t, LegBase<L>>;

        friend class ThreeWindingsTransformerAdder;

    private:
        stdcxx::Reference<ThreeWindingsTransformer> m_transformer;

        double m_r;

        double m_x;

        double m_ratedU;

        std::unique_ptr<CurrentLimits> m_limits;
    };

public:
    class Leg1 : public LegBase<Leg1> {
    public:
        Leg1(double r, double x, double g, double b, double ratedU);

        ~Leg1() noexcept override = default;

        double getB() const;

        double getG() const;

        Leg1& setB(double b);

        Leg1& setG(double g);

        std::string toString() const;

    protected: // LegBase
        const std::string& getTypeDescription() const override;

    private:
        double m_g;

        double m_b;
    };

public:
    class Leg2or3 : public LegBase<Leg2or3>, public RatioTapChangerHolder {
    public: // TapChangerHolder
        const Network& getNetwork() const override;

        Network& getNetwork() override;

    public: // RatioTapChangerHolder
        stdcxx::CReference<RatioTapChanger> getRatioTapChanger() const override;

        stdcxx::Reference<RatioTapChanger> getRatioTapChanger() override;

        RatioTapChangerAdder newRatioTapChanger() override;

    public:
        Leg2or3(double r, double x, double ratedU);

        ~Leg2or3() noexcept override = default;

        virtual std::string toString() const = 0;

    protected: // RatioTapChangerHolder
        void setRatioTapChanger(std::unique_ptr<RatioTapChanger> ratioTapChanger) override;

    private:
        std::unique_ptr<RatioTapChanger> m_ratioTapChanger;
    };

public:
    ThreeWindingsTransformer(const std::string& id, const std::string& name, std::unique_ptr<Leg1> leg1, std::unique_ptr<Leg2or3> leg2, std::unique_ptr<Leg2or3> leg3);

    ~ThreeWindingsTransformer() noexcept override = default;

    const Leg1& getLeg1() const;

    Leg1& getLeg1();

    const Leg2or3& getLeg2() const;

    Leg2or3& getLeg2();

    const Leg2or3& getLeg3() const;

    Leg2or3& getLeg3();

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
    std::unique_ptr<Leg1> m_leg1;

    std::unique_ptr<Leg2or3> m_leg2;

    std::unique_ptr<Leg2or3> m_leg3;
};

ThreeWindingsTransformer::Side getThreeWindingsTransformerSide(const std::string& sideName);

std::string getThreeWindingsTransformerSideName(const ThreeWindingsTransformer::Side& side);

std::ostream& operator<<(std::ostream& stream, const ThreeWindingsTransformer::Side& side);

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/ThreeWindingsTransformer.hxx>

#endif  // POWSYBL_IIDM_THREEWINDINGSTRANSFORMER_HPP
