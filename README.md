

<h1>Chat App</h1>

A very simple chat application with it's server application. 
If you ever thought your chatting app of your choice was too secure and well functioning, then you've come to the right place!

![image](https://github.com/user-attachments/assets/a8a561ef-78a1-434a-8ba4-5c7991ed1f16)

To use, boot up a server with the ip and port you want to bind to (can be your private ip if you'd want to LAN) and then run up as many clients as you want, connecting to that port and address from the server with an username

<h3>Setup</h3>

to build you need:

- boost 
- imgui 
- glfw

just including the repos in the packages directory should do. Boost should work if it is in the system default dir (e.g. C:/boost for windows) so if thats the case, feel free to avoid having to copy it on the packages dir
  
<h3>Next steps</h3>

- add fetch contents to cmake for packages
- move out of boost and use asio separately
- add more thread safety around callbacks and shared data
- make server bind to lan ip address by default and make cmd args to choose where to bind, including localhost as option
- make input box automatically set cursor to it after a message was sent
- organize events and messages declarations/definitions into their own files
- decouple UI implementation as an MVC
