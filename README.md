# MovieBackEnd_C-
## Simple booking backend engine written in c++

### Environment used for development
Microsoft Visual Studio 2019 has been used for developing this project. The compiler has been downgraded to C++14, so any C++14 compiler should be sufficient to build this project. The native environment is a VS solution that is making use of VS utilities for building and debugging. 

Boost has been used to add unit testing functionality. So as to be able to link the written code with boost libraries it's important to have boost installed. If the solution is to be opened with VS, please go to project properties->C/C++ and under **Additional Include Directories** type the folder where Boost test includes are. For me, this is C:\vcpkg\installed\x86-windows\include.

Before building the solution, there is a global define that needs to be selected. If **PRODUCTION** is defined, then tests are not executed and CLI (main function) is executed instead. If **PRODUCTION** is not defined then Boost tests are executed.

### CMake file
A CMake file has also been created for cross-platform builds. The validity of the build has been verified using VS2019's CMake tool. There are two things to note in the CMakelists.txt file    
- Enable/Disable PRODUCTION. By default, PRODUCTION is enabled and this should be the preferred option. Tests can still be run.    
- BOOST_INCLUDE_DIRS is the equivalent in CMake to "Additional Include Directories" under VS solution    

CMakelists.txt should provide all the functionality required to build, test and execute the project.

### Database
The database file is a simple csv file located in the same folder as the rest of the source files. The user can edit this file, but please be advise that changes in this file will cause unit test failures. Alternatively, the user can add a new database file and pass it as an argument to the program (the main function takes the database name as **dbname = argv[1];**).   
The database follows some simple rules. The first item in the line is the theather name. The following items in the line are the movies featured in that particular theather. The theather can be present in multiple lines featuring different movies. Items must be separated by a comma. For example

*Theather London,X-Men,Terminator    
Theather Fleet,Pulp Fiction,Casablanca    
Theather London,Pulp Fiction*   

### CLI
The CLI does not provide parallel access to users. Still basic functionality can be used. Note that the desired database needs to be passed to the program as an argument as described before.

### BOOST TESTS
A total of 9 tests have been created to cover most of the code base. Expected results are based on the contents of testdatabase.csv. The last test tries to simulate and test concurrent acccess to the engine's data structures by spawning a few threads and try to make them book pseudo-simultaneously. The test proves that no thread booked all tickets for a given movie.
