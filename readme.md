# DCDL

## Introduction

This is my implementation of a game present in a French TV show, "Des chiffres et des lettres". You can find more details [at this address](https://en.wikipedia.org/wiki/Des_chiffres_et_des_lettres#Numbers_round)

In short, the rules are as following: the player is given 6 number plates, going from 1 to 10, or being the numbers 25, 50, 75 and 100. Using the 4 basics arithmetic operations (addition, substraction, multiplication, division), the goal is to change the numbers until you reach the target number, which is between 100 and 999. One number plate can only be used once. Furthermore, you can only use positive integers. Therefore, an operation such as "2 - 3" is illegal as the result would be -1. Similarly, something like 6/4 would be forbidden, as the result is 1.5.

I am using the old method of drawing numbers: the 25, 50, 75 and 100 numbers are apparently half as likely to appear compared to the 1-10 ones. Furthermore, you can only have these big numbers once per game, and the 1-10 numbers twice per game. What was done in the game show before doing it on computers was having 24 plates, 2 for each 1-10 number and 1 for 25-50-75-100, which I copy when drawing the numbers (checking if a plate was already used).
The only restrictions regarding the number to find is that it's between 100 and 999 (included).

This is just a project I did for fun in order to improve my skills and to understand how to implement it, since I've always liked this game. There are arguably better and faster implementations out there. If you want to play and enter your solution, it will always be a little clunky because of the command line, but I can't think of an ergonomic solution without adding a graphical interface, which I don't want to do at the moment. [This implementation, done by other people, is available in English and provides a graphical interface](http://kitsune.tuxfamily.org/wiki/doku.php)

## Possible improvements

At the moment, the program only displays the first solution it finds, not the one which is the most "obvious" to a human eye. You may get some very weird results and find that the computer goes for extremely complicated calculations, but it just happens to be the first possible solution, depending on the order of the numbers. This is only an initial version of the game, eventually I would like to display all the different solutions, ordering them by the number of operations. I would also like to find a way to deal with redundant operations when the game happens to draw the same number twice. At the moment, the game should avoid most redundant operations, but is unable to detect a redundant operation if the same number happened to be drawn twice.

I may also have found a bug where one number plate is reused, but it happens rarely, and tracking this bug is pretty difficult because of this.

## How to use

The program was made to be easy to use and will guide you when selecting your game mode. You only have to run the program and type the answers expected by the game.