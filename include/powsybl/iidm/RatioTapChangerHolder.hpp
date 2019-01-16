/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_RATIOTAPCHANGERHOLDER_HPP
#define POWSYBL_IIDM_RATIOTAPCHANGERHOLDER_HPP

#include <powsybl/iidm/TapChangerHolder.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class RatioTapChanger;
class RatioTapChangerAdder;

class RatioTapChangerHolder : public TapChangerHolder {
public:
    ~RatioTapChangerHolder() noexcept override = default;

    virtual stdcxx::CReference<RatioTapChanger> getRatioTapChanger() const = 0;

    virtual stdcxx::Reference<RatioTapChanger> getRatioTapChanger() = 0;

    virtual const std::string& getTapChangerAttribute() const = 0;

    virtual RatioTapChangerAdder newRatioTapChanger() = 0;

    virtual void setRatioTapChanger(std::unique_ptr<RatioTapChanger> ratioTapChanger) = 0;

protected:
    RatioTapChangerHolder() noexcept = default;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_RATIOTAPCHANGERHOLDER_HPP
