cd ..\..\Code
CALL CreateBuildResults.cmd
cd ..\Build\Code

CALL Build_CLR2_RTM.cmd
CALL Build_CLR4_RTM.cmd