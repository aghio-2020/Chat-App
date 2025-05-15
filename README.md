

<h1>Chat App</h1>

to build you need:

- boost 
- imgui 
- glfw

connect entering a ip address to the server, or run one locally and connect with localhost
  
<h3>Next improvements</h3>

- add fetch contents to cmake for packages
- move out of boost and use asio separately
- add more thread safety around callbacks and shared data
- make server bind to lan ip address by default and make cmd args to choose where to bind, including localhost as option
- make input box automatically set cursor to it after a message was sent
