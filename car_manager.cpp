//Liam Rotchford
//9/29/18

/*	this file handles all car movement and the cycling of set function calls that are needed for every movement iteration. it also handles the user control 
	and automated control. along with the LLL of node addresses our where the obstacles are in the GPS map ARR
*/

#include "GPS_Map.h"
using namespace std;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// ============================ CAR_MANAGER CLASS FUNCTIONS ========================== //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //


// ====================== CAR_MANAGER CONSTRUCTOR ============================ //
/*	sets the LLL head ptr to null, calls the stoplight constuctor, and sets automated to be true so the car drives itself until we hit the first
	car in front of us
*/
Car_manager::Car_manager(char Lightcolor) : c_head(NULL), Light(Lightcolor), automated(true) 
{
}

// ====================== CAR_MANAGER DECONSTRUCTOR ============================ //
/*	this will delete all allocated memeory of our node ptr data that holds the addresses of where the obstacles are in the Gps map 	*/
Car_manager::~Car_manager()
{
	if(c_head)
	{
		car_node * current = c_head;			//set current to head
		int val = list_destuctor(c_head, current);	//call recursive destructor to delete all the nodes in the list
		delete c_head;					//delete last head

		c_head = NULL;					//set both to null
		current = NULL;
	}
}

/* 	recursive deallocator for the c_head list, it will delete from the head until no more nodes exist 	THIS IS A LIST OF CAR NDOES HOLDING NODE ADDRESSES
*/
int Car_manager::list_destuctor(car_node *& c_head, car_node *& current)
{
	if(!c_head)				//check if head is null
		return 0;

	current = current->gonextcar();		//set current to next node
	c_head->setcar(NULL);			//set heads node data to null
	delete c_head;				//delete head
	c_head = current;			//set the head to where current it, the next node

	return list_destuctor(c_head, current);
}

// ====================== LLL BUILDER ============================ //
/*	this function will build the car node LLL it will first set the indexs for the start and end of the array	*/
int Car_manager::build_list()
{
	int first = 1;	
	start = table;
	node ** last_lane = table + 4;				//last lane 3, skip bus lane using the base case

	check_each_index(start, last_lane, first);		//send to check every index recursive	

	return 1;
}

/*	using ptr arithmetic increment through each index and each iteration sending it to teh set list fuincton that will take the head pointer and go through
	the list until we have gone through every index
*/
int Car_manager::check_each_index(node ** start, node ** last_lane, int & first)
{
	if(start == last_lane)			//check if we are in the last index
		return 0;

	node * current = *start;		//set current to be the head pointer

	set_list(current, c_head, first);	//send to set list with the head pointer of the array index and the head pointer of our LLL,

	return check_each_index(++start, last_lane, first);
}

/*	from the head pointer of the array indec go through the list and on any occurace that the data is a car or a parked car 
	save that node address in a new car_node, if this is the first go then set the first new car node to be the c_head for the list
*/
int Car_manager::set_list(node * current, car_node *& c_head, int & first)
{
	if(!current)						//are we at the end of the head pointer for array index
		return 0;

	if(current->getdata() == 'P' || current->getdata() == 'C')	//are we a car or parked car
	{
		if(first == 1)					//our first found obstacle become the c_head pointer for the LLL
		{
			c_head = new car_node;	
			c_head->setcar(current);
			c_head->setnextcar(NULL);
			++first;
		}else						//other wise add at the head of the list
		{
			car_node * temp = c_head;
			c_head = new car_node;
			c_head->setcar(current);		//set the data of the car location as the locations we are currently on	

			c_head->setnextcar(temp);
		}
	}
	return set_list(current->gonext(), c_head, first);
}

// ====================== MOVEMENT CYCLER ============================ //
/*	this function is the main function that has all the function calls that need to be called on every movement iteration or user choice
	it will switch what mover function we will be using depending on if the user chooses or not to take control of the car. it will also based on return values
	determine to end the program if we park the car or reach the end of the map. if we are still moving then it will change the light
	check the surroundings, print the surroundings and car data, and then print the map, it will reset the car speed from 0 if we stopped. and then
	to keep with the idea of a moving simulation for auto drive i have a execution delay
*/
int Car_manager::cycler()
{
	bool found = false;
	int depth = 1, index = 0, speed_saver = 0, users_pick = 0, val = 0;
	node * temp = NULL;					//set a base ptr that will be changed to wherever the car is for location fucntion in genreal .cpp
	node ** last_lane = table + 3;				//make sure we can stop at lane 3 and not go into the bus lane unessesaryily	
	node * mycar = My_Car::locate(start, last_lane, temp, found, depth, index);	//locate where our car is

	if(automated == true)					//if we are still auto moving then send to the auto mover function, if we return a 3
		val = mover(mycar, speed_saver,depth, index, users_pick);			//then the user has chosen to take over the car
	
	if(automated == false || val == 3)			//if user control send to user mover, if val 3 user has chosen to take over the car
	{
		users_pick = user_options();			//send to choose what option they want with using the car
		val = user_mover(mycar, speed_saver, depth, index, users_pick);	//send to user control mover 
	}
	if(val == 4)		//we just parked end prog
		return 0;
	else if(val == 0)	//we reached the end of the map return to main
		return 1;
	else			//contine moving through the map	
	{
		Light.LightChange();				//cycle light
		My_Car::check_safty(temp, index, depth);	//check surrounding
		My_Car::car_data_disp();			//display surroundings info
		GPS_Map::display_map();				//display updated map
	
		if(val == 2)		//we just hit a light, or user chose to stop the car, so we need to reset the speed back to 10 from 0
			My_Car::car_speed = speed_saver;	//reset speed
	}	
	
	if(automated == true)			//this delay will make it feel much more like a moving simulation
		this_thread::sleep_for(4s);	//delay execution of the displays at a time so its easier to read

	return cycler();	
}

