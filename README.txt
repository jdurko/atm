PLEASE make sure you are running on  cse06.cse.unt.edu since the server 
address has been defined in the code to run on this specific server

To compile our code,open a terminal and type:  g++ -o ClientCNP ClientCNP.cpp ,
then open another terminal and type g++ -pthread  -o ServerCNP ServerCNP.cpp

In fact both ClientCNP and ServerCNP are the the main file to compile 
,and also require CNP_Protocol.h , DataMap.h to be in the same file directory. 

To execute my code type:  ./ClientCNP for instance, and ./ServerCNP

*Case: Only when your environment paths C/C++ are not linked to your program directory

Please to compile, you may specify 
to the compiler the directory for input files 
by using the  Path To your Directory of code, I mean  by typing
g++ Path_To_Directory\Code Path_To_Directory\Code\ClientCNP.cpp
g++ -pthread Path_To_Directory\Code Path_To_Directory\Code\ServerCNP.cpp 

THANKS
from the team

