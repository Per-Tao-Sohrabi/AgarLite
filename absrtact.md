# Agar I/O
#### By Per Sohrabi and Yunfei Liu, 2025-10-13.

## Objective and requirements:
The purpose of this project is to build a top down Agar-Io like game where player characters traverse a 2-dimensional game map where the objective is to consume items such as food and other characters in order to grow in size. 
Local Multiplayer: For simplicity, the whole game will be run locally on the DE10-Lite board, in which the two player characters can be controlled simultaneously from the board using two sets of switch controls. 
Score mechanism: A score mechanism will be implemented in order to determine the lead. The score is aimed to also influence player stats such as movement speed.
Character interaction will lead to a transfer of score, with score going from the non-leading character to the leading character. 
2D-game board
VGA output: The game should be displayable using VGA. 
Food items: Food items should spawn at determined rates in random locations on the game map.

## Optional features:
The score will lead to a size increase of the player hitbox. 
Difficulty level: Different difficulty levels could be characterized by how often foods spawn. 
Power ups: Power up items could momentarily give an upper edge to a character. Examples could be higher speed, protection from score transferring, and remote score transfers. 
Solution:
This project is aimed to be developed for the DE10-Lite board. The game will be developed in C where basic I/O methods such as polling for interrupts will be used to read player input and thus update the game state accordingly. The game state and associated graphical representation will be displayed using the VGA port to any available VGA compatible screen. This could be an old computer monitor or a projector such as the ones found in the computer labs at KTH. Interrupts from the available button control will be used in order to pause/reset the game. 

## Verification:
We intend to verify the functionality of the game by rigorous testing throughout the development process, ensuring that every requirement works as intended in the variety of game states that could be produced throughout and across a run. In order to do this, while developing independent functions, great care will be put towards for example ensuring that trigger handlers do not interrupt unintended aspects of the game. 

## Contributions: 
The game is developed by the authors of this text, and coordinated using a shared Git-hub repository. The different assets of the game could be developed independently when possible, while co-working will be implemented as much as possible to ensure synchronized ideation and implementations. 

## Reflections:
Upon finalising, we will reflect on the project by focusing on major technical points of interest such as difficulties or interesting solutions, ease or hurdles with regards to coordination, and additional improvements and features that could be added to the game. 
