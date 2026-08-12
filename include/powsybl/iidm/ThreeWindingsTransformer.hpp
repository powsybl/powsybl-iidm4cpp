/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_THREEWINDINGSTRANSFORMER_HPP
#define POWSYBL_IIDM_THREEWINDINGSTRANSFORMER_HPP

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/Overload.hpp>
#include <powsybl/iidm/ThreeSides.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerLeg.hpp>

namespace powsybl {

namespace iidm {

class Substation;

class ThreeWindingsTransformer : public Connectable {
public:
    using Leg = three_windings_transformer::Leg;

public:  // Identifiable
    const IdentifiableType& getType() const override;

public: //Connectable
    virtual void remove() override;

public:
    ThreeWindingsTransformer(const std::string& id, const std::string& name, bool fictitious, Leg&& leg1, Leg&& leg2, Leg&& leg3, double ratedU0);

    ~ThreeWindingsTransformer() noexcept override = default;

    const Leg& getLeg(const ThreeSides& side) const;

    Leg& getLeg(const ThreeSides& side);

    const Leg& getLeg1() const;

    Leg& getLeg1();

    const Leg& getLeg2() const;

    Leg& getLeg2();

    const Leg& getLeg3() const;

    Leg& getLeg3();

    stdcxx::const_range<Leg> getLegs() const;

    stdcxx::range<Leg> getLegs();

    double getRatedU0() const;

    ThreeWindingsTransformer& setRatedU0(double ratedU0);

    ThreeSides getSide(const Terminal& terminal) const;

    stdcxx::CReference<Substation> getSubstation() const;

    stdcxx::Reference<Substation> getSubstation();

    const Terminal& getTerminal(const ThreeSides& side) const;

    Terminal& getTerminal(const ThreeSides& side);

    const Terminal& getTerminal(const std::string& voltageLevelId) const;

    Terminal& getTerminal(const std::string& voltageLevelId);

    /**
     * Only checks permanent limits for limitType::CURRENT
     */
    bool isOverloaded() const;

    /**
     * Only checks permanent limits for limitType::CURRENT
     */
    bool isOverloaded(double limitReduction) const;

    virtual unsigned long getOverloadDuration() const;

    bool checkPermanentLimit(const ThreeSides& side, const LimitType& type) const;

    bool checkPermanentLimit(const ThreeSides& side, double limitReduction, const LimitType& type) const;

    bool checkPermanentLimit1(const LimitType& type) const;

    bool checkPermanentLimit1(double limitReduction, const LimitType& type) const;

    bool checkPermanentLimit2(const LimitType& type) const;

    bool checkPermanentLimit2(double limitReduction, const LimitType& type) const;

    bool checkPermanentLimit3(const LimitType& type) const;

    bool checkPermanentLimit3(double limitReduction, const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits(const ThreeSides& side, const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits(const ThreeSides& side, double limitReduction, const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits1(const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits1(double limitReduction, const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits2(const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits2(double limitReduction, const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits3(const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits3(double limitReduction, const LimitType& type) const;

    std::vector<std::unique_ptr<Overload>> checkAllTemporaryLimits(const ThreeSides& side, const LimitType& type) const;

    std::vector<std::unique_ptr<Overload>> checkAllTemporaryLimits(const ThreeSides& side, double limitReduction, const LimitType& type) const;

    void applySolvedValues();
    void setRatioTapPositionToSolvedTapPosition();
    void setPhaseTapPositionToSolvedTapPosition();

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    friend class three_windings_transformer::Leg;

    using Connectable::getTerminal;

private:
    // The legs of this transformer, in the natural order (leg1, leg2 and leg3)
    std::vector<Leg> m_legs;

    double m_ratedU0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_THREEWINDINGSTRANSFORMER_HPP
