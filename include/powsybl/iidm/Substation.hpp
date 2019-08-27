/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SUBSTATION_HPP
#define POWSYBL_IIDM_SUBSTATION_HPP

#include <functional>
#include <set>
#include <string>
#include <vector>

#include <powsybl/iidm/Container.hpp>
#include <powsybl/iidm/Country.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class ThreeWindingsTransformer;
class ThreeWindingsTransformerAdder;
class TwoWindingsTransformer;
class TwoWindingsTransformerAdder;

class Substation : public Container {
public:
    Substation(Network& network, const std::string& id, const std::string& name, const stdcxx::optional<Country>& country, const std::string& tso, const std::set<std::string>& geographicalTags);

    ~Substation() noexcept override = default;

    Substation& addGeographicalTag(const std::string& geographicalTag);

    const stdcxx::optional<Country>& getCountry() const;

    const std::set<std::string>& getGeographicalTags() const;

    const Network& getNetwork() const;

    Network& getNetwork();

    unsigned long getThreeWindingsTransformerCount() const;

    stdcxx::const_range<ThreeWindingsTransformer> getThreeWindingsTransformers() const;

    stdcxx::range<ThreeWindingsTransformer> getThreeWindingsTransformers();

    const std::string& getTso() const;

    unsigned long getTwoWindingsTransformerCount() const;

    stdcxx::const_range<TwoWindingsTransformer> getTwoWindingsTransformers() const;

    stdcxx::range<TwoWindingsTransformer> getTwoWindingsTransformers();

    stdcxx::const_range<VoltageLevel> getVoltageLevels() const;

    stdcxx::range<VoltageLevel> getVoltageLevels();

    ThreeWindingsTransformerAdder newThreeWindingsTransformer();

    TwoWindingsTransformerAdder newTwoWindingsTransformer();

    VoltageLevelAdder newVoltageLevel();

    Substation& setCountry(const stdcxx::optional<Country>& country);

    Substation& setTso(const std::string& tso);

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    void addVoltageLevel(VoltageLevel& voltageLevel);

    friend class VoltageLevelAdder;

private:
    stdcxx::Reference<Network> m_network;

    stdcxx::optional<Country> m_country;

    std::string m_tso;

    std::set<std::string> m_geographicalTags;

    std::vector<std::reference_wrapper<VoltageLevel> > m_voltageLevels;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SUBSTATION_HPP
