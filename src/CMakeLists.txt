#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(IIDM_SOURCES
    AssertionError.cpp
    PowsyblException.cpp

    iidm/AbstractComponentsManager.cpp
    iidm/AbstractMultiVariantIdentifiableExtension.cpp
    iidm/AbstractTerminalTopologyVisitor.cpp
    iidm/ActivePowerLimits.cpp
    iidm/ActivePowerLimitsAdder.cpp
    iidm/ApparentPowerLimits.cpp
    iidm/ApparentPowerLimitsAdder.cpp
    iidm/Battery.cpp
    iidm/BatteryAdder.cpp
    iidm/Boundary.cpp
    iidm/Branch.cpp
    iidm/Bus.cpp
    iidm/BusAdder.cpp
    iidm/BusbarSection.cpp
    iidm/BusbarSectionAdder.cpp
    iidm/BusBreakerVoltageLevel.cpp
    iidm/BusBreakerVoltageLevelBusCache.cpp
    iidm/BusBreakerVoltageLevelTopology.cpp
    iidm/BusBreakerVoltageLevelVariant.cpp
    iidm/BusBreakerVoltageLevelViews.cpp
    iidm/BusCache.cpp
    iidm/BusRef.cpp
    iidm/BusTerminal.cpp
    iidm/BusTerminalViews.cpp
    iidm/CalculatedBus.cpp
    iidm/Component.cpp
    iidm/ConfiguredBus.cpp
    iidm/Connectable.cpp
    iidm/ConnectableType.cpp
    iidm/ConnectedComponent.cpp
    iidm/ConnectedComponentsManager.cpp
    iidm/Container.cpp
    iidm/Country.cpp
    iidm/CurrentLimits.cpp
    iidm/CurrentLimitsAdder.cpp
    iidm/DanglingLine.cpp
    iidm/DanglingLineAdder.cpp
    iidm/DanglingLineBoundary.cpp
    iidm/DanglingLineGeneration.cpp
    iidm/DanglingLineGenerationAdder.cpp
    iidm/EnergySource.cpp
    iidm/Extendable.cpp
    iidm/Extension.cpp
    iidm/ExtensionAdder.cpp
    iidm/ExtensionProvider.cpp
    iidm/ExtensionProviders.cpp
    iidm/FlowsLimitsHolder.cpp
    iidm/Generator.cpp
    iidm/GeneratorAdder.cpp
    iidm/HalfLine.cpp
    iidm/HalfLineAdder.cpp
    iidm/HalfLineBoundary.cpp
    iidm/HvdcConverterStation.cpp
    iidm/HvdcLine.cpp
    iidm/HvdcLineAdder.cpp
    iidm/IdBasedBusRef.cpp
    iidm/Identifiable.cpp
    iidm/Injection.cpp
    iidm/InternalConnection.cpp
    iidm/InternalConnectionAdder.cpp
    iidm/LccConverterStation.cpp
    iidm/LccConverterStationAdder.cpp
    iidm/LimitType.cpp
    iidm/Line.cpp
    iidm/LineAdder.cpp
    iidm/LineCharacteristics.cpp
    iidm/Load.cpp
    iidm/LoadAdder.cpp
    iidm/LoadingLimits.cpp
    iidm/LoadType.cpp
    iidm/MergedBus.cpp
    iidm/MinMaxReactiveLimits.cpp
    iidm/MinMaxReactiveLimitsAdder.cpp
    iidm/MultipleVariantContext.cpp
    iidm/Network.cpp
    iidm/NetworkIndex.cpp
    iidm/NetworkRef.cpp
    iidm/NetworkVariant.cpp
    iidm/NetworkViews.cpp
    iidm/NodeBreakerVoltageLevel.cpp
    iidm/NodeBreakerVoltageLevelBusCache.cpp
    iidm/NodeBreakerVoltageLevelBusNamingStrategy.cpp
    iidm/NodeBreakerVoltageLevelTopology.cpp
    iidm/NodeBreakerVoltageLevelVariant.cpp
    iidm/NodeBreakerVoltageLevelViews.cpp
    iidm/NodeTerminal.cpp
    iidm/NodeTerminalViews.cpp
    iidm/OperationalLimits.cpp
    iidm/OperationalLimitsHolder.cpp
    iidm/PhaseTapChanger.cpp
    iidm/PhaseTapChangerAdder.cpp
    iidm/PhaseTapChangerHolder.cpp
    iidm/PhaseTapChangerStep.cpp
    iidm/RatioTapChanger.cpp
    iidm/RatioTapChangerAdder.cpp
    iidm/RatioTapChangerHolder.cpp
    iidm/RatioTapChangerStep.cpp
    iidm/ReactiveCapabilityCurve.cpp
    iidm/ReactiveCapabilityCurveAdder.cpp
    iidm/ReactiveLimitsHolder.cpp
    iidm/ShuntCompensator.cpp
    iidm/ShuntCompensatorAdder.cpp
    iidm/ShuntCompensatorLinearModel.cpp
    iidm/ShuntCompensatorLinearModelAdder.cpp
    iidm/ShuntCompensatorModelType.cpp
    iidm/ShuntCompensatorNonLinearModel.cpp
    iidm/ShuntCompensatorNonLinearModelAdder.cpp
    iidm/StaticVarCompensator.cpp
    iidm/StaticVarCompensatorAdder.cpp
    iidm/Substation.cpp
    iidm/SubstationAdder.cpp
    iidm/Switch.cpp
    iidm/SwitchAdder.cpp
    iidm/SwitchKind.cpp
    iidm/SynchronousComponent.cpp
    iidm/SynchronousComponentsManager.cpp
    iidm/Terminal.cpp
    iidm/TerminalBuilder.cpp
    iidm/ThreadLocalMultipleVariantContext.cpp
    iidm/ThreeWindingsTransformer.cpp
    iidm/ThreeWindingsTransformerLeg.cpp
    iidm/ThreeWindingsTransformerLegAdder.cpp
    iidm/ThreeWindingsTransformerAdder.cpp
    iidm/TieLine.cpp
    iidm/TieLineAdder.cpp
    iidm/TopologyKind.cpp
    iidm/TopologyLevel.cpp
    iidm/TopologyVisitor.cpp
    iidm/TwoWindingsTransformer.cpp
    iidm/TwoWindingsTransformerAdder.cpp
    iidm/ValidationException.cpp
    iidm/ValidationUtils.cpp
    iidm/VariantContextGuard.cpp
    iidm/VariantManager.cpp
    iidm/VoltageLevel.cpp
    iidm/VoltageLevelAdder.cpp
    iidm/VscConverterStation.cpp
    iidm/VscConverterStationAdder.cpp

    iidm/converter/BusFilter.cpp
    iidm/converter/Constants.cpp
    iidm/converter/ConversionParameters.cpp
    iidm/converter/ExportOptions.cpp
    iidm/converter/FakeAnonymizer.cpp
    iidm/converter/ImportOptions.cpp
    iidm/converter/Parameter.cpp
    iidm/converter/SimpleAnonymizer.cpp

    iidm/converter/xml/AbstractExtensionXmlSerializer.cpp
    iidm/converter/xml/AbstractVersionableExtensionXmlSerializer.cpp
    iidm/converter/xml/AliasesXml.cpp
    iidm/converter/xml/BatteryXml.cpp
    iidm/converter/xml/BusbarSectionXml.cpp
    iidm/converter/xml/BusBreakerViewSwitchXml.cpp
    iidm/converter/xml/BusXml.cpp
    iidm/converter/xml/DanglingLineXml.cpp
    iidm/converter/xml/ExtensionXmlSerializer.cpp
    iidm/converter/xml/GeneratorXml.cpp
    iidm/converter/xml/HvdcLineXml.cpp
    iidm/converter/xml/IidmXmlVersion.cpp
    iidm/converter/xml/IidmXmlUtil.cpp
    iidm/converter/xml/LccConverterStationXml.cpp
    iidm/converter/xml/LineXml.cpp
    iidm/converter/xml/LoadXml.cpp
    iidm/converter/xml/NetworkXml.cpp
    iidm/converter/xml/NetworkXmlReaderContext.cpp
    iidm/converter/xml/NetworkXmlWriterContext.cpp
    iidm/converter/xml/NodeBreakerViewInternalConnectionXml.cpp
    iidm/converter/xml/NodeBreakerViewSwitchXml.cpp
    iidm/converter/xml/PropertiesXml.cpp
    iidm/converter/xml/ReactiveLimitsXml.cpp
    iidm/converter/xml/ShuntCompensatorXml.cpp
    iidm/converter/xml/StaticVarCompensatorXml.cpp
    iidm/converter/xml/SubstationXml.cpp
    iidm/converter/xml/TerminalRefXml.cpp
    iidm/converter/xml/ThreeWindingsTransformerXml.cpp
    iidm/converter/xml/TieLineXml.cpp
    iidm/converter/xml/TwoWindingsTransformerXml.cpp
    iidm/converter/xml/VersionsCompatibility.cpp
    iidm/converter/xml/VoltageLevelXml.cpp
    iidm/converter/xml/VscConverterStationXml.cpp

    iidm/extensions/LoadDetail.cpp
    iidm/extensions/LoadDetailAdder.cpp
    iidm/extensions/LoadDetailXmlSerializer.cpp
    iidm/extensions/SlackTerminal.cpp
    iidm/extensions/SlackTerminalAdder.cpp
    iidm/extensions/SlackTerminalXmlSerializer.cpp

    iidm/util/ConnectedComponents.cpp
    iidm/util/DistinctPredicate.cpp
    iidm/util/Identifiables.cpp
    iidm/util/LimitViolationUtils.cpp
    iidm/util/LinkData.cpp
    iidm/util/Networks.cpp
    iidm/util/Substations.cpp
    iidm/util/SV.cpp
    iidm/util/SwitchesFlow.cpp
    iidm/util/TerminalFinder.cpp
    iidm/util/VoltageLevels.cpp

    logging/ConsoleLogger.cpp
    logging/ContainerLogger.cpp
    logging/Level.cpp
    logging/LoggerFactory.cpp
    logging/LogMessage.cpp
    logging/NoopLogger.cpp

    math/ConnectedComponentsComputationResult.cpp
    math/GraphUtil.cpp

    network/BatteryNetworkFactory.cpp
    network/EurostagFactory.cpp
    network/FictitiousSwitchFactory.cpp
    network/FourSubstationsNodeBreakerFactory.cpp
    network/LoadBarExt.cpp
    network/LoadFooExt.cpp
    network/MultipleExtensionsTestNetworkFactory.cpp
    network/ThreeWindingsTransformerNetworkFactory.cpp

    stdcxx/DateTime.cpp
    stdcxx/demangle.cpp
    stdcxx/exception.cpp
    stdcxx/Properties.cpp
    stdcxx/time.cpp

    xml/XmlCharConversion.cpp
    xml/XmlStreamException.cpp
    xml/XmlStreamReader.cpp
    xml/XmlStreamWriter.cpp
)

