/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOADADDER_HPP
#define POWSYBL_IIDM_LOADADDER_HPP

#include <powsybl/iidm/InjectionAdder.hpp>
#include <powsybl/iidm/LoadType.hpp>
#include <powsybl/stdcxx/export.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class Load;
class Network;
class VoltageLevel;

class IIDM_DECLSPEC LoadAdder : public InjectionAdder<LoadAdder> {
public:
    ~LoadAdder() noexcept override = default;

    Load& add();

    LoadAdder& setLoadType(const LoadType& loadType);

    LoadAdder& setP0(double p0);

    LoadAdder& setQ0(double q0);

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit LoadAdder(VoltageLevel& voltageLevel);

    friend class VoltageLevel;

private:
    LoadType m_loadType = LoadType::UNDEFINED;

    double m_p0 = stdcxx::nan();

    double m_q0 = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOADADDER_HPP
