
# Build instructions

----
### Building GLFW:

cd into 'include/glfw' if include is empty then clone the repo again with '--recursive' to include all the submodules when cloning.

at 'include/glfw' create a directory named 'build' and cd into it, run 'cmake ..' when done, run 'make'.

if compilation was successful you should see the libraries inside of the 'src' folder.

### Building GLEW:

cd into 'include/glew' and run 'make', if you get a "no rule to make target" error then run 'make extensions' instead, then 'make' when the process is finished.

### Building the app:

After building both GLEW and GLFW, you should then go to the projects directory, create a folder named 'build' and then run 'make' which builds the app and creates the executable in the build folder.



