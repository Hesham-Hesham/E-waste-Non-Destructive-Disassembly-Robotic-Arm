#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/himaet23/EwasteNonDestructiveDisassembly/src/ft_sensor_driver"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/himaet23/EwasteNonDestructiveDisassembly/install/lib/python3/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/himaet23/EwasteNonDestructiveDisassembly/install/lib/python3/dist-packages:/home/himaet23/EwasteNonDestructiveDisassembly/build/ft_sensor_driver/lib/python3/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/himaet23/EwasteNonDestructiveDisassembly/build/ft_sensor_driver" \
    "/usr/bin/python3" \
    "/home/himaet23/EwasteNonDestructiveDisassembly/src/ft_sensor_driver/setup.py" \
    egg_info --egg-base /home/himaet23/EwasteNonDestructiveDisassembly/build/ft_sensor_driver \
    build --build-base "/home/himaet23/EwasteNonDestructiveDisassembly/build/ft_sensor_driver" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/himaet23/EwasteNonDestructiveDisassembly/install" --install-scripts="/home/himaet23/EwasteNonDestructiveDisassembly/install/bin"
