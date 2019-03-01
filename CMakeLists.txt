cmake_minimum_required(VERSION 2.8)
project(ctp-swig)

set(BUILD_SHARED_LIBS true)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_VERBOSE_MAKEFILE true)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(WIN32)
    set(CMAKE_INSTALL_PREFIX $ENV{HOMEPATH})
else()
    set(CMAKE_INSTALL_PREFIX $ENV{HOME})
endif()

set(CTP_SDK_DIR ${PROJECT_SOURCE_DIR}/20180730_ptest)

if(WIN32)
    set(CTP_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/ctp/api/trade/win/public)
    set(CTP_LIBRARY_DIRS ${PROJECT_SOURCE_DIR}/ctp/api/trade/win/lib ${PROJECT_SOURCE_DIR}/ctp/api/risk/lib)
elseif(UNIX)
    set(CTP_INCLUDE_DIRS
        ${CTP_SDK_DIR}/v6.3.13_20180730_api_tradeapi_se_linux64
        ${CTP_SDK_DIR}/v6.3.13_20180730_api_clientdatacollectdll_linux64)
    set(CTP_LIBRARY_DIRS
        ${CTP_SDK_DIR}/v6.3.13_20180730_api_tradeapi_se_linux64
        ${CTP_SDK_DIR}/v6.3.13_20180730_api_clientdatacollectdll_linux64)
    message(NOTICE "CTP_LIBRARY_DIRS: ${CTP_LIBRARY_DIRS}")
endif()

include_directories(${CTP_INCLUDE_DIRS})
link_directories(${CTP_LIBRARY_DIRS})

if(WIN32)
    find_library(CTP_MDUSER_LIBRARY NAMES thostmduserapi PATHS ${CTP_LIBRARY_DIRS})
    find_library(CTP_TRADE_LIBRARY NAMES thosttraderapi PATHS ${CTP_LIBRARY_DIRS})
    # find_library(CTP_RISK_LIBRARY NAMES riskuserapi PATHS ${CTP_LIBRARY_DIRS})
    if(CTP_MDUSER_LIBRARY AND CTP_TRADE_LIBRARY)  # AND CTP_RISK_LIBRARY)
        set(CTP_LIBRARIES ${CTP_MDUSER_LIBRARY} ${CTP_TRADE_LIBRARY}) # ${CTP_RISK_LIBRARY})
        set(CTP_SHAREDLIBFILES 
            # ${PROJECT_SOURCE_DIR}/ctp/api/risk/lib/riskuserapi.dll
            ${PROJECT_SOURCE_DIR}/ctp/api/trade/win/lib/thostmduserapi.dll
            ${PROJECT_SOURCE_DIR}/ctp/api/trade/win/lib/thosttraderapi.dll
        )
    endif()
elseif(UNIX)
#    find_library(CTP_MDUSER_LIBRARY NAMES thostmduserapi_se PATHS ${CTP_LIBRARY_DIRS} HINTS "")
#    find_library(CTP_TRADE_LIBRARY NAMES thosttraderapi_se PATHS ${CTP_LIBRARY_DIRS})
#    find_library(CTP_LINUX_DATA_COLLECT_LIBRARY NAMES LinuxDataCollect PATHS ${CTP_LIBRARY_DIRS})

    add_library(CTP_MDUSER_LIBRARY SHARED IMPORTED)
    set_target_properties( CTP_MDUSER_LIBRARY PROPERTIES IMPORTED_LOCATION ${CTP_SDK_DIR}/v6.3.13_20180730_api_tradeapi_se_linux64/thostmduserapi_se.so )
    message(NOTICE "CTP_MDUSER_LIBRARY: " CTP_MDUSER_LIBRARY)

    add_library(CTP_TRADE_LIBRARY SHARED IMPORTED)
    set_target_properties( CTP_TRADE_LIBRARY PROPERTIES IMPORTED_LOCATION ${CTP_SDK_DIR}/v6.3.13_20180730_api_tradeapi_se_linux64/thosttraderapi_se.so )

    add_library(CTP_LINUX_DATA_COLLECT_LIBRARY SHARED IMPORTED)
    set_target_properties( CTP_LINUX_DATA_COLLECT_LIBRARY PROPERTIES IMPORTED_LOCATION ${CTP_SDK_DIR}/v6.3.13_20180730_api_clientdatacollectdll_linux64/LinuxDataCollect.so )

    set(CTP_LIBRARIES CTP_MDUSER_LIBRARY CTP_TRADE_LIBRARY CTP_LINUX_DATA_COLLECT_LIBRARY)
    set(CTP_SHAREDLIBFILES
        ${CTP_SDK_DIR}/v6.3.13_20180730_api_tradeapi_se_linux64/thostmduserapi_se.so
        ${CTP_SDK_DIR}/v6.3.13_20180730_api_tradeapi_se_linux64/thosttraderapi_se.so
        ${CTP_SDK_DIR}/v6.3.13_20180730_api_clientdatacollectdll_linux64/LinuxDataCollect.so
        )
    message(NOTICE " [OK] CTP_LIBRARIES: ${CTP_LIBRARIES}")
endif()

file(COPY ${CTP_SHAREDLIBFILES} DESTINATION "${CMAKE_INSTALL_PREFIX}/local/lib")
file(COPY ${CTP_SDK_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(SWIG REQUIRED)
if(SWIG_FOUND)
    include(${SWIG_USE_FILE})

    set(ENV{JAVA_HOME} /usr/lib/jvm/default)
    find_package(Java)
    find_package(JNI)
    find_package(Ruby)
    find_package(PythonInterp)
    find_package(PythonLibs)
    find_package(R)
    find_package(CSharp)
    find_package(Mono)
    # use $ENV{LUA_DIR}
    find_package(Lua51)
    find_package(PerlLibs)
    find_package(TCL)
    find_package(Octave)

    if(JNI_FOUND)
        add_subdirectory(java)
    endif()

    if(RUBY_FOUND)
        add_subdirectory(ruby)
    endif(RUBY_FOUND)

    if(R_FOUND)
#        add_subdirectory(R)
    endif(R_FOUND)

    if(PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND)
        add_subdirectory(python)
    endif()

    if(CSharp_FOUND OR MONO_FOUND)
#        add_subdirectory(csharp)
    endif()

    if(LUA51_FOUND)
#        add_subdirectory(lua)
    endif(LUA51_FOUND)

    if(TCL_FOUND)
#        add_subdirectory(tcl)
    endif(TCL_FOUND)

    if(PERLLIBS_FOUND)
#        add_subdirectory(perl)
    endif(PERLLIBS_FOUND)

    if(OCTAVE_FOUND)
#        add_subdirectory(octave)
    endif(OCTAVE_FOUND)
endif()

#add_subdirectory(code)
#add_subdirectory(test)
