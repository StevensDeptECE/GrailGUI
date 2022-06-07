# add_grail_executable([BINNAME <binary name>] SRC <src path> LIBS <libs>)

# add_grail_executable(SRC testStockServer.cc LIBS grail)
add_grail_executable(SRC testXDLButton.cc LIBS grail)
add_grail_executable(SRC GenericXDLClient.cc LIBS grail)
add_grail_executable(SRC SimpleXDLServer.cc LIBS grail)

add_grail_executable(SRC testStockQuoteDeltaServer.cc QuoteTable.cc LIBS grail)
add_grail_executable(SRC testStockQuoteDeltaClient.cc QuoteTable.cc LIBS grail)

# add_grail_executable(SRC GraphicalXDLClient.cc LIBS grailserver)