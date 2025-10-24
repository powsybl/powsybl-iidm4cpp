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
#include <powsybl/iidm/NetworkRef.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Network;
class NetworkIndex;
class OverloadManagementSystem;
class OverloadManagementSystemAdder;
class ThreeWindingsTransformer;
class ThreeWindingsTransformerAdder;
class TwoWindingsTransformer;
class TwoWindingsTransformerAdder;

class Substation : public Container {
public:  // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

    const Network& getParentNetwork() const override;

    Network& getParentNetwork() override;

    const IdentifiableType& getType() const override;

public:
    Substation(Network& network, const std::string& id, const std::string& name, bool fictitious, const stdcxx::optional<Country>& country, const std::string& tso, const std::set<std::string>& geographicalTags);
    Substation(Network& rootNetwork, Network& subNetwork, const std::string& id, const std::string& name, bool fictitious, const stdcxx::optional<Country>& country, const std::string& tso, const std::set<std::string>& geographicalTags);

    ~Substation() noexcept override = default;

    Substation& addGeographicalTag(const std::string& geographicalTag);

    const stdcxx::optional<Country>& getCountry() const;

    const std::set<std::string>& getGeographicalTags() const;

    unsigned long getThreeWindingsTransformerCount() const;

    stdcxx::const_range<ThreeWindingsTransformer> getThreeWindingsTransformers() const;

    stdcxx::range<ThreeWindingsTransformer> getThreeWindingsTransformers();

    const std::string& getTso() const;

    unsigned long getTwoWindingsTransformerCount() const;

    stdcxx::const_range<TwoWindingsTransformer> getTwoWindingsTransformers() const;

    stdcxx::range<TwoWindingsTransformer> getTwoWindingsTransformers();

    stdcxx::const_range<VoltageLevel> getVoltageLevels() const;

    stdcxx::range<VoltageLevel> getVoltageLevels();

    unsigned long getOverloadManagementSystemCount() const;

    stdcxx::const_range<OverloadManagementSystem> getOverloadManagementSystems() const;

    stdcxx::range<OverloadManagementSystem> getOverloadManagementSystems();

    ThreeWindingsTransformerAdder newThreeWindingsTransformer();

    TwoWindingsTransformerAdder newTwoWindingsTransformer();

    VoltageLevelAdder newVoltageLevel();

    OverloadManagementSystemAdder newOverloadManagementSystem();

    void remove();

    Substation& setCountry(const stdcxx::optional<Country>& country);

    Substation& setTso(const std::string& tso);

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    void addVoltageLevel(VoltageLevel& voltageLevel);

    void addOverloadManagement(OverloadManagementSystem& overloadManagementSystem);

    void remove(const VoltageLevel& voltageLevel);

    void setNetworkRef(Network& network);

    stdcxx::Reference<Network>& getSubNetworkRef();

    void setSubNetworkRef(const stdcxx::Reference<Network>& subNetwork);

    friend class VoltageLevel;

    friend class VoltageLevelAdder;

    friend class OverloadManagementSystem;
    friend class OverloadManagementSystemAdder;

    friend class NetworkIndex;

private:
    NetworkRef m_rootNetwork;
    stdcxx::Reference<Network> m_subNetwork;

    stdcxx::optional<Country> m_country;

    std::string m_tso;

    std::set<std::string> m_geographicalTags;

    std::vector<std::reference_wrapper<VoltageLevel> > m_voltageLevels;

    std::vector<std::reference_wrapper<OverloadManagementSystem> > m_overloadManagementSystems;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SUBSTATION_HPP
