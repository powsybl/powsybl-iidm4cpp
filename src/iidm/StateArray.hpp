/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_STATEARRAY_HPP
#define POWSYBL_IIDM_STATEARRAY_HPP

#include <memory>

#include <powsybl/iidm/MultiStateObject.hpp>
#include <powsybl/iidm/Stateful.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

#include "State.hpp"

namespace powsybl {

namespace iidm {

template <typename T>
class StateArray {
public:
    typedef std::function<std::unique_ptr<T>()> StateFactory;

public:
    StateArray(MultiStateObject& multiStateObject, const StateFactory& stateFactory);

    ~StateArray() noexcept = default;

    void allocateStateArrayElement(const std::set<unsigned long>& indexes, const StateFactory& stateFactory);

    std::unique_ptr<T> copy(unsigned long index) const;

    void deleteStateArrayElement(unsigned long index);

    void extendStateArraySize(unsigned long initStateArraySize, unsigned long number, const StateFactory& stateFactory);

    const T& get() const;

    T& get();

    void reduceStateArraySize(unsigned long number);

private:
    MultiStateObject& m_multiStateObject;

    std::vector<std::unique_ptr<T> > m_states;
};

}  // namespace iidm

}  // namespace powsybl

#include "StateArray.hxx"

#endif  // POWSYBL_IIDM_STATEARRAY_HPP
