set(grail-csp
		IPV4Socket.cc
		Request.cc
		Socket.cc 
		SocketIO.cc
		XDLRequest.cc
)

list(TRANSFORM grail-csp PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-csp ${grail-csp} PARENT_SCOPE)