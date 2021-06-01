# Battleship matchmaker
The concept of this algorithm is to establish a connection between two users where the communication will happen through message queuing. 
This algorithm will use 2 programs one is called the service and the other is called the user. These two will be in connection through a broker called benternet.
ZeroMQ will be used for message queing. The applicable programs for this application can be accessed through the zeeslagpp map .

## Working principle
The UI will send a message to the benternet broker. The service subscribes on the applicable data present on the benternet broker to collect the neccesary information from a single or multiple users.
When the service collected the needed data from multiple users. The service will choose an enemy for each user and send a message to each user wich user is their enemy. Each user can subscribe on his opponents username. When this is happened the connection between two users can be established.The coordinates to attack the other player can be entered.

![image](https://user-images.githubusercontent.com/73300420/120371389-06759c80-c316-11eb-8893-9411bba18865.png)
