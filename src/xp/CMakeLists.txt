set(grail-xp
  Calendar.cc
  Stats.cc
  lzmadecode.cc
)

list(TRANSFORM grail-xp PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-xp ${grail-xp} PARENT_SCOPE)
