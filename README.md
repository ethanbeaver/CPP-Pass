CPP-Pass
========

_Disclaimer: this README was mostly written late at night. The story, all names, characters, and incidents portrayed in this production are almost entirely fictitious. No identification with actual persons (living or deceased), places, buildings, and products is intended or should be inferred. Or whatever._

Hello, and welcome to the world's first totally impermeable password manager. We go to great lengths to make sure your data is never seen or heard from again. Best of luck to the poor saps who have to try breaking into this safe.

Usage
-----

For quick help reference, use the `-h` flag.

    CPP-Pass -h

Start by generating a key file. A key is 32 bytes long and is totally random. That's 256 bits of randomness. Good luck breaking that one.

    CPP-Pass -g key.bin

Great. Now that we have a key, let's open up a safe.

    CPP-Pass -k key.bin -p password.safe

Now follow the onscreen prompts. You can rest assured that your passwords will remain safe, even in the event that a team of three college students tries to break in.

Installation
------------

No matter how awesome this code is, all code is useless without being turned into the useless garbage that is machine code. Clearly being filed in a circular filing cabinet is not what we want. Here's how to avoid that dark fate.

Go install the dependencies!

    Mac OS:
    brew install tclap openssl
    
    Ubuntu/Debian:
    apt-get install libtclap-dev libssl-dev

From the project root, run these commands on a Linux or Mac OS terminal.

    mkdir build && cd build  # make a place to store built files
    cmake ..                 # generate the makefiles
    make all                 # compile code

If `cmake` fails, try `cmake -D OPENSSL_ROOT_DIR=/path/to/openssl ..`.

**Important:** Windows is _not_ supported. It may work on Windows, but nobody bothered to install the dependencies and test everything. Good luck.

_Note: openssl can be found at `/usr/local/opt/openssl` on Mac OS._

Testing
-------

And here we are required to point out the necessity of testing one's code. Lucky for you, we make it super easy.

    ctest

That wasn't so bad, was it? What are you going to do with all your extra time?

Notes
-----

1. Sometimes it segfaults at strange times. Segfaults are not intended behavior, but they probably won't help an attacker much.
1. Still other times it throws a `runtime_error`. Such errors are considered "normal" behavior. You probably did something wrong and the system is telling you to stop goofing off.
1. There are no known vulnerabilities in the encryption techniques.
1. The only reasonable success metric for breaking this program is access to the passwords without a key file. An example use-case of this system is: a user keeps his key on a jump drive and the password safe on a networked drive in common storage.
1. `Storage` is where the magic happens.

How to Contribute
-----------------

Consider the following file structure

* `src` contains all _.cpp_ files
* `src/include` contains all _.h_ files (all the function descriptions live in here)
* `test` contains all unit tests
* `CMakeLists.txt` and `*.cmake` files are what `cmake` uses to generate the makefiles

And now for a celebratory picture:

![](http://static1.gamespot.com/uploads/original/324/3246309/2944516-4166708851-Comme.jpg)

Wow. So zen.

Be sure to commit often while working on specific objectives.

I use CLion from IntelliJ. It's free for students and super clean. But I won't judge too harshly if you decide another IDE is superior.

Under no circumstances should you ever f_îU¡«êI♥{E↑e⌂6-èU⌂.xhO>ìoR1UIhelpimtrappedinarandomessfactoryOIS↔¢KµIå[L¢Ö2óä? \*noise\*

Hey, at least this isn't as boring as most READMEs.

Now that we have all that cleared up, we hope you enjoyed your flight with RandomAndy Readme Tools, and have a nice day!
