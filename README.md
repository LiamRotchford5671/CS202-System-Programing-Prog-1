# CS202-ProgramingSystems-Prog-1
Program 1 assigned under Karla Fant at Portland State University

Description: 

This program simulates the movement of a automated self driving car the base call is the GPS_Map class that handles the ARR that contains the road map that we will be travelling along with our car, the map is populated with chars containing either ' ' (spaces) , C cars, B bus, P parked car, = stoplight strip. This class holds the object of the node class "has a". The My car class derived from the base class handles all data and functions pertaining to the surroundsing of the car, the car speed, and the brake time. The last lowest derived class connected to My_car is Car_manager class that handles all car movement be it automated or user controled. it has a the Stoplight object that controls the cycling of the light, it also contains the car_node class head that contains the LLL of node pointers to where all the obstacles on the map is. Further information on the rules of my program can be found in this file under user_rules(). In the event of a car being in front of our own car the user will be given the option to take over the car control or continue with automated control. 

Data Structures: Array of linked lists & Linked list

                   Map ---- Node
                    |
                    |
                  My_car
                    |
                    |
    Stoplight----Car_manager ---- Car Node
