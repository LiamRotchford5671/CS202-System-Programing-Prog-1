//Liam Rotchford
//9/29/18
//PROGRAM 1 CS 202

/* This program simulates the movement of a automated self driving car
 * the base call is the GPS_Map class that handles the ARR that contains the road map that we will be travelling along with our car, the map is populated with 
 * chars containing either ' ' (spaces) , C cars, B bus, P parked car, = stoplight strip. This class holds the object of the node class "has a". The My car class 
 * derived from the base class handles all data and functions pertaining to the surroundsing of the car, the car speed, and the brake time. The last lowest derived 
 * class connected to My_car is Car_manager class that handles all car movement be it automated or user controled. it has a the Stoplight object that controls the 
 * cycling of the light, it also contains the car_node class head that contains the LLL of node pointers to where all the obstacles on the map is. Further 
 * information on the rules of my program can be found in this file under user_rules(). In the event of a car being in front of our own car the user will be given
 * the option to take over the car control or continue with automated control. 
 */

#include "GPS_Map.h"
using namespace std;

int main()
{
	Car_manager object;	 
	
	object.GPS_Map::builder(); 	//builds the ARR road map populated with obstacles
	object.build_list();		//builds the LLL that contains the address locations through node pointers of where the obstacles are in the ARR
	user_menu(object);		//begin program

	return 0;
}

// ====================== USER MENU ============================ //
/*	This function asks the user to to input what car speed we would like to start with, it then confirms the data being valid (10 or 15) then it displays
	the rules of my program, prints the map and key, then it pauses the execution to allow time to read the rules. From there it begins the cycler function
	which initiates the automated movement of the car through the cycler that cycles a set of specified functions until we reach the end of the map
	if the user parks the car it will end the program.
*/
void user_menu(Car_manager & object)
{
	bool valid = false;
	int val = 0;
	char choice = 0;

	cout << "\n\nO Welcome to the User menu: " << "\n======================================================================================" << endl;
	do
	{
		//get user input for car speed	
		cout << "\n\tO To begin please enter your cars speed, due to the school year starting with the influx of students of the area " << endl; 
		cout << "\n\tPSU and the City of Portland have decided that a SPEED LIMIT of 10 to 15 {mph} to ensure safty of pedestrian: " << endl;
		cout << "\n\t++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;	
		cout << "\n\n\tInput Car Speed: ";
		cin >> val;
		cin.ignore(100, '\n');	

		if(val == 10 || val == 15)
			valid = true;
		else
			cout << "\n\n\to I'm sorry, but as your self-driving automotive I must insist that our speed stay within the law" << endl;
	}while(!valid);

	object.My_Car::input_speed(val);		//input car speed into class data member
	user_rules(object);				//display user rules	
	
	this_thread::sleep_for(10s);			//delay execution of the displays at a time so its easier to read  
	val = object.cycler();				//begin program and cycle car through the map

	if(val == 0)					//parked the car end the program
		cout << "\n\to You just parked the car! " << endl;
	else						//reached the end of the map, end the program
		cout << "\n\to You've reached the end of the map." << endl;

	cout << "\n\n\to Thank you for using the automated car simulation! \n\n\n" << endl;
}

