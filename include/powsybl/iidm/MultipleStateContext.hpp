/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_MULTIPLESTATECONTEXT_HPP
#define POWSYBL_IIDM_MULTIPLESTATECONTEXT_HPP

#include <powsybl/iidm/StateContext.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class MultipleStateContext : public StateContext {
public:
    explicit MultipleStateContext(unsigned long index);

    ~MultipleStateContext() override = default;

protected: // StateContext
    unsigned long getStateIndex() const override;

    void resetIfStateIndexIs(unsigned long index) override;

    void setStateIndex(unsigned long index) override;

private:
    stdcxx::optional<unsigned long> m_index;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_MULTIPLESTATECONTEXT_HPP
