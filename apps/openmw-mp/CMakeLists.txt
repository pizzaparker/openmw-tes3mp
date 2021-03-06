project(tes3mp-server)

option(ENABLE_BREAKPAD "Enable Google Breakpad for Crash reporting" OFF)

if(ENABLE_BREAKPAD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_BREAKPAD")
    if (UNIX)
        set(Breakpad_Headers "${CMAKE_SOURCE_DIR}/extern/breakpad/src/client/linux")
        set(Breakpad_Library "${CMAKE_SOURCE_DIR}/extern/breakpad/src/client/linux/libbreakpad_client.a")
    elseif(WIN32)
        set(Breakpad_Headers "${CMAKE_SOURCE_DIR}/extern/breakpad/src/client/windows")
        set(Breakpad_Library "-lbreakpad_client")
    endif (UNIX)
    include_directories(${CMAKE_SOURCE_DIR}/extern/breakpad/src ${Breakpad_Headers})
endif(ENABLE_BREAKPAD)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_LUA")

# local files
set(SERVER
    main.cpp
    Player.cpp Players.cpp
    Networking.cpp
    MasterClient.cpp
    Cell.cpp
    CellController.cpp
    Utils.cpp
    CharClass.cpp
    Inventory.cpp
    Settings.cpp
    Timer.cpp
    Books.cpp
    GUI.cpp
    Dialogue.cpp
    Factions.cpp
    Cells.cpp
    Quests.cpp
    Spells.cpp
    Actors.cpp
    NetActor.cpp
    CellState.cpp
    Object.cpp
    stacktrace.cpp
    Window.cpp
    Weather.cpp
    BaseMgr.cpp
    Script/CommandController.cpp Script/EventController.cpp Script/LuaState.cpp Script/luaUtils.cpp
)

if(WIN32)
    list(APPEND SERVER stackwalker/StackWalker.cpp)
endif()

set(SERVER_HEADER

)
source_group(tes3mp-server FILES ${SERVER} ${SERVER_HEADER})

set(PROCESSORS_ACTOR
        processors/actor/ProcessorActorAnimFlags.hpp processors/actor/ProcessorActorAnimPlay.hpp
        processors/actor/ProcessorActorAttack.hpp processors/actor/ProcessorActorCellChange.hpp
        processors/actor/ProcessorActorEquipment.hpp processors/actor/ProcessorActorInteraction.hpp
        processors/actor/ProcessorActorList.hpp processors/actor/ProcessorActorPosition.hpp
        processors/actor/ProcessorActorSpeech.hpp processors/actor/ProcessorActorStatsDynamic.hpp
        processors/actor/ProcessorActorTest.hpp
        )

source_group(tes3mp-server\\processors\\actor FILES ${PROCESSORS_ACTOR})

set(PROCESSORS_PLAYER
        processors/player/ProcessorChatMsg.hpp processors/player/ProcessorGUIMessageBox.hpp
        processors/player/ProcessorGUIWindow.hpp

        processors/player/ProcessorRecordDynamic.hpp processors/player/ProcessorGameWeather.hpp
        processors/player/ProcessorPlayerAnimFlags.hpp processors/player/ProcessorPlayerAnimPlay.hpp
        processors/player/ProcessorPlayerAttack.hpp processors/player/ProcessorPlayerAttribute.hpp
        processors/player/ProcessorPlayerBook.hpp processors/player/ProcessorPlayerBounty.hpp
        processors/player/ProcessorPlayerCellChange.hpp processors/player/ProcessorPlayerCellState.hpp
        processors/player/ProcessorPlayerCharClass.hpp processors/player/ProcessorPlayerCharGen.hpp
        processors/player/ProcessorPlayerDeath.hpp processors/player/ProcessorPlayerDisposition.hpp
        processors/player/ProcessorPlayerEquipment.hpp processors/player/ProcessorPlayerFaction.hpp
        processors/player/ProcessorPlayerInteraction.hpp processors/player/ProcessorPlayerInventory.hpp
        processors/player/ProcessorPlayerJournal.hpp processors/player/ProcessorPlayerKillCount.hpp
        processors/player/ProcessorPlayerLevel.hpp processors/player/ProcessorPlayerMap.hpp
        processors/player/ProcessorPlayerMiscellaneous.hpp processors/player/ProcessorPlayerPosition.hpp
        processors/player/ProcessorPlayerQuickKeys.hpp processors/player/ProcessorPlayerRest.hpp
        processors/player/ProcessorPlayerResurrect.hpp processors/player/ProcessorPlayerShapeshift.hpp
        processors/player/ProcessorPlayerSkill.hpp processors/player/ProcessorPlayerSpeech.hpp
        processors/player/ProcessorPlayerSpellbook.hpp processors/player/ProcessorPlayerStatsDynamic.hpp
        processors/player/ProcessorPlayerTopic.hpp
        )

source_group(tes3mp-server\\processors\\player FILES ${PROCESSORS_PLAYER})

set(PROCESSORS_OBJECT
        processors/object/ProcessorContainer.hpp processors/object/ProcessorDoorState.hpp
        processors/object/ProcessorMusicPlay.hpp processors/object/ProcessorObjectAnimPlay.hpp
        processors/object/ProcessorObjectDelete.hpp processors/object/ProcessorObjectLock.hpp
        processors/object/ProcessorObjectMove.hpp processors/object/ProcessorObjectPlace.hpp
        processors/object/ProcessorObjectRotate.hpp processors/object/ProcessorObjectScale.hpp
        processors/object/ProcessorObjectSpawn.hpp processors/object/ProcessorObjectState.hpp
        processors/object/ProcessorObjectTrap.hpp processors/object/ProcessorScriptLocalShort.hpp
        processors/object/ProcessorScriptLocalFloat.hpp processors/object/ProcessorScriptMemberShort.hpp
        processors/object/ProcessorScriptMemberFloat.hpp processors/object/ProcessorScriptGlobalShort.hpp
        processors/object/ProcessorScriptGlobalFloat.hpp processors/object/ProcessorVideoPlay.hpp
        )

source_group(tes3mp-server\\processors\\object FILES ${PROCESSORS_OBJECT})

set(PROCESSORS
        processors/ProcessorInitializer.cpp
        processors/PlayerProcessor.cpp
        processors/ActorProcessor.cpp
        processors/ObjectProcessor.cpp
        processors/WorldstateProcessor.cpp
        )

source_group(tes3mp-server\\processors FILES ${PROCESSORS})

include_directories("./")

# Main executable

add_executable(tes3mp-server
        ${SERVER} ${SERVER_HEADER}
        ${PROCESSORS_ACTOR} ${PROCESSORS_PLAYER} ${PROCESSORS_OBJECT} ${PROCESSORS}
        ${APPLE_BUNDLE_RESOURCES}
        )

# For Lua debugging
target_compile_definitions(tes3mp-server PRIVATE $<$<CONFIG:Debug>:SOL_SAFE_FUNCTIONS> $<$<CONFIG:RelWithDebInfo>:SOL_SAFE_FUNCTIONS>)
target_compile_definitions(tes3mp-server PRIVATE $<$<CONFIG:Debug>:SOL_SAFE_USERTYPE> $<$<CONFIG:RelWithDebInfo>:SOL_SAFE_USERTYPE>)
target_compile_definitions(tes3mp-server PRIVATE $<$<CONFIG:Debug>:SERVER_DEBUG> $<$<CONFIG:RelWithDebInfo>:SERVER_DEBUG>)

set_property(TARGET tes3mp-server PROPERTY CXX_STANDARD 14)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(tes3mp-server PRIVATE -Wno-ignored-qualifiers -ftemplate-depth=2048)
endif()

target_link_libraries(tes3mp-server
    #${Boost_SYSTEM_LIBRARY}
    #${Boost_THREAD_LIBRARY}
    #${Boost_FILESYSTEM_LIBRARY}
    #${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${RakNet_LIBRARY}
    components
    ${LuaJit_LIBRARIES}
    ${Breakpad_Library}
)

if (UNIX)
    target_link_libraries(tes3mp-server dl)
    # Fix for not visible pthreads functions for linker with glibc 2.15
    if(NOT APPLE)
        target_link_libraries(tes3mp-server ${CMAKE_THREAD_LIBS_INIT})
    endif(NOT APPLE)
endif(UNIX)

if (BUILD_WITH_CODE_COVERAGE)
  add_definitions (--coverage)
  target_link_libraries(tes3mp-server gcov)
endif()

if (MSVC)
    # Debug version needs increased number of sections beyond 2^16
    if (CMAKE_CL_64)
        set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
    endif (CMAKE_CL_64)
    add_definitions("-D_USE_MATH_DEFINES")
endif (MSVC)
