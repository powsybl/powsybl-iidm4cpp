/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_REACTIVELIMITSHOLDER_HPP
#define POWSYBL_IIDM_REACTIVELIMITSHOLDER_HPP

#include <memory>

#include <powsybl/iidm/MinMaxReactiveLimitsAdder.hpp>
#include <powsybl/iidm/ReactiveCapabilityCurveAdder.hpp>

namespace powsybl {

namespace iidm {

class ReactiveLimits;

class ReactiveLimitsHolder {
public:
    virtual ~ReactiveLimitsHolder() noexcept = default;

    template <typename T, typename = typename std::enable_if<std::is_base_of<ReactiveLimits, T>::value>::type>
    const T& getReactiveLimits() const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<ReactiveLimits, T>::value>::type>
    T& getReactiveLimits();

    MinMaxReactiveLimitsAdder newMinMaxReactiveLimits();

    ReactiveCapabilityCurveAdder newReactiveCapabilityCurve();

protected:
    ReactiveLimitsHolder();

    explicit ReactiveLimitsHolder(std::unique_ptr<ReactiveLimits>&& limits);

private:
    void setReactiveLimits(std::unique_ptr<ReactiveLimits>&& limits);

    friend MinMaxReactiveLimitsAdder;

    friend ReactiveCapabilityCurveAdder;

private:
    std::unique_ptr<ReactiveLimits> m_reactiveLimits;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/ReactiveLimitsHolder.hxx>

#endif //POWSYBL_IIDM_REACTIVELIMITSHOLDER_HPP
