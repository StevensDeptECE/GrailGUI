# add_grail_executable([BINNAME <binary name>] SRC <src path> LIBS <libs>)

# add_grail_executable(SRC testStockServer.cc LIBS grail)
add_grail_executable(SRC testXDLButton.cc LIBS grail)
add_grail_executable(SRC GenericXDLClient.cc LIBS grail)
add_grail_executable(SRC SimpleXDLServer.cc LIBS grail)

add_executable(testStockQuoteDeltaServer testStockQuoteDeltaServer.cc QuoteTable.cc)
target_link_libraries(testStockQuoteDeltaServer grail)
add_executable(testStockQuoteDeltaClient testStockQuoteDeltaClient.cc QuoteTable.cc)
target_link_libraries(testStockQuoteDeltaClient grail)

# add_grail_executable(SRC testStockQuoteDeltaServer.cc QuoteTable.cc LIBS grail)
# add_grail_executable(SRC QuoteTable.cc testStockQuoteDeltaClient.cc  LIBS grail)

# add_grail_executable(SRC GraphicalXDLClient.cc LIBS grailserver)