# Design Goals of XDL transmission

The XDL Language should be able to define data for binary transport
The data should be automatically viewable on the client using standard display methods
such as turning an object into a table of name-value pairs, 
or turning a list of objects into a table where each object is a row

The names of objects should be translatable to other languages using a standard dictionary
definition so that applications can display data in the user's local language.

client normally initiates a conversation which the server responds to, however once a connection
is initiated it should be possible for the server to send and the client should respond (push).
Thus clients should be multi-threaded, and a thread should always be listening for incoming messages.
Instead of using terms client and server, more generically we will use sender and receiver.

Objects on the sender should be capable of sending a message to the receiver requesting an update.
An update would synchronize the state between receiver and sender.

Example: A list with n elements requests update. This requires knowing when data was last received. Timestamp?
Send a request for update with: the id of the list, and the timestamp when it was last updated.

Use case: client has list "AAPL" with 7800 quotes. Send request for update ("AAPL", 7800).  The server responds with any new elements

Use case: client has a list of emails. Server deletes some emails, synchronize

Use case: client has a list, insert and delete various records

Key objects to be updated: List, HashMap