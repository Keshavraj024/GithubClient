cmake_minimum_required(VERSION 3.16)

project(GithubClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 Sql)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(GithubClient
    main.cpp
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS Source/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS Include/*.h)

# always set the property first
set_source_files_properties(UI/Components/Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(GithubClient
        URI GithubClient
        VERSION 1.0
        QML_FILES
            UI/Main.qml

            UI/Components/RepoCard.qml
            UI/Components/Theme.qml
            UI/Components/SearchBar.qml
            UI/Components/ThemeToggleSwitch.qml
            UI/Components/TokenInput.qml
            UI/Components/StatusBar.qml
            UI/Components/EmptyStateFeedback.qml
            UI/Components/LoadingOverlay.qml
            UI/Components/Header.qml
            UI/Components/ToolBar.qml
            UI/Components/ErrorPopup.qml
            UI/Components/SaveDialog.qml

            UI/Controls/ColorBehavior.qml

            UI/Customs/CustomButton.qml
        SOURCES
            ${SOURCES}
            ${HEADERS}

        RESOURCES
            Assets/images/App-Logo.png
            Assets/images/sun.png
            Assets/images/moon.png
            Assets/icons/search.png
        )


target_include_directories(GithubClient PRIVATE Include/Repository Include/User Include/Api Include/DatabaseManager Include/DataModels)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(GithubClient PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appGithubClient
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(GithubClient
    PRIVATE Qt6::Quick Qt6::Sql
    PRIVATE Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS GithubClient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
