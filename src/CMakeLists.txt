#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(IIDM_SOURCES
    AssertionError.cpp
    PowsyblException.cpp

    iidm/AbstractMultiVariantConnectableExtension.cpp
    iidm/Battery.cpp
    iidm/BatteryAdder.cpp
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
    iidm/BusTerminal.cpp
    iidm/BusTerminalViews.cpp
    iidm/CalculatedBus.cpp
    iidm/ConfiguredBus.cpp
    iidm/Connectable.cpp
    iidm/ConnectableType.cpp
    iidm/Container.cpp
    iidm/Country.cpp
    iidm/CurrentLimits.cpp
    iidm/DanglingLine.cpp
    iidm/DanglingLineAdder.cpp
    iidm/EnergySource.cpp
    iidm/Extendable.cpp
    iidm/Extension.cpp
    iidm/ExtensionProvider.cpp
    iidm/Generator.cpp
    iidm/GeneratorAdder.cpp
    iidm/HvdcConverterStation.cpp
    iidm/HvdcLine.cpp
    iidm/HvdcLineAdder.cpp
    iidm/Identifiable.cpp
    iidm/Injection.cpp
    iidm/InternalConnection.cpp
    iidm/InternalConnectionAdder.cpp
    iidm/LccConverterStation.cpp
    iidm/LccConverterStationAdder.cpp
    iidm/Line.cpp
    iidm/LineAdder.cpp
    iidm/LineCharacteristics.cpp
    iidm/Load.cpp
    iidm/LoadAdder.cpp
    iidm/LoadType.cpp
    iidm/MergedBus.cpp
    iidm/MinMaxReactiveLimits.cpp
    iidm/MinMaxReactiveLimitsAdder.cpp
    iidm/MultipleVariantContext.cpp
    iidm/Network.cpp
    iidm/NetworkIndex.cpp
    iidm/NetworkViews.cpp
    iidm/NodeBreakerVoltageLevel.cpp
    iidm/NodeBreakerVoltageLevelBusCache.cpp
    iidm/NodeBreakerVoltageLevelBusNamingStrategy.cpp
    iidm/NodeBreakerVoltageLevelTopology.cpp
    iidm/NodeBreakerVoltageLevelVariant.cpp
    iidm/NodeBreakerVoltageLevelViews.cpp
    iidm/NodeTerminal.cpp
    iidm/NodeTerminalViews.cpp
    iidm/PhaseTapChanger.cpp
    iidm/PhaseTapChangerAdder.cpp
    iidm/PhaseTapChangerStep.cpp
    iidm/Properties.cpp
    iidm/RatioTapChanger.cpp
    iidm/RatioTapChangerAdder.cpp
    iidm/RatioTapChangerStep.cpp
    iidm/ReactiveCapabilityCurve.cpp
    iidm/ReactiveCapabilityCurveAdder.cpp
    iidm/ReactiveLimitsHolder.cpp
    iidm/ShuntCompensator.cpp
    iidm/ShuntCompensatorAdder.cpp
    iidm/StaticVarCompensator.cpp
    iidm/StaticVarCompensatorAdder.cpp
    iidm/Substation.cpp
    iidm/SubstationAdder.cpp
    iidm/Switch.cpp
    iidm/SwitchAdder.cpp
    iidm/SwitchKind.cpp
    iidm/Terminal.cpp
    iidm/TerminalBuilder.cpp
    iidm/ThreeWindingsTransformer.cpp
    iidm/ThreeWindingsTransformerAdder.cpp
    iidm/TieLine.cpp
    iidm/TieLineAdder.cpp
    iidm/TopologyKind.cpp
    iidm/TopologyLevel.cpp
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
    iidm/converter/ExportOptions.cpp
    iidm/converter/FakeAnonymizer.cpp
    iidm/converter/ImportOptions.cpp

    iidm/converter/xml/BatteryXml.cpp
    iidm/converter/xml/BusbarSectionXml.cpp
    iidm/converter/xml/BusBreakerViewSwitchXml.cpp
    iidm/converter/xml/BusXml.cpp
    iidm/converter/xml/CurrentLimitsXml.cpp
    iidm/converter/xml/DanglingLineXml.cpp
    iidm/converter/xml/ExtensionXmlSerializer.cpp
    iidm/converter/xml/GeneratorXml.cpp
    iidm/converter/xml/HvdcLineXml.cpp
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
    iidm/converter/xml/VoltageLevelXml.cpp
    iidm/converter/xml/VscConverterStationXml.cpp

    iidm/util/DistinctPredicate.cpp
    iidm/util/LimitViolationUtils.cpp

    logging/ConsoleLogger.cpp
    logging/ContainerLogger.cpp
    logging/Level.cpp
    logging/LoggerFactory.cpp
    logging/LogMessage.cpp
    logging/NoopLogger.cpp

    network/BatteryNetworkFactory.cpp
    network/EurostagFactory.cpp

    stdcxx/DateTime.cpp
    stdcxx/demangle.cpp
    stdcxx/put_time.cpp

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
target_link_libraries(iidm PUBLIC LibXml2::LibXml2 Boost::filesystem Boost::system ${CMAKE_DL_LIBS})

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
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(iidm-static PUBLIC LibXml2::LibXml2 Boost::filesystem Boost::system ${CMAKE_DL_LIBS})

# Installation
install(TARGETS iidm iidm-static
    EXPORT iidm-targets
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/powsybl
    DESTINATION ${INSTALL_INCLUDE_DIR}
    PATTERN "test/*" EXCLUDE
)

add_subdirectory(test)
