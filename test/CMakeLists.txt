add_executable(testWidgets testWidgets.cc)
add_executable(solarSystem SolarSystem.cc)
add_executable(testAudio testAudio.cc)

target_link_libraries(testWidgets grail)
target_link_libraries(solarSystem grail)
target_link_libraries(testAudio grail)

# setting up a new executable, only needs the two lines
