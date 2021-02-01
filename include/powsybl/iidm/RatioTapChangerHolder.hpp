/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_RATIOTAPCHANGERHOLDER_HPP
#define POWSYBL_IIDM_RATIOTAPCHANGERHOLDER_HPP

#include <powsybl/iidm/TapChangerHolder.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class RatioTapChanger;
class RatioTapChangerAdder;

class RatioTapChangerHolder : public virtual TapChangerHolder {
public:
    ~RatioTapChangerHolder() noexcept override = default;

    stdcxx::CReference<RatioTapChanger> getOptionalRatioTapChanger() const;

    stdcxx::Reference<RatioTapChanger> getOptionalRatioTapChanger();

    virtual const RatioTapChanger& getRatioTapChanger() const = 0;

    virtual RatioTapChanger& getRatioTapChanger() = 0;

    virtual RatioTapChangerAdder newRatioTapChanger() = 0;

protected:
    RatioTapChangerHolder() noexcept = default;

private:
    virtual void setRatioTapChanger(std::unique_ptr<RatioTapChanger> ratioTapChanger) = 0;

private:
    friend class RatioTapChanger;

    friend class RatioTapChangerAdder;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_RATIOTAPCHANGERHOLDER_HPP
