/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/LoadExponentialModelAdder.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

LoadExponentialModelAdder::LoadExponentialModelAdder(LoadAdder& parent) :
    m_parent(parent) {
}

LoadExponentialModelAdder &LoadExponentialModelAdder::setNp(double np) {
    m_np = checkExponent(m_parent, np);
    return *this;
}

LoadExponentialModelAdder &LoadExponentialModelAdder::setNq(double nq) {
    m_nq = checkExponent(m_parent, nq);
    return *this;
}

LoadAdder& LoadExponentialModelAdder::add() {
    checkExponent(m_parent, m_np);
    checkExponent(m_parent, m_nq);

    m_parent.setExpModelAdder(*this);

    return m_parent;
}
std::unique_ptr<LoadModel> LoadExponentialModelAdder::build() const {
    std::unique_ptr<LoadModel> ptrLoadModel = std::unique_ptr<LoadModel>(new LoadExponentialModel(m_np, m_nq));
    copyPropertiesTo(*ptrLoadModel);
    return ptrLoadModel;
}

void LoadExponentialModelAdder::addNewModel(Load& load, double np, double nq) {
    checkExponent(load, np);
    checkExponent(load, nq);

    std::unique_ptr<LoadModel> ptrLoadModel = std::unique_ptr<LoadModel>(new LoadExponentialModel(np, nq));
    load.setModel(std::move(ptrLoadModel));
}

}  // namespace iidm

}  // namespace powsybl