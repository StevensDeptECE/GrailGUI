set(grail-CAD
    Curve.cc
    Circle.cc
    BezierPath.cc
)

list(TRANSFORM grail-CAD PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-CAD ${grail-CAD} PARENT_SCOPE)