// ====================== USER RULES DISPLAY ============================ //
/*	This program just displays the start up rules of my program simulation so the users can understand whats happening, it also prints the begining start up
	map and the map key
*/
void user_rules(Car_manager & object)
{
	
	cout << "\n\n\nO Before we automatically begin please read over this information about the simulation: " << endl;
	cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" <<endl;
	cout << "\n\t1) The simulated car will automatically move through the obstacles for you as a frame by frame moving display" << endl;
	cout << "\n\t2) As you can see from our GPS map, the left most road strip is reserved for parking, followed by 3 driving lanes, and finally the " << endl;
	cout << "\n\t  rightmost lane that is reserved only for busses." << endl;
	cout << "\n\t3) While driving at such a low speed it will take only one car length to come to a complete stop, and while there are cars to our " << endl;
	cout << "\n\t  direct left and right changing lanes will be restricted." << endl;
	cout << "\n\t4) In the event that car gets in front of the car you will be given the option to take over control of the vehicle if desired." << endl; 
	cout << "\n\t5) If you choose to park, the simulation will end. " << endl;

	cout << "\n\no GPS Map Symbol Key:" << "\n=============================================" << endl;
	cout << "\n\t(X) ~ My Car\t(C) ~ Other Cars\t(P) ~ Parked Cars\t(B) ~ Bus\t(=) ~ Stoplight Strip\t( ) ~ Empty Car Length Space" << endl;
		object.GPS_Map::display_map();
	cout << "\nO The simulation will begin shortly {using delay execution function to allow time to read}" << endl;
	cout << "\n=====================================================================================" << endl;
}	

// ====================== USER CONTROL (y/n) ============================ //
/*	This will execute when the user reaches a car in front of our own car and the user is now prompted in here if they would like to take over control of the 
	car, or allow the program to continue to automatically move on its own.
*/
int user_takeover()
{
	int choice = 0;
	bool valid = false;

	do
	{
		//user prompt to take over car or not	
		cout << "\n\to There is currently a car directly in front of us, what course of action you like?" << endl;
		cout << "\n\t1) Take control " << "\n\t2) Allow car to handle course of action" << endl;	
		cout << "\n\to Selection: ";
		cin >> choice;
		cin.ignore(100, '\n');

		if(choice == 1 || choice == 2)
			valid = true;
		else
			cout << "\n\to Please select from the given options." << endl;

	}while(!valid);
	
	return choice;
}

// ====================== USER CONTROL OPTIONS ============================ //
/*	this function will execute when the user choses to take over the car, it will prompt the user about what options they have to move the car
*/
int user_control()
{
	int choice = 0;	
	bool valid = false;

	do
	{
		//user prompt if they chose to take over, options prompt	
		cout << "\n=====================================================================================" << endl;
		cout << "\n\to What course of action will you take?" << endl;
		cout << "\n\t1) Stop " << "\n\t2) go forward {right on map} " << "\n\t3) change lane left {up on map} " << "\n\t4) change lane right {down on map}" << endl;
		cout << "\n\to Selection: ";
		cin >> choice;
		cin.ignore(100, '\n');

		if(choice == 1 || choice == 2 || choice == 3 || choice == 4)
			valid = true;
		else
			cout << "\n\to Please select from the given options." << endl;

	}while(!valid);

	cout << "\n=====================================================================================" << endl;
	return choice;
}

// ====================== USER CONTROL REPORTING ============================ //
/*	this function executes when the user needs a cout out report of why something is happening, something is currently happening, or a reason why the user
	control option they wish to take is not avaible. this can be from stopping the car, the light is red, green, yellow, cannot change lanes, there is 
	something in front of us we cant move forward, or we've parked the car
*/
void user_report(int reason)
{
	cout << "\nO REPORT: " << "\n+++++++++++" << endl;
	if(reason == 1)  //stopping the car
		cout << "\n\to Initating stopping sequence now" << endl;
	else if(reason == 2)	//light is red
		cout << "\n\to The stoplight is currently red, we cannot move forward" << endl;
	else if(reason == 3)	//car in front of us we cannot move forward
		cout << "\n\to There is currently a car in front of us, to avoid a collision we cannot move forward" << endl;
	else if(reason == 4)	//cannot change over to a lane
		cout << "\n\to The lane space you are attempting to shift too is currently taken" << endl;
	else if(reason == 5)	//car has been parked
		cout << "\n\to Car has been parked, ending simulation" << endl;
	else if(reason == 6)	//green light
		cout << "\n\to Light is currently green, moving forward" << endl;
	else if(reason == 7)	//yellow light
		cout << "\n\to Light is currently Yellow, initiating stopping sequence" << endl;
}




