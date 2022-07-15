#XDL automatic updating

XDL is an efficient, binary data transport layer.
It is like google protobuf but supports metadata automatically, and supports the notion of
connected

The following requests should be supported. Requests are 4 byte integers

1. main page, the equivalent of index.html
1. request all metadata (send the metadata for the symbol table of available information
1. request object(s), send a list of names and they will be sent to the client
1. simplified list update (add only). Need 
1. request update to object(s), send a list of names and information about the state and get updates
