set(grail-CAD
    Curve.cc
    Cylinder.cc
    RectangularPrism.cc
    Sphere.cc
    Triangle.cc
    Rectangle.cc
    Circle2D.cc
    Gear.cc
    Curve.cc
    Cylinder.cc
    Transform.cc
    Circle.cc
    BezierPath.cc
    Line.cc
    Facet.cc
    Helix.cc
    Extrusion.cc
)

list(TRANSFORM grail-CAD PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-CAD ${grail-CAD} PARENT_SCOPE)
