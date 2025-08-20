/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/LoadZipModelAdder.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

LoadZipModelAdder::LoadZipModelAdder(LoadAdder& parent) :
    m_parent(parent) {
}

LoadZipModelAdder &LoadZipModelAdder::setC0p(double c0p) {
    m_c0p = checkCoefficient(m_parent, c0p);
    return *this;
}
LoadZipModelAdder &LoadZipModelAdder::setC1p(double c1p) {
    m_c1p = checkCoefficient(m_parent, c1p);
    return *this;
}
LoadZipModelAdder &LoadZipModelAdder::setC2p(double c2p) {
    m_c2p = checkCoefficient(m_parent, c2p);
    return *this;
}
LoadZipModelAdder &LoadZipModelAdder::setC0q(double c0q) {
    m_c0q = checkCoefficient(m_parent, c0q);
    return *this;
}
LoadZipModelAdder &LoadZipModelAdder::setC1q(double c1q) {
    m_c1q = checkCoefficient(m_parent, c1q);
    return *this;
}
LoadZipModelAdder &LoadZipModelAdder::setC2q(double c2q) {
    m_c2q = checkCoefficient(m_parent, c2q);
    return *this;
}

LoadAdder& LoadZipModelAdder::add() {
    checkCoefficient(m_parent, m_c0p);
    checkCoefficient(m_parent, m_c1p);
    checkCoefficient(m_parent, m_c2p);
    checkCoefficient(m_parent, m_c0q);
    checkCoefficient(m_parent, m_c1q);
    checkCoefficient(m_parent, m_c2q);

    if (std::abs(m_c0p + m_c1p + m_c2p - 1.) > std::numeric_limits<double>::epsilon()) {
        throw ValidationException(m_parent, "Sum of c0p, c1p and c2p should be 1");
    }
    if (std::abs(m_c0q + m_c1q + m_c2q - 1.) > std::numeric_limits<double>::epsilon()) {
        throw ValidationException(m_parent, "Sum of c0q, c1q and c2q should be 1");
    }

    m_parent.setZipModelAdder(*this);

    return m_parent;
}
std::unique_ptr<LoadModel> LoadZipModelAdder::build() const {
    std::unique_ptr<LoadModel> ptrLoadModel = std::unique_ptr<LoadModel>(new LoadZipModel(m_c0p, m_c1p, m_c2p, m_c0q, m_c1q, m_c2q));

    return ptrLoadModel;
}

void LoadZipModelAdder::addNewModel(Load& load, double c0p, double c1p, double c2p, double c0q, double c1q, double c2q) {
    checkCoefficient(load, c0p);
    checkCoefficient(load, c1p);
    checkCoefficient(load, c2p);
    checkCoefficient(load, c0q);
    checkCoefficient(load, c1q);
    checkCoefficient(load, c2q);

    if (std::abs(c0p + c1p + c2p - 1.) > std::numeric_limits<double>::epsilon()) {
        throw ValidationException(load, "Sum of c0p, c1p and c2p should be 1");
    }
    if (std::abs(c0q + c1q + c2q - 1.) > std::numeric_limits<double>::epsilon()) {
        throw ValidationException(load, "Sum of c0q, c1q and c2q should be 1");
    }

    std::unique_ptr<LoadModel> ptrLoadModel = std::unique_ptr<LoadModel>(new LoadZipModel(c0p, c1p, c2p, c0q, c1q, c2q));
    load.setModel(std::move(ptrLoadModel));
}



}  // namespace iidm

}  // namespace powsybl