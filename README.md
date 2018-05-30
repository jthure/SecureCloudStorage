# Matser Thesis: Low-trust cloud data storage and sharing
This repo contains the implementations made during our master thesis. It consists of two main parts: A Gstreamer plugin and the crypto library Charm as a submodule conatining our own crypto scheme implementations

## Building and installing
The following instructions have been tested on Ubuntu 18.04. If you are on ubuntu or another debian based OS you can try the script quick-install-debian by just running it `./quick-install-debian`. It tries to install all dependencies, build and install everything. If there any problem you can do it manually.

### Building and installing Charm
The first step is to build Charm. In order to build Charm you need the dependencies in:
```bash
apt install build-essential python3 python3-setuptools python3-pip libgmp-dev libssl-dev flex bison
```
You also need PBC which is not available in the package repository. There is a copy PBC v0.5.14 included in deps/pbc. Extract the tar file, cd into the directory and run
```bash
./configure && make && sudo make install
```
After this you should be able to build Charm. Enter the root directory of Charm and run
```bash
./configure.sh && make && sudo make install
```
In some cases I've had problems with the permissions on the installed Charm files in the way that _others_ didn't get any permissions, specifically the files got _rwxrwx- - -_. This causes the application using it to crash. I fixed it by running the command `find /usr/local/lib/python3.6/dist-packages/Charm_Crypto-0.50-py3.6-linux-x86_64.egg/ -type f -exec chmod a+r {} \;` to add read permissions to everyone.

### Building and installing Gstreamer plugin
The second step is to build the gstreamer plugin. In oreder to build it you need gstreamer dependencies
```bash
apt install libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
```
Enter the directory _gst-pre-plugin_ and run the following
```bash
./autogen && make && sudo make install
```
The plugin is installed in _/usr/local/lib/gstreamer-1.0/_ In order for gstreamer to find when running an application you need to specify the env variable `GST_PLUGIN_PATH=/usr/local/lib/gstreamer-1.0/`

Next, enter the directory _gst-app_ and run
```bash
./autogen && make
```
This builds the programs _encryptor_, _re-encryptor_ and _decryptor_. Run the programs in the following way:
```bash
./encryptor video.enc
./re-encryptor video.enc video.reenc
./decryptor video.reenc
```