/* 	user has chosen to take over control of the car, send them to the prompt and get the option they want to do with the car	*/
int Car_manager::user_options()
{
	int num = user_control();
	if(num == 1)		//stop
		return 1;

	else if(num == 2)	//go forward
		return 2;

	else if(num == 3)	//change lane left
		return 3;
	else
		return 4;	//change lane right 
}

// ====================== AUTOMATIC CAR MOVER ============================ //

/*	tis function checks the 4 cases of what could be happening with our car while we are auto driving, if there is a car in front of us, the light is in 
	front of us, we reached the end of the map, or none of those so we can move forward, it then call the respective mover_ fucntions
*/
int Car_manager::mover(node *& mycar, int & speed_saver, int depth, int index, int users_pick)
{
	int num = 0;	
	if(mycar->gonext()->getdata() == 'C')					//check if there is a car in front of us  
		num = mover_changelane(mycar, index, depth, users_pick);		//call movers change lane function

	else if(mycar->gonext()->getdata() == '=') 				//check if the light is in front of us
		num = mover_light(mycar, speed_saver); 					//call movers light function

	else if(mycar->gonext() == NULL)					//end of the map set our car X to a space return 0 to end prog`
	{
		mycar->setdata(' ');	
		return 0;	

	}else									//move ahead one node
		num = mover_forward(mycar);						//call mover forward function

	return num;
}

// ====================== USER CAR MOVER ============================ //
/*	this function takes into account the users choice of what to do with the car, we alwasy check if we are at the end of the map first, if user chose to stop 
	to stop the car then we report that the car is stopped and set car speed to 0. if they want to go forward we check if there is car in front of us
	and report, or a light and check with the mover light function to see if its green or red, otherwise move forward with mover forward function
	if tehy want to change lanes they specify left or right and then it goes into change lane function. if they choose to park the car the prog ends
*/
int Car_manager::user_mover(node *& mycar, int & speed_saver,int depth, int index, int users_pick)
{
	int num = 0;
	if(mycar->gonext() == NULL)					//end of the map
	{
		mycar->setdata(' ');						//set to blank and end prog	
		return 0;	
	}

	if(users_pick == 1)						//user control chose to stop the car
	{
		speed_saver = My_Car::car_speed;				//save speed	
		My_Car::car_speed = 0;						//set car speed to 0 to be reset later when we move again	
		user_report(1);							//state to user that we are stopping the car
		return 2;	

	}else if(users_pick == 2)					//user chose to move forward
	{
		if(mycar->gonext()->getdata() == 'C')			//car is in front of us we cant move forward
		{
			user_report(3);						//car in front call user report
			return 1;	
		}
		else if(mycar->gonext()->getdata() == '=')		//we are at the light 
			num = mover_light(mycar, speed_saver);			//check with light if green in mover light func	

		else
			num = mover_forward(mycar);			//if none of these issues are interupt then we can move forward and call mover_forward

	}else								//user choses to change lane
	{
		num = mover_changelane(mycar, index, depth, users_pick);	//call change lane function with the users pick to move either left or right
		if(num == 4)				//parked the car
		{	GPS_Map::display_map();
			return 4;			//end prog
		}
	}

	return num;
}

// ====================== CHANGE LANE ============================ //
/* 	move change lane function determines if we are auto or not, if there is a car in front ask user if they wanna take over, if yes change to auto to false
	and return 3 to move into user_move rfunction, otherwise go into change lane function and go into the side they wish to merge to, if they
	cannot merge to that lane report it, if they park report and end the program, else return num
*/
int Car_manager::mover_changelane(node *& mycar, int index, int depth, int users_pick)
{
	int num = 0;	
	if(automated == true)						//check if auto or not
	{
		num = user_takeover();					//there is a car in front of us so ask user if they would like to take over or not	

		if(num == 1)						//if yes we change automated and return 3 to move into user_mover function
		{
			automated = false;
			return 3;
		}
	}

	num = change_lane(mycar, index, depth, users_pick);		//otherwise we go into change lane function with the side they wish to change into
	
	if(num == 0)								//if they cannot not change into the lane we report
	{
		user_report(4);							// cannot change lanes there
		num = 1;							//reset to the continue return for cycler
	}
	else if(num == 2)						//if we return a 2 then we report parked and return a 2
	{
		user_report(5);	//parked
		return 4;
	}

	return num;
}

