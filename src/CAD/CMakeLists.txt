set(grail-CAD
    Triangle.cc
    Rectangle.cc
    Circle2D.cc
    Gear.cc
    Curve.cc
    Circle.cc
    BezierPath.cc
    Line.cc
    Facet.cc
    Helix.cc
)

list(TRANSFORM grail-CAD PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-CAD ${grail-CAD} PARENT_SCOPE)
