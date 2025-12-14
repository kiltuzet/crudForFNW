cmake_minimum_required(VERSION 3.16)

project(untitled1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core Sql)
find_package(Qt6 REQUIRED COMPONENTS Core)


qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appuntitled1
    main.cpp
)

qt_add_qml_module(appuntitled1
    URI untitled1
    QML_FILES
        Main.qml
        SOURCES nutritionconverter.h nutritionconverter.cpp
        SOURCES
        SOURCES database/databaseconnection.cpp database/databaseconnection.h database/databaseinitializer.cpp database/databaseinitializer.h repos/productsrepository.cpp repos/productsrepository.h
        SOURCES repos/achievementsrepository.h repos/achievementsrepository.cpp
        SOURCES repos/usersrepository.h repos/usersrepository.cpp
        SOURCES repos/recipesrepository.h repos/recipesrepository.cpp
        SOURCES repos/recipeingredientsrepository.h repos/recipeingredientsrepository.cpp
        SOURCES
        SOURCES repos/emotionsrepository.h repos/emotionsrepository.cpp
        SOURCES repos/consumedrepository.h repos/consumedrepository.cpp
        SOURCES repos/exerciseentriesrepository.h repos/exerciseentriesrepository.cpp
        SOURCES repos/dailystatisticsrepository.h repos/dailystatisticsrepository.cpp
        RESOURCES resources.qrc
        SOURCES
        SOURCES
        QML_FILES PaginatedListView.qml
        SOURCES repos/BaseRepository.h
        SOURCES
        SOURCES repos/BaseRepository.cpp
        SOURCES services/BaseService.h
        SOURCES services/BaseService.cpp
        SOURCES services/productservice.h services/productservice.cpp
        SOURCES services/recipeservice.h services/recipeservice.cpp
        SOURCES services/usersservice.h services/usersservice.cpp
        SOURCES services/emotionsservice.h services/emotionsservice.cpp
        SOURCES services/acheivmentsservice.h services/acheivmentsservice.cpp
        SOURCES services/dailystatistics.h services/dailystatistics.cpp
        SOURCES services/consumedservice.h services/consumedservice.cpp
        SOURCES services/exerciseservice.h services/exerciseservice.cpp
        #SOURCES service/BaseService.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appuntitled1 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appuntitled1
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appuntitled1
    PRIVATE Qt6::Quick
    Qt6::Sql
)
qt6_add_resources(appuntitled1 "application_resources"
    PREFIX "/"
    FILES
        bdext.sql
)
target_link_libraries(appuntitled1 PRIVATE Qt6::Core)
target_link_libraries(appuntitled1 PRIVATE Qt6::Core)


include(GNUInstallDirs)
install(TARGETS appuntitled1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