/*	take the my car and if the auto is false check only the proxcimity for one side that they chose, if false return 0 and report that they cannnot go there
	if auto is true check both sides, if both are false return zero those spots are taken, if we have a true then check against the LLL and find a match
	with that match set our car to be the next right after the match node
*/
int Car_manager::change_lane(node *& mycar, int index, int depth, int users_pick)
{
	int counter = 1;
	--index;	
	node * left = table[index];

	index += 2;
	node * right = table[index];

	node * holder = NULL;

	if(automated == false)
	{
		if(users_pick == 3)		//user choose left
		{
			My_Car::proxcimity_left = check_left_right( left, holder, depth-1, counter, true);	//check prox
			if(My_Car::proxcimity_left == false)
				return 0;
		}
		else if(users_pick == 4)	//user chose right	
		{
			My_Car::proxcimity_right = check_left_right( right, holder, depth-1, counter, true);	
			if(My_Car::proxcimity_right == false)
				return 0;
		}
	}else					//auto is off so check both sides
	{
		My_Car::proxcimity_left = check_left_right( left, holder, depth-1, counter, true);
		My_Car::proxcimity_right = check_left_right( right, holder, depth-1, counter, true);	
		
		if(My_Car::proxcimity_left == false && My_Car::proxcimity_right == false)	//if both return false we cant change to any lane
			return 0;
	}
	
	car_node * to_match = new car_node;	//make node car node
	to_match->setcar(holder);		//set it to be the address of the match node location before the free space we can change into
	
	//car_list_display(c_head);
	int num = check_list(c_head, mycar, to_match);		//check list to find match and change our car into that lane in front of our match

	delete to_match;
	to_match = NULL;

	if(num == 0)		//cant park there already taken
		return 0;

	else if(num == 2)	//changing lane into a parking space
		return 2;
	else			//changing lane in front of a car
		return 1;
}

// ====================== CHANGE LANE {CHECK THROUGH LLL TO MOVE} ============================ //
/* 	check through the LLL to see if we find a match for the node we found with the free space in front of it, if we find it then use it to set our car
	to be its next node in the list, if the data is a P then we are in the park lane, if our data is in a C then we are still in lanes 1, 2, or 3
*/
int Car_manager::check_list(car_node *& c_head, node *& mycar, car_node *& to_match)
{
	if(!c_head)
		return 0;

	if(c_head->getcardata() == to_match->getcardata())
	{
		if(to_match->getcardata()->gonext()->getdata() == ' ')	//check if the matched datas next node is free
		{
			mycar->setdata(' ');

			node * temp = c_head->getcardata();	//pull the obstacle location match to the temp
			temp->gonext()->setdata('X');	//set the change lane blank space to where our mycar will now be using the list 
							//by opening the saved location of the obstacle of type node of the list
			mycar = c_head->getcardata();	//set mycar location to where the car obstacle is
			mycar = mycar->gonext();	//set mycar location to the now 'X' location in front of the car obstacle successfully changing lanes

			if(to_match->getcardata()->getdata() == 'P')
				return 2;	
			else if(to_match->getcardata()->getdata() == 'C')
				return 1;
		}

		return 0;
	}

	return check_list(c_head->gonextcar(), mycar, to_match);
}

// ====================== AT STOP LIGHT ============================ //
/*	if the light is red or yellow stop the car, change the speed to zero and report to user, it will display the data and then reset the speed late
	otherwise move the car to the node after the light strip =
*/
int Car_manager::mover_light(node *& mycar, int &speed_saver)
{
	char stoplightcolor = Light.check_light();		//determine if we should display the light color if we are close enough

	if(stoplightcolor == 'R' || stoplightcolor == 'Y')	//if red or yellow stop the car
	{
		speed_saver = My_Car::car_speed;	
		My_Car::car_speed = 0;					
		user_report(2);
		return 2;	
	}else							//light is green and move to the node after the '='
	{
		user_report(6);	
		mycar->setdata(' ');
		mycar->gonext()->gonext()->setdata('X');
		mycar = mycar->gonext()->gonext();
	}
	return 1;
}

// ====================== MOVE FORWARD ============================ //
/*	move one node forward, if the next node is null return 0 after display the last map 	*/
int Car_manager::mover_forward(node *& mycar)
{
	mycar->setdata(' ');		// no cars ahead of the car move ahead one node
	mycar->gonext()->setdata('X');		 
	mycar = mycar->gonext();

	if(mycar->gonext() == NULL)		//next node is now null end of the map return 0 and end prog
	{
		GPS_Map::display_map();
		return 0;
	}
	return 1;
}

// ====================== LLL NODE PTR DISPLAY ============================ //
/*	simple c_head LLL display of the node addresses 	*/
int Car_manager::car_list_display(car_node *& c_head)
{
	if(!c_head)
		return 0;

	cout << " Data: " << c_head->getcardata() << endl;

	return car_list_display(c_head->gonextcar());
}
