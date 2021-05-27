# Battleship matchmaker
The concept of this algorithm is to establish a connection between two users where the concept of communication will happen through message queuing. 
This algorithm will use 2 programs one is called the service and the other is called de user interface(UI), these two will be in connection through a broker called benternet.
ZeroMQ will be used for message queing. The applicable programs for this application can be accessed through the zeeslagpp map .

## Working principle
The UI will send a message to the benternet broker. The service subscribes on the applicable data present on the benternet broker to collect the neccesary information from a single or multiple users.
When the service collected the needed data from multiple user the UI shall receive a choice to connect randomly or by choice with another player. 
The connection between two players will be possible by subscription ons usernames.
When the connection is established. The coordinates to attack the other player can be entered.

