/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_DYNAMICMODELINFO_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_DYNAMICMODELINFO_HPP

#include <powsybl/iidm/AbstractMultiVariantIdentifiableExtension.hpp>

namespace powsybl {

namespace iidm {

class Identifiable;

namespace extensions {

namespace iidm {

class DynamicModelInfo : public AbstractMultiVariantIdentifiableExtension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:
    const std::string& getModelName() const;

    DynamicModelInfo& setModelName(const std::string& modelName);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    DynamicModelInfo(Identifiable& identifiable, const std::string& modelName);

    friend class DynamicModelInfoAdder;

private:
    /**
     * dynamic model name, support MultiVariantObject 
     */
    std::vector<std::string> m_modelNames;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_DYNAMICMODELINFO_HPP
