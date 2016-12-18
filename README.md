# FileCopyServer
By: Luke Cotton

## What is FileCopyServer?
FileCopyServer is the server end of two programs I like to call FileCopy. FileCopy will allow you to send files back and forth between two systems on a network. Another program, called FileCopyClient, is used to send a file to a computer that is hosting this server.

## How to compile.
To compile, use the following steps
### Unix based systems with libpthread.
Compile using make. Go into the FileCopyServer subdirectory, and then compile with
```
make
```
### OS X
You can use the steps above for Unix based systems, or you can open the Xcode project file in Xcode at the root of the repository.
### Windows
Open the Visual Studio solution file in the FileCopyServer directory, and from there run and build the project.

## How to use.
Once compiled, you can run it with the command:
```
./FileCopyServer <path to dir>
```
Or run it with Visual Studio or Xcode. Make sure to specify valid paths to a folder on a system as a command line argument.
