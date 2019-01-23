//Liam Rotchford
//9/29/18

/* this file manages the setters and getters of the ARR and LLL structures, the base class function of building, displaying, constucting, deleting the ARR. 
	the my cars surrounds data and functions to get that data and car data, and the stop light functions to cycle the color and initiated sequence
*/

#include "GPS_Map.h"
using namespace std;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// ================== NODE CLASS SETTERS AND GETTERS ====================== //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
/*	access and set data or next pointer 					*/
void node::setdata(char value)
{
	data = value;
}

char node::getdata()
{
	return data;
}

node*& node::gonext()
{
	return next;
}

void node::setnext(node * temp)
{
	next = temp;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// ================== CAR_NODE CLASS SETTERS AND GETTERS ====================== //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
/*	access and set data or next pointer 					*/
void car_node::setcar(node * temp)
{
	car_location = temp;
}

node*& car_node::getcardata()
{
	return car_location;
}

car_node*& car_node::gonextcar()
{
	return nextcar;
}

void car_node::setnextcar(car_node * temp)
{
	nextcar = temp;
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// ================== MAP CLASS FUNCTIONS ====================== //
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

// ====================== MAP CONSTRUCTOR ============================ //
/*	build the ARR list, build the array of node head pointers, then set each head pointer index to null, using intializing list for table size. the array is 5 */
GPS_Map::GPS_Map() : table_size(5)
{
	table = new node * [table_size];		//create array of head pointers of size 5

	node ** start = table;				//set index[0]
	node ** end = (table + table_size);		//set index [4]

	set_to_NULL(*start);				//set all to null
}

// ====================== SET EVERY HEAD PTR IN ARRAY TO NULL ============================ //
/*	use pointer arith to go through each index in the newly created array and set each head pointer to NULL */
int GPS_Map::set_to_NULL(node *& start)
{
	if(start == table[4])				//loop through every index
	{
		start = NULL;
		return 1;
	}

	start = NULL;

	return set_to_NULL(++start);			//ptr interate through array indexes
}

// ====================== MAP DECONSTRUCTOR ============================ //
/*	set the start and ending index of the array and loop it through sending it do deconstuct that deletes the LLL on each head pointer , once it has moved 
	through each index and has deleted the lists of each then delete the table array
*/
GPS_Map::~GPS_Map()
{
	start = table;
	end = (table + table_size);
	int num = 1;

	while(start < end)			//move through the array indexes
	{
		deconstruct(*start, num);	//send to recursive deconstructor
		num = 0;			//reset num
		++start;			//move to next index
	}

	delete [] table;			//delete table array
	table = NULL;
};

// ====================== RECURSIVE DECONSTUCTOR FOR ARRAY OF LINKED LISTS ====================== //

//	this function takes in the head from the current head of the current index of the array and then recursivly moves to the end of the list. 
//	it increments the num value to determine the number of nodes in the list for that index, it then deletes the list from the tail of the list until 
//	num is 1 to state that we are back on the head and can set the index to NULL.

int GPS_Map::deconstruct(node *& head, int & num)
{
	if(!head)				//base case
		return 0;

	if(!deconstruct(head->gonext(), ++num))	//loop to end of the list counting the number of nodes in this index
	{
		node * temp = head->gonext();	//delete the last tail node
		delete head;
		--num;				//decrement the amount of nodes left

		if(num == 1)			//if num is 1 we are on the last node which is the head pointer on the index
			head = NULL;
		else
			head = temp;		//if we arent on the head point reset head 
	}

	return 0;
}

// ==================== FILE LOADER ========================= //
/*	the ARR array and indexes have been intiated to NULL, so now we can populate and build the lists on each head pointer using my car.txt file
*/
int GPS_Map::builder()
{
	return populate();
}

int GPS_Map::populate()
{
	ifstream f_in;
	char file_end, value;
	int index = 0, count = 10;

	f_in.open("Car.txt");		//file
	f_in >> noskipws;		//allows white spaces to allowed when reading from file to avoid skipping it


	if(f_in)			//check that file was opened correctly
	{
		file_end = f_in.peek();				

		while((!f_in.eof()) && (file_end != -1))	//loop till we have gotten all of the characters from the file
		{
			node * temp = new node;			//create a new node
			f_in >> value;				//set the char data to the new node
			temp->setdata(value);
			f_in.ignore(100, '#');
			temp->setnext(NULL);		

			if(table[index] == NULL)		//if the head pointer is null then make this first new node the head pointer
			{
				table[index] = temp;
				temp = NULL;
				table[index]->setnext(NULL);
			}else					//else add the new node to the head of the list
			{
				node * hold = table[index];
				table[index] = temp;
				temp->setnext(hold);
			}

			--count;				//decrement the counter so we get 10 nodes fro each list	

			if(count == 0)				//once we have 10 nodes for one list move to the next index
			{
				if(index != 4)			//make sure we dont go out of index
					++index;		//avoid over stepping array index bounds
				count = 10;			//make sure each list is 10 nodes long
			}

			if(f_in.peek() == '\n')			//check if we hit the new line yet (end of file)
				f_in.ignore();

			file_end = f_in.peek();
		}

		f_in.close();					//close file

	}else
		cout << "\no Something is wrong with the Car.txt file." << endl;

}

// ======================= DISPLAY MAP ============================= //
/*	begin the map display, set the start and head index locations send to traverser to print the right order of labels on the map 
*/
int GPS_Map::display_map()
{
	int index = 0, num = 1;
	start = table;
	end = (table + table_size);

	cout << "\no GPS Map:" << "\n+++++++++++" << endl;

	int val = traverser(start, end, index, num);
	cout << "\n=====================================================================================" << endl;

	return 1;
}

/*	display the map labels to make its clear whats what, it will call the recursive function to display the LLL from the head pointer in the 
	index, it will then increment the index
*/
int GPS_Map::traverser(node ** start, node ** end, int index, int & num) const
{
	if(start == end)
		return 1;

	if(index == 0)						//labels
		cout << "\n\tO Parking Lane:\t |";

	else if(index == 4)
		cout << "\n\tO Bus Lane:\t |";

	else
	{
		cout << "\n\tO Lane " << num << ":\t |";
		++num;
	}

	display_map(*start);			//send to recursive display by printing from the LLL sending it the head ptr

	return traverser(++start, end, ++index, num);	//increment the index to send the next head ptr
}

/*	recursive function to display the data in the ARR list by sending it the head pointer and recursinig to the end of the list		*/
int GPS_Map::display_map(node * head) const
{
	if(head->gonext() == NULL)
	{	
		cout << head->getdata() << " |" << endl;	//print data at end
		return 1;
	}

	cout << head->getdata() << " -> ";			//print data

	return display_map(head->gonext());			//recurse to next node
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// ============================ MY CAR CLASS FUNCTIONS ========================== //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

// ====================== MY_CAR CONSTRUCTOR ============================ //
/*	my car constuctor with intialization list to set data members				*/
My_Car::My_Car() : car_speed(0), brake_time(0), proxcimity_front(0), proxcimity_left(false), proxcimity_right(false), stoplight_prox(0)
{
}

// ====================== CAR_SPEED SETTER ============================ //
/*	with prompt from main set the car speed data memeber and with that set the brake time 			*/
int My_Car::input_speed(int val)
{
	car_speed = val;		//data member set
	brake_time = 1;
}

// ==================== SEARCH AND LOCATE CAR ====================================== //

//	traverse through the array indexes from 1 to 3 so we dont waste time looking in the parked and bus lanes. once found save it to node temp by returning the 
//ptr location. when found we will return back to input speed saving the ptr location. through this function we will also find the index the car is in along
//with how many nodes deep in the list the car is so we can check the left and right lane node locations to wherever the car currently is in the map

node* My_Car::locate(node ** start, node ** last_lane, node *& temp, bool &found, int &depth, int &index)
{
	if(start == last_lane) 					//have it skip looking in the bus lane	
	{
		temp = search(*start, found, depth);		//send in head ptr of the current index
		return temp;
	}

	temp = search(*start, found, depth);			//send in head ptr of the current index

	if(found == true)					//if we already found the location of our car return early and stop traversal
		return temp;

	depth = 1;						//reset depth to begin at 1 for the new list to check

	return locate(++start, last_lane, temp, found, depth, ++index);
}

/* 	find the actual mycar node location which is 'X' on the map		*/
node* My_Car::search(node * head, bool & found, int &depth)
{
	if(head->gonext() == NULL)				//didnt find car
	{
		return NULL;	
	}

	if(head->getdata() == 'X')				//found car
	{
		found = true;
		return head;					//return node *& address
	}

	return search(head->gonext(), found, ++depth);		
}

// ====================== CHECK SURROUNDINGS ============================ //
/*	this fucntion will call the fucntions that determine the car lengths (nodes) away from the next car our car is, same with the stop light and if 
	there are cars to our left and right of our car and set the data members	
*/
int My_Car::check_safty(node * car, int index, int depth)
{
	int lengths = 0, counter = 1;
	node * looker = car;
	node * holder = NULL;

	//if the proxcimity front gets a 0 then there are no cars ahead, make sure this is disped in car info			
	proxcimity_front = look_ahead(looker->gonext(), lengths, true);			//find front prox
	stoplight_prox = look_ahead(looker->gonext(), lengths, false);			//find stoplight prox

	--index;									//decrement index to check our left
	node * left = table[index];							//set to that head pointer
	proxcimity_left = check_left_right( left, holder, depth, counter, false);	//call function for left	
	
	index += 2;									//increment to the right side
	node * right = table[index];							//set to that head pointer
	proxcimity_right = check_left_right( right, holder, depth, counter, false);	//call function sending it the right headpointer
	return 1;
}

// ====================== CHECK FRONT PROXCIMITY ============================ //
/*	find the number of nodes ahead the next car or stop light bool will be true for the car and false for the light, on each recurse call it
	it increment length until it finds one then returns te number of nodes, otherwise it returns 0 to specify no cars or lights ahead
*/
int My_Car::look_ahead(node * looker, int lengths, bool light_or_car)
{
	if(looker == NULL)					//no cars or light ahead	
	{
		return 0;
	}

	if(light_or_car == true && looker->getdata() == 'C')	//car ahead
	{
		if(lengths == 0)
			return -1;
		else
			return lengths+1;			//return number of nodes counted includeing the one were on
	}

	if(light_or_car == false && looker->getdata() == '=')	//light strip ahead
	{	
		if(lengths == 0)
			return -1;
		else
			return lengths+1;			//return number of nodes counted includeing the one were on
	}

	return look_ahead(looker->gonext(), ++lengths, light_or_car);
}

// ====================== CHECK LEFT AND RIGHT PROXCIMITY ============================ //
	//find out if the nodes to our left and right has a car sitting in it, if we are being called by the change lane function and our data is a C or P
	//then we are on the node one behind our car but on its left or right. if the next node after that is a space then we can change to that lane using
	//the pointer to that other car. we find the match in our LLL and then basically set our car to its gonext node 
bool My_Car::check_left_right(node * head, node *& holder, int depth, int counter, bool changer)
{
	if(!head)
		return false;
	if(counter == depth)
	{
		//we are changeing lanes as this function was called by the change lane func, it will take the ptr location
		if(changer == true && (head->getdata() == 'C' || head->getdata() == 'P'))	
		{	// from one depth before being perfectly alongside our mycar node location to compare with the c_node class where
			if(head->gonext()->getdata() == ' ')	
			{
				holder = head;	//save this obsticle location in holder to be found in our LLL to use it to set our car as its next node
				return true;	
			}else
				return false;
		}else	//not being called by the change lane function
		{
			if(head->gonext()->getdata() == ' ') 	//if node to my direct left or right is a free space return true
				return true;	
			else				//node was not free so return false
				return false;
		}	
	}
	return check_left_right(head->gonext(), holder, depth, ++counter, changer);
}

// ====================== CAR DATA DISPLAY ============================ //
/*	this function just displays all the data members from the my car class that we found from our proxcimity functions and our car speed, brake time, 
	if we are stopped or in motion.
*/
int My_Car::car_data_disp()
{
	cout << "\no Car Data:" << "\n+++++++++++" << endl;
	cout << "\n\to Current Speed: " << car_speed << endl;
	cout << "\n\to Amount of car lengths needed to stop: " << brake_time << endl;
	cout << "\n\to Currently in motion or stopped / moving: ";

	if(car_speed == 0)
		cout << " STOPPED " << endl; 
	else
		cout << " IN MOTION " << endl;

	if(proxcimity_front == 0)
		cout << "\n\to Front proxcimity: NO CARS AHEAD" << endl;

	else if(proxcimity_front == -1)	
		cout << "\n\to Front proxcimity: CAR DIRECTLY AHEAD" << endl;
	else	
		cout << "\n\to Front proxcimity: NEXT CAR IS " << proxcimity_front << " CAR LENGTHS AHEAD" << endl;

	if(proxcimity_left == true)
		cout << "\n\to Left proxcimity: OPEN" << endl;
	else
		cout << "\n\to Left proxcimity: CLOSED" << endl;

	if(proxcimity_right == true)
		cout << "\n\to Right proxcimity: OPEN" << endl;
	else
		cout << "\n\to Right proxcimity: CLOSED" << endl;

	if(stoplight_prox == 0)
		cout << "\n\to Stoplight proxcimity: NO LIGHT AHEAD" << endl;

	else if(stoplight_prox == -1)	
		cout << "\n\to Stoplight proxcimity: LIGHT DIRECTLY AHEAD" << endl;
	else	
		cout << "\n\to Front proxcimity: NEXT LIGHT IS " << stoplight_prox << " CAR LENGTHS AHEAD" << endl;

}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// ============================ STOPLIGHT CLASS FUNCTIONS ========================== //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

// ====================== STOPLIGHT CONSTRUCTOR ============================ //
/*	initializes my intial color to green, and the counter to 1 	*/ 
StopLight::StopLight(char color) : Light_color(color), lightcounter(1)
{
}

// ====================== CHANGE LIGHT COLOR ============================ //
/*	these two functions will cycle through the interations of chars G Y R in sets of 3, it will change on every movement of the car or choice by the user 	*/
int StopLight::LightChange()
{
	if(lightcounter == 4)		//reset back to 1
		lightcounter = 1;

	return LightChange(lightcounter);
}

int StopLight::LightChange(int & lightcounter)
{
	if(lightcounter == 1)		//determine light color based on what our counter value is at
		Light_color = 'G';

	else if(lightcounter == 2)
		Light_color = 'Y';

	else
		Light_color = 'R';

	++lightcounter;			//increment the counter
	return 1;		
}

// ====================== CHECK LIGHT COLOR ============================ //
/*	this function will just check what color the light is at and report back to the user about what is by the user_reports function 	*/
char StopLight::check_light()	
{
	if(Light_color == 'R')
		user_report(2);	
	else if(Light_color == 'Y')
		user_report(7);
	else
		user_report(6);
	return Light_color;
}