# Shared library
add_library(iidm SHARED ${IIDM_SOURCES})
set_target_properties(iidm PROPERTIES SOVERSION ${IIDM_SOVERSION} VERSION ${IIDM_VERSION})
target_include_directories(iidm SYSTEM
    PUBLIC
        $<TARGET_PROPERTY:Boost::boost,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:LibXml2::LibXml2,INTERFACE_INCLUDE_DIRECTORIES>
)
target_include_directories(iidm
    INTERFACE
        $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(iidm PUBLIC LibXml2::LibXml2 Boost::disable_autolinking Boost::date_time Boost::dynamic_linking Boost::filesystem Boost::system ${CMAKE_DL_LIBS} Threads::Threads)

# Static library
add_library(iidm-static STATIC ${IIDM_SOURCES})
set_target_properties(iidm-static PROPERTIES OUTPUT_NAME iidm)
target_include_directories(iidm-static SYSTEM
    PUBLIC
        $<TARGET_PROPERTY:Boost::boost,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:LibXml2::LibXml2,INTERFACE_INCLUDE_DIRECTORIES>
)
target_include_directories(iidm-static
    INTERFACE
        $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(iidm-static PUBLIC LibXml2::LibXml2 Boost::disable_autolinking Boost::date_time Boost::dynamic_linking Boost::filesystem Boost::system ${CMAKE_DL_LIBS} Threads::Threads)

# Installation
install(TARGETS iidm iidm-static
    EXPORT iidm-targets
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/powsybl
    DESTINATION ${INSTALL_INCLUDE_DIR}
    PATTERN "test/*" EXCLUDE
)

if (BUILD_TESTS)
    add_subdirectory(test)
endif()
