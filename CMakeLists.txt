cmake_minimum_required(VERSION 3.14)

project(noonoo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.5
  GIT_SHALLOW    TRUE
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

set(SOURCES
  src/main.cpp
  src/Game.cpp
  src/Renderer.cpp
  src/Question.cpp
  src/QuestionFactory.cpp
  src/JsonHistoryRepository.cpp
  src/Mascot.cpp
  src/states/SelectingDifficultyState.cpp
  src/states/AskingState.cpp
  src/states/ShowingResultState.cpp
  src/states/GameOverState.cpp
  src/states/HistoryState.cpp
  src/states/SessionDetailState.cpp
)

add_executable(noonoo ${SOURCES})

target_include_directories(noonoo PRIVATE include)
target_compile_options(noonoo PRIVATE -Wall -Wextra)
target_link_libraries(noonoo PRIVATE raylib)

add_custom_command(TARGET noonoo POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:noonoo>/assets
)
