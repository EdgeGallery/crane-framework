set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/crane")

add_library(crane SHARED 
                $<TARGET_OBJECTS:common> 
                $<TARGET_OBJECTS:cranecore> 
                $<TARGET_OBJECTS:gstfacade> 
                $<TARGET_OBJECTS:craneadapter> 
                #$<TARGET_OBJECTS:cranemep>
                #$<TARGET_OBJECTS:craneserver>
                $<TARGET_OBJECTS:cranewrapper>
                )
add_library(crane_static STATIC 
                $<TARGET_OBJECTS:common> 
                $<TARGET_OBJECTS:cranecore> 
                $<TARGET_OBJECTS:gstfacade> 
                $<TARGET_OBJECTS:craneadapter> 
                #$<TARGET_OBJECTS:cranemep>
                #$<TARGET_OBJECTS:craneserver>
                $<TARGET_OBJECTS:cranewrapper>
                )

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/crane")

#target_link_libraries(crane libpistache.a libssl.so libcrypto.so libpthread.so)
#target_link_libraries(crane_static libpistache.a libssl.so libcrypto.so libpthread.so)
#target_link_libraries(crane libpistache.a libssl.so libcrypto.so libpthread.so dl gstreamer-1.0 gobject-2.0 glib-2.0 boost_system crypto ssl cpprest)
target_link_libraries(crane pistache ssl libcrypto.so libpthread.so dl gstreamer-1.0 gobject-2.0 glib-2.0 boost_system crypto ssl cpprest)
target_link_libraries(crane_static libpistache.a libssl.so libcrypto.so libpthread.so dl gstreamer-1.0 gobject-2.0 glib-2.0 boost_system crypto ssl cpprest)           
#修改静态路的名称
set_target_properties(crane_static PROPERTIES OUTPUT_NAME "crane")

#set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/crane/plugins")
add_library(cranepluginsysmsg MODULE $<TARGET_OBJECTS:cranemsg>)
target_link_libraries(cranepluginsysmsg crane)

add_library(cranepluginsysmep MODULE $<TARGET_OBJECTS:cranemep>)
target_link_libraries(cranepluginsysmep boost_system pistache crypto ssl cpprest crane)

add_library(cranepluginsyssrv MODULE $<TARGET_OBJECTS:cranesrv>)
target_link_libraries(cranepluginsyssrv pistache ssl crane)

set(CRANE_PUBLIC_HEADERS 
        ${PROJECT_SOURCE_DIR}/src/inc/crane.h
        ${PROJECT_SOURCE_DIR}/src/inc/cranePlugin.h
        ${PROJECT_SOURCE_DIR}/src/crane/common/inc/craneMacro.h
        ${PROJECT_SOURCE_DIR}/src/crane/common/inc/Util.h
        ${PROJECT_SOURCE_DIR}/src/crane/common/inc/PluginBaseInterface.h
        ${PROJECT_SOURCE_DIR}/src/crane/common/inc/AbsPluginFactory.h
        ${PROJECT_SOURCE_DIR}/src/crane/craneAdapter/inc/AbsPluginFrame.h
        ${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/msg/inc/CraneMsgCenterPlugin.h
        ${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/srv/inc/CraneSrv.h
        ${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/mep/inc/CraneMep.h
)

set_target_properties(crane PROPERTIES PUBLIC_HEADER "${CRANE_PUBLIC_HEADERS}")

install(TARGETS crane crane_static cranepluginsysmep cranepluginsyssrv cranepluginsysmsg
    CONFIGURATIONS RELEASE
    LIBRARY DESTINATION lib/crane
    ARCHIVE DESTINATION lib/crane/static
    PUBLIC_HEADER DESTINATION include/crane
)
