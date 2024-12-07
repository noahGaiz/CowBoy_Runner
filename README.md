# CowBoy_Runner

This was a group project me and my team members made in March of 2023.We had around 5 weeks to complete it. It was completed in C and this model(stm32f031x6) for the microprocessor.
The link to watch the video of our project is( https://youtu.be/vCNG7I6nP4Q)

The link to the video exaplains everything a lot better

We were given the assignemtn to create a game on this microprocessor that had a couple reqiurements like serial connection(connection to the keyboard), game noise and mainly replayablility. Our idea sparked off the game subway surfer but instead have a 3d top view to the game , we made a 2d top view as its difficult to make 3d characters on the microprossesor. The idea was the cowboy would be running to the right of the screen trying to dodge oncoming cacti. In the game there were 3 lanes where the cacti can come from and some cacti come up closer than others. The object of the game was to survive as long as possible with an active timer on the screen to show your score.

In the game we created our own sound for when the user loses in the game

The hardest part about coding the game was working with the ports of the microprocessor and connecting our keyboard to it. I remember trying to connect and turn on LEDs to show the users lives so if the user lost a life, one of the LEDs would turn off. Another issue was trying to write the statement that reads the user clicking buttons on the bread board or the computer to move the character for all the possible moves for the character/controls for the game.
