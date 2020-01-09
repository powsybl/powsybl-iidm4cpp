/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_CVG_CURRENTLIMITSPERSEASON_HPP
#define POWSYBL_IIDM_EXTENSIONS_CVG_CURRENTLIMITSPERSEASON_HPP

#include <boost/optional.hpp>

#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>

namespace powsybl {

namespace iidm {

class Connectable;

namespace extensions {

namespace cvg {

class CurrentLimitsPerSeason : public Extension {
public:
    class CurrentLimitsTriple : public Validable {
    public:  // Validable
        std::string getMessageHeader() const override;

    public:
        CurrentLimitsTriple() = default;

        CurrentLimitsTriple(std::unique_ptr<CurrentLimits> currentLimits, std::unique_ptr<CurrentLimits> currentLimits1, std::unique_ptr<CurrentLimits> currentLimits2, std::unique_ptr<CurrentLimits> currentLimits3);

        ~CurrentLimitsTriple() noexcept override = default;

        CurrentLimitsTriple(const CurrentLimitsTriple& other);

        CurrentLimitsTriple& operator=(const CurrentLimitsTriple& other);

        stdcxx::CReference<CurrentLimits> getCurrentLimits() const;

        stdcxx::Reference<CurrentLimits> getCurrentLimits();

        stdcxx::CReference<CurrentLimits> getCurrentLimits1() const;

        stdcxx::Reference<CurrentLimits> getCurrentLimits1();

        stdcxx::CReference<CurrentLimits> getCurrentLimits2() const;

        stdcxx::Reference<CurrentLimits> getCurrentLimits2();

        stdcxx::CReference<CurrentLimits> getCurrentLimits3() const;

        stdcxx::Reference<CurrentLimits> getCurrentLimits3();

        CurrentLimitsAdder<std::nullptr_t, CurrentLimitsTriple> newCurrentLimits();

        CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsTriple> newCurrentLimits1();

        CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsTriple> newCurrentLimits2();

        CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsTriple> newCurrentLimits3();

        void setCurrentLimits(const ThreeWindingsTransformer::Side& side, std::unique_ptr<CurrentLimits> limits);

        void setCurrentLimits(std::nullptr_t side, std::unique_ptr<CurrentLimits> limits);

    private:
        std::unique_ptr<CurrentLimits> m_currentLimits;

        std::unique_ptr<CurrentLimits> m_currentLimits1;

        std::unique_ptr<CurrentLimits> m_currentLimits2;

        std::unique_ptr<CurrentLimits> m_currentLimits3;
    };

public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    CurrentLimitsPerSeason(Connectable& connectable, const std::map<std::string, CurrentLimitsTriple>& currentLimits);

    explicit CurrentLimitsPerSeason(Connectable& connectable);

    const std::map<std::string, CurrentLimitsTriple>& getCurrentLimits() const;

    std::map<std::string, CurrentLimitsTriple>& getCurrentLimits();

    stdcxx::CReference<CurrentLimitsTriple> getSeason(const std::string& name) const;

    stdcxx::Reference<CurrentLimitsTriple> getSeason(const std::string& name);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    std::map<std::string, CurrentLimitsTriple> m_currentLimits;
};

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_CVG_CURRENTLIMITSPERSEASON_HPP

