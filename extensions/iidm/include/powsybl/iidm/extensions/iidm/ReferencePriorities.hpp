/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCEPRIORITIES_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCEPRIORITIES_HPP

#include <vector>
#include <map>

#include <powsybl/iidm/AbstractMultiVariantIdentifiableExtension.hpp>
#include <powsybl/iidm/IdentifiableType.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePriority.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Connectable;
class Network;

namespace extensions {

namespace iidm {

class ReferencePriorityAdder;

class ReferencePriorities : public AbstractMultiVariantIdentifiableExtension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:
    ~ReferencePriorities() noexcept override = default;

    ReferencePriorities& add(const std::shared_ptr<ReferencePriority>& referencePriority);

    ReferencePriorityAdder newReferencePriority();

    std::vector<std::shared_ptr<ReferencePriority>>& getReferencePriorities();

    stdcxx::const_range<ReferencePriority> getReferencePriorities() const;

    void deleteReferencePriorities();

    /**
     * Gets the reference priorities defined in the given network,
     * sorted by decreasing priority (i.e. higher priorities are first in the list).
     * Priorities 0 are filtered out.
     * @param network network from which reference priorities should be listed
     */
    static std::vector<std::shared_ptr<ReferencePriority>> get(Network& network);
    /**
     * Deletes all defined reference priorities in the network for the current variant
     * @param network network whose reference priorities should be deleted
     */
    static void deleteReferencePriorities(Network& network);


protected:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    explicit ReferencePriorities(Connectable& connectable);
    friend class ReferencePrioritiesAdder;

private:
    std::vector<std::vector<std::shared_ptr<ReferencePriority>>> m_referencePriorities;

    static std::map<IdentifiableType, unsigned long> DEFAULT_CONNECTABLE_TYPE_PRIORITIES;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCEPRIORITIES_HPP
