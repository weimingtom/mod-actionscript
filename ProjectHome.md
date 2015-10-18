
---

## Serverside ActionScript solution for Apache Http Server ##

---




This software enables ActionScript 3 active web pages.

Currently, there are server-side solutions for creating web-apps using most programming languages, from C/C++ to JavaScript or C#. This solution allows building web applications using the very flexible and yet powerful language ActionScript. It is based on Tamarin virtual machine, which is available as open source.

This solution currently comes as a FastCGI implementation and has been tested on Apache HTTP Server.

Tools to create and run actionscript bytecode (.abc files) are provided.



Current project status: in development. An early stage preview is available.
(currently developed/tested on Linux, Ubuntu 8.10)

Check this [README](http://code.google.com/p/mod-actionscript/source/browse/trunk/tamarin-embed/README) for details on how to compile it.

Database support for MySQL (No connection pool).


---


_any help will be very appreciated!_
write me at victor dot dramba atgmaildotcom


---


## I don't want to compile it, just give me the program! ##

Current build is not yet ready for production. If you're curious enough though, check the [Downloads](http://code.google.com/p/mod-actionscript/downloads/list) section and try the thing on your computer.

Oups, you're on windows! Not so bad, you can run a virtual machine. We prepared a remastered Debian installer together with a virtual machine emulator (QEmu) that will run under XP or Vista _without the need to install anything_ on your OS. It will produce a Debian with ModAS installed. You have 2 bat files and a README in the zip.

Check server log `tail -f /var/log/apache2/error.log` for ActionScript and other errors. trace() output also goes in the error log.

Let me know if it worked ;)


---


## Roadmap ##

GET/POST/COOKIE management (done)

AMF

Session management

Templating (done TAL minimal implementation, no caching)

File Upload management (done, but API might change)

Image processing (GD?)

Email (unix mail, SMTP)

Curl

Sockets

_You can help speed up development!
If you know C++, pick an item in the list and come up with a solution. I'll be glad to add commiters to the project (for relevant contributions)_

---


**references**:

  * Build Tamarin ActionScript virtual machine
> https://developer.mozilla.org/En/Tamarin/Tamarin_Build_Documentation
  * mod\_fcgid
> http://fastcgi.coremail.cn/