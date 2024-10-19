# Resources Directory
Welcome to the resources directory! Most likley you know why you are here already or you are just curious. 
For the most part this directory is used to store the important sounds and graphics in the game. However there are
some ways that you can change how the game runs to give you more of a unique experience! Keep reading if you are
interested in these options.

## Changing Sounds/Textures
If you would like to change how the game looks and sounds you can replace any of the textures and sounds in those
two folders. Just make sure that what you are replacing it with has the same name, file type, and aspect ratio.

## Creating Levels
Are you bored with the default game levels? Then why not create one! Here are the rules that must be followed
when creating a level.

### File Type/Name
When creating a level make sure that you are using a csv file and that the name is the same as one of the current
levels. Your level will replace the current level in the game, so just select that button from the menu.

### Level Grid
The levels in Over_Gravity are made using a grid/tile design. Each tile is 80px x 80px for 800x480 resolution.
The levels are 12 tiles across and as many tiles deep as you want. 800/80 = 10 so why 12? Before the entities wrap
around the screen they need a second to teleport. This means that if you have a platform on the edge of the screen
make sure there is one off-screen. The first and last column is not shown.

### Tile Types
Each spot in the csv can be filled in with one of the following options:

1. -1 : Empty Space
2.  0 : Empty Platform
3.  1 : Player spawn (Exactly 1 Per Level)
4.  2 : Non-tracking enemy Spawn (1 required)
5.  3 : All enemy spawn (1 required)