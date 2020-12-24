# ~/.profile: executed by the command interpreter for login shells.
# This file is not read by bash(1), if ~/.bash_profile or ~/.bash_login
# exists.
# see /usr/share/doc/bash/examples/startup-files for examples.
# the files are located in the bash-doc package.

# the default umask is set in /etc/profile; for setting the umask
# for ssh logins, install and configure the libpam-umask package.
#umask 022

# if running bash
if [ -n "$BASH_VERSION" ]; then
    # include .bashrc if it exists
    if [ -f "$HOME/.bashrc" ]; then
	. "$HOME/.bashrc"
    fi
fi

# set PATH so it includes user's private bin if it exists
if [ -d "$HOME/bin" ] ; then
    PATH="$HOME/bin:$PATH"
fi

# set PATH so it includes user's private bin if it exists
if [ -d "$HOME/.local/bin" ] ; then
    PATH="$HOME/.local/bin:$PATH"
fi

export DISPLAY=:0.0
export LIBGL_ALWAYS_INDIRECT=1
export PKG_CONFIG_PATH=/usr/lib/pkgconfig:/usr/lib/x86_64-linux-gnu/pkgconfig:/usr/local/lib/pkgconfig
#export LD_LIBRARY_PATH=/usr/lib:/usr/lib/x86_64-linux-gnu:/usr/local/lib:/home/dongyin/CraneFramework/build/lib/crane:/home/dongyin/CraneFramework/build/lib/plugins
export LD_LIBRARY_PATH=/usr/lib:/usr/lib/x86_64-linux-gnu:/usr/local/lib:/mnt/d/CraneFramework/build/lib/crane:/mnt/d/CraneFramework/build/lib/plugins
#export C_INCLUDE_PATH=/usr/include:/usr/include/glib-2.0:/usr/include/c++/7:/usr/include/x86_64-linux-gnu/bits:/usr/lib/x86_64-linux-gnu/glib-2.0/include:/usr/local/include:/usr/local/include/gstreamer-1.0
export C_INCLUDE_PATH=/usr/include:/usr/include/glib-2.0:/usr/lib/x86_64-linux-gnu/glib-2.0/include:/usr/local/include:/usr/local/include/gstreamer-1.0:/usr/include/python3.8
export CPLUS_INCLUDE_PATH=$C_INCLUDE_PATH

export MEP_APIGW_ENDPOINT=127.0.0.1:8443
export MEP_APIGW_NOTIFY_URL=127.0.0.1:9090
export APP_INSTANCE_ID=682a2fb92b224029b0d02205231b8450
export CRANE_SYS_PLUGIN_PATH=/mnt/d/CraneFramework/build/lib/crane
export CRANE_APP_PLUGIN_PATH=/mnt/d/CraneFramework/build/lib/plugins
export CRANE_INIT_MODE=LIB
