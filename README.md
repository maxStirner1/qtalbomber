# qtalbomber - yet another bomberman

http://jexhson.github.io/qtalbomber


![qtalbomber](https://raw.githubusercontent.com/jexhson/qtalbomber/master/doc/figures/qtalbomber.png)


## Game overview

To activate or deactivate a player in the game, simply click on it icon
before starting the game. When the player icon borders are blue, the
player is active, when becoming red, the player is dead and gray border
means that the player is not active. To distinguish players, refers to
the player icon color.

## Application file architecture

The *Maps* directory contains the available game fields (see map
creation for more informations). Only maps in this directory will be
selected at game start.

The *Sounds* directory includes sound files (*.wav* format) used by
QtalBomber. If you want to change one or more of these sounds, be sure
to assign the filename that the one you are replacing. Feel free to add
your favorite song as a soundtrack for your games!

## User controls

Action          |      Player 1      | Player 2  | Player 3|  Player 4
----------------|--------------------|-----------|---------|----------
Up              |     Top arrow      |    Z      |   U     |    G
Down            |    Bottom arrow    |    S      |   J     |    B
Left            |     Left arrow     |    Q      |   H     |    V
Right           |    Right arrow     |    D      |   K     |    N
Drop bomb       | Cmd (or right Alt) |    A      |   Y     |  Space

## Map creation

You can create your own mazes game, use your imagination and creativity
to make maps new and fun. To do this, create an XML file according to
the maps available in the *Maps* directory. See below for symbol meaning
and a map example.

**WARNING:** The card must not exceed 15x15 characters. The borders are
generated automatically, it is not necessary to draw them.

### Available map symbols

Symbol | Description
-------|--------------------------------------------
\#     | Unbreakable block
b      | Breakable block
\_     | Unused space
1-4    | Start position of a player. Must be unique

### Map XML example

~~~xml
<?xml version="1.0" encoding="utf-8"?>
<qtalmap xmlns="http://www.bombermanproject.com"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://www.bombermanproject.com qtalbomber.xsd">
  <map>
_______________
_#_#_#_#_#_#_#_
__1___bbb______
_#_#_#_#_#_#b#_
____#___#___b__
_#_#_#_#_#_#b#_
_2______b___b__
_#_#_#_#_#_#_#_
__#___b________
_#_#_#_#_#_#_#_
________b__#___
_#_#_#_#_#_#_#_
___b__#_____3__
_#_#_#_#_#_#_#_
______#__4_____
</map>
  <name>Default Map</name>
  <author>Billy Bob</author>
  <description>
    Rah-rah-ah-ah-ah-ah!
    Roma-roma-mamaa!
    Ga-ga-ooh-la-la!
  </description>
</qtalmap>
~~~

## License

qtalbomber is released under the MIT License. See [LICENSE][1] file for
details.

[1]: https://github.com/jexhson/qtalbomber/blob/master/LICENSE
