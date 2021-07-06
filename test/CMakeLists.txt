add_executable(testWidgets testWidgets.cc)
add_executable(solarSystem SolarSystem.cc)
add_executable(testAudio testAudio.cc)
add_executable(ESRIMapDemo ESRIMapDemo.cc)

target_link_libraries(testWidgets grail)
target_link_libraries(solarSystem grail)
target_link_libraries(testAudio grail)
target_link_libraries(ESRIMapDemo grail)

# setting up a new executable, only needs the two lines
