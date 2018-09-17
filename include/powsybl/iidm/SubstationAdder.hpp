/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SUBSTATIONADDER_HPP
#define POWSYBL_IIDM_SUBSTATIONADDER_HPP

#include <set>

#include <powsybl/iidm/Country.hpp>
#include <powsybl/iidm/IdentifiableAdder.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class Network;
class Substation;

class SubstationAdder : public IdentifiableAdder<SubstationAdder> {
public:
    ~SubstationAdder() noexcept override = default;

    Substation& add();

    SubstationAdder& setCountry(const Country& country);

    SubstationAdder& addGeographicalTag(const std::string& geographicalTag);

    SubstationAdder& setTso(const std::string& tso);

protected: // IdentifiableAdder
    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit SubstationAdder(Network& network);

    friend class Network;

private:
    Network& m_network;

    stdcxx::optional<Country> m_country;

    std::string m_tso;

    std::set<std::string> m_geographicalTags;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SUBSTATIONADDER_HPP
