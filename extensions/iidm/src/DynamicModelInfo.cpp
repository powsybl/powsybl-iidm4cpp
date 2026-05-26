/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/DynamicModelInfo.hpp>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

DynamicModelInfo::DynamicModelInfo(Identifiable& identifiable, const std::string& modelName) :
    AbstractMultiVariantIdentifiableExtension(identifiable) {
    unsigned long variantArraySize = getVariantManagerHolder().getVariantManager().getVariantArraySize();
    m_modelNames.resize(variantArraySize, modelName);
}

void DynamicModelInfo::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Identifiable>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Identifiable>()));
    }
}

const std::string& DynamicModelInfo::getName() const {
    static std::string s_name = "dynamicModel";
    return s_name;
}

const std::type_index& DynamicModelInfo::getType() const {
    static std::type_index s_type = typeid(DynamicModelInfo);
    return s_type;
}

const std::string& DynamicModelInfo::getModelName() const {
    return m_modelNames[getVariantIndex()];
}

DynamicModelInfo& DynamicModelInfo::setModelName(const std::string& modelName) {
    m_modelNames[getVariantIndex()] = modelName;
    return *this;
}

void DynamicModelInfo::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        m_modelNames[index] = m_modelNames[sourceIndex];
    }
}

void DynamicModelInfo::deleteVariantArrayElement(unsigned long /*index*/) {
    //nothing to do
}

void DynamicModelInfo::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_modelNames.resize(m_modelNames.size() + number, m_modelNames[sourceIndex]);
}

void DynamicModelInfo::reduceVariantArraySize(unsigned long number) {
    m_modelNames.resize(m_modelNames.size() - number);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
