**************ChamberCrawler3000*************

Project authors: Humble Masood & Albert Zhang

Date of production: April 4, 2016


**********************************************


-> About the game

A game of CC3K takes place on a board 79 columns wide and 25 rows high. The goal of the game is
to descend 5 floors. Along the way, a character controlled by the player slays enemies and picks
up treasures. A dungeon consists of these 5 floors with exactly the same layout but different
contents. While stair, potion, gold, player and enemy location will vary, each floor will consist
of the same chamber layout.


-> Playing the game

Objective:

Valid directions

1. no = north
2. ne = north-east
3. ea = east
4. se = south-east
5. so = south
6. sw = south-west
7. we = west
8. nw = north-west


Commands:

1. u [direction]: use the potion or pickup the treasure indicated by the direction parameter
2. a [direction]: attack the enermy in the specified direction. For the samurai and the knight,
	the enemy must be 1 square away(in any direction). For the wizard there must be enemy along the
   direction of attack up until boundary of the chamber.
3. k(night), w(izard), s(amurai): specifies the type of class player wants to be when staring the game
4. r: restart the game
5. q: allows the player to admit defeat and end the game
