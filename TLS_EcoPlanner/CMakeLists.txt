cmake_minimum_required(VERSION 3.5)

project(TLS_EcoPlanner LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TLS_EcoPlanner
	PCH.hpp
	main.cpp
	source/Misc.hpp
	source/mainwindow.hpp source/mainwindow.cpp
	source/mainwindow.ui
	source/Controller.hpp source/Controller.cpp
	source/FileOperations.hpp source/FileOperations.cpp
	source/Message.hpp

	stage/Stage.hpp stage/Stage.cpp
	stage/StageModel.hpp stage/StageModel.cpp

	history/History.hpp history/History.cpp
	history/HistoryModel.hpp history/HistoryModel.cpp
	history/HistoryResetColumnDelegate.hpp history/HistoryResetColumnDelegate.cpp
	history/HistorySerialization.hpp history/HistorySerialization.cpp

	history/action/HistoryAction.hpp history/action/HistoryAction.cpp
	history/action/MineActions.hpp history/action/MineActions.cpp
	history/action/HouseActions.hpp history/action/HouseActions.cpp
	history/action/ShopActions.hpp history/action/ShopActions.cpp
	history/action/DayStartAction.hpp history/action/DayStartAction.cpp
	history/action/RuinsActions.hpp history/action/RuinsActions.cpp

	game_data/GameData.hpp game_data/GameData.cpp
	game_data/WriteGameDataToXml.cpp
	game_data/ReadGameDataFromXml.cpp
	game_data/GameDataXml.hpp

	buildings/GoldMine.hpp buildings/GoldMine.cpp
	buildings/Shop.hpp buildings/Shop.cpp
	buildings/House.hpp buildings/House.cpp
	buildings/Ruins.hpp buildings/Ruins.cpp

	resources/icons.qrc
)

target_precompile_headers(TLS_EcoPlanner PRIVATE PCH.hpp)

include_directories("F:/Libraries/boost_1_86_0")

find_package(Qt5 REQUIRED COMPONENTS Widgets )
find_package(Qt5 REQUIRED COMPONENTS XmlPatterns )

add_link_options("-fuse-ld=bfd")
target_link_libraries(TLS_EcoPlanner PRIVATE Qt5::Widgets)
target_link_libraries(TLS_EcoPlanner PRIVATE Qt5::XmlPatterns)

configure_file(game_data.xsd game_data.xsd)
configure_file(plan_schema.xsd plan_schema.xsd)
configure_file(resources/Combinear/Combinear.qss resources/Combinear/Combinear.qss)
configure_file(resources/Combinear/License.txt resources/Combinear/License.txt)
