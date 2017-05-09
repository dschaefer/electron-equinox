# electron-equinox
An example on how to load up Equinox into Electron and call an OSGi service

I'm using the CMake plug-in for CDT to build the Node C++ Addon.
You need CMake and a toolchain installed on your machine.
You could probably do this from the command line by creating a build directory,
calling CMake and running the 'install' target to copy the .node file into the lib directory.

Also before running, do a import of org.eclipse.osgi from your target so the Launcher can find it.

I'm sure I'm missing things but I just wanted to get this up on github so people can look at the code
and see if it sparks any ideas.
