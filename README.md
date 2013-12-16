BrownMaze2012
=============

A collection of code from the 2012 Brown Maze Competition.

The collection is probably incomplete, but should 
have most of the important files we used last year.


About the Sections
==================

libraries-CompApr13

The state of the Arduino libraries used during the 
competition. Of special interest are Brain, which runs
A*, and DriveSystem, which deals with telling the robot
where to go in absolute coordinates. Some of these 
libraries have not been extensively tested.

programs:

Main programs for actually running the maze solver. 
newDriveSys and newDriveSys2 are essentially what were run
during competition while finalExample is an example of what
a finalized program should look like.

tests:

Basic test/calibration programs.  We used straightLine after
the competition to evaluate some of the characteristics of
the robot's drive system.