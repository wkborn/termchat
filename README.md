An ncurses terminal chat program.

# Installation

Dependencies:

Debian/Ubuntu
`sudo apt install libenet-dev libncurses5-dev build-essential`

Arch
`pacman -S enet make gcc enet ncurses`

~~~
make
sudo make install
~~~

# Usage

Client:

`$ termchat`

Server:

`$ termchat -s`
