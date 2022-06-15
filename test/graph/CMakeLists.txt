# add_grail_executable([BINNAME <binary name>] SRC <src path> LIBS <libs>)

# add_grail_executable(SRC LuisaDemo.cc LIBS grail)
# add_grail_executable(SRC testAnim.cc LIBS grail)
add_grail_executable(SRC testLineGraph.cc LIBS grail)
add_grail_executable(SRC testBarChart.cc LIBS grail)
add_grail_executable(SRC testBoxChart.cc LIBS grail)
add_grail_executable(SRC testCandlestickChart.cc LIBS grail)
#TODO: refactor the classes, they are relying on similar concepts with different names in graphstyle
#TODO: refactor the transformation of (x,y) coords, use the axis to do the transform
#TODO: come up with unified graph style (blue style, gray, ... and be able to apply to all graphs)
 
#TODO: time series (test Julian Datte, come up with cool way to format dates)
#TODO: Debug GapMinder (data set has pointer bugs!) and unify with the hierarchy
#TODO: Unified way of animating all graphs to make a movie in time
#TODO: reparent GraphStyle from Member, right now Member is what sets Tab up, but what if we want multiple graphs on a page?


add_grail_executable(SRC testAxis.cc LIBS grail)
# add_grail_executable(SRC testCSR.cc LIBS grail)
# add_grail_executable(SRC testGapMinder.cc LIBS grail)
# add_grail_executable(SRC testGapMinderLoader.cc LIBS grail)
add_grail_executable(SRC testGapMinderWidget.cc LIBS grail)
# add_grail_executable(SRC testHeatMap.cc LIBS grail)
# add_grail_executable(SRC testTimeSeries.cc LIBS grail) # this appears to be old crap
# add_grail_executable(SRC testWidgets.cc LIBS grail)
