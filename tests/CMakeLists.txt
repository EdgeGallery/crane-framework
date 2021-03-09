function(crane_test test_name)
    set(TEST_EXECUTABLE run_${test_name})
    set(TEST_SOURCE ${test_name}.cpp)

    INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/inc")
    INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/crane/common/inc")

    include_directories("${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/msg/inc")
    include_directories("${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/mep/inc")
    include_directories("${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/srv/inc")
    
    include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
    include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneAdapter/inc")
    include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneCore/inc")
    include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneMep/inc")
    include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneServer/inc")
    include_directories("${PROJECT_SOURCE_DIR}/src/crane/gstFacade/inc")

    include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/playcd/cpp/inc")
    include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/playmp3/cpp/inc")

    include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/swappable/inc")

    #link_libraries(stdc++)
    add_executable(${TEST_EXECUTABLE} ${TEST_SOURCE})
    target_link_libraries(${TEST_EXECUTABLE} stdc++ gtest gtest_main crane dl curl pthread gstreamer-1.0 gobject-2.0 glib-2.0 boost_system crypto ssl cpprest)
    add_test(${test_name} ${TEST_EXECUTABLE})
endfunction()

configure_file("certs/server.crt" "certs/server.crt" COPYONLY)
configure_file("certs/server.key" "certs/server.key" COPYONLY)
configure_file("certs/rootCA.crt" "certs/rootCA.crt" COPYONLY)
configure_file("certs/client.key" "certs/client.key" COPYONLY)
configure_file("certs/client.crt" "certs/client.crt" COPYONLY)


#crane_test(jsontest)
crane_test(coreTest)
#crane_test(srvTest)
#crane_test(msgCenterTest)
crane_test(pluginInstMgntTest)
crane_test(wrapperTest)
crane_test(swapTest)
#crane_test(mepTest)
