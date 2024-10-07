# Understanding Config Files

There are two types of config files that you can adjust in the resources folder. 
The first of which are the level files and the second are the constant values.
This read me file is a guide to how those files are layed out and how you can edit
them to change the behavior of the game without having to recompile the code.

## Level Config

Under the levels folder you can find the level csv files. These files are setup so you
can dynamically change the level you are designing. Here are the properties of the files:

Level Size: 800x450 (Scaled up from this for other resolutions)

### Platforms
Dimensions: (110,17) <br>
File Layout: 0,x,y <br>
Example: 0,100,40 (Platform at 100,40)

### Player Spawn
Dimensions: (50,60) <br>
File Layout: 1,x,y <br>
Example: 1,100,40 (Player Spawn at 100,40)

### Enemy Spawn
Dimensions: (50,50) <br>
File Layout: 2,x,y <br>
Example: 2,100,40 (Enemy Spawn at 100,40)

## Values Config
This config is setup to change the values of the game. There are two columns in this file.
The first is what value you are changing and the second is the value. Use this table
to know what to put in the first column:

0 - int - Bullet Speed
1 - int - Timpy X Velocity
2 - int - Robor X Velocity
3 - decimal - Revolver Reload Speed
4 - int - Combo Required to Get Shield

Example:

Change bullet speed to 500: <br>
1,500
