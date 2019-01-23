//Liam Rotchford
//9/29/18

#include <iostream>
#include <cstring>
#include <cctype>
#include <fstream>	//for file loading
#include <thread>	//for the sleep fucntion to delay execution

// ============================== NODE CLASS FOR GPS MAP {ARR} "HAS A" =============================== //
/*	this class holds the setters and getters to access and set the data for the nodes which are my char ARR nodes
*/
class node		//for array of linked list used in map
{
	public:
		void setdata(char value);		 
		char getdata();

		node*& gonext();
		void setnext(node * temp);

	protected:
		char data;
		node * next;
};

// ====================== CAR NODE CLASS FOR CAR_MANAGER {LLL} "HAS A" ============================ //
/*	this class holds the setters and getters to access and set the data for the car nodes that are the nodes for the LLL that contain the node pointers 
	that contain the address of where the obstacles are in the ARR
*/
class car_node		//for the LLL of where the cars are in the array of linked lists map
{
	public:
		void setcar(node * temp);
		node*& getcardata();

		car_node*& gonextcar();
		void setnextcar(car_node * temp);
	
	protected:
		node * car_location;
		car_node * nextcar;
};

// ================================= GPS_MAP BASE CLASS ============================ // 
/*	this class handles the ARR road map. it builds the ARR, populates it with chars from my external file Car.txt, displays it, destorys it
*/
class GPS_Map
{
	public:
		GPS_Map();
		~GPS_Map();
		
		int builder();
		int display_map();
		int populate();

	protected:
		node ** table;		
		node ** start;
		node ** end;
		int table_size;
				

// ------------------ FUNCTIONS ----------------------------- //	
		int set_to_NULL(node *& start);
		int deconstruct(node *& head, int & num);
		
		int traverser(node ** start, node ** end, int index, int & num) const;
		int display_map(node * head) const;
};

// =========================== MY CAR CLASS, DERIVED FROM THE GPS MAP CLASS ==================== //
/*	This class contains all of the mycar surroundings data, car speed, and brake time. it has functions that will determine how far away the next car infront of
	us is, the stop light is, if the space to our left and right is free. find the car in the ARR
*/
class My_Car : public GPS_Map
{
	public:
		
		My_Car();
		
		int input_speed(int val);
		int car_data_disp();

	protected:

		int car_speed;
		int brake_time;
		int proxcimity_front;
		bool proxcimity_left;
		bool proxcimity_right;
		int stoplight_prox;	

// ------------------ FUNCTIONS ----------------------------- //	
		//find the car in map	
		node* locate(node ** start, node ** last_lane, node *& temp, bool &found, int &depth, int & index);
		node* search(node * head, bool & found, int &depth);

		int check_safty(node * car, int index, int depth);
		int look_ahead(node * looker, int lengths, bool light_or_car);
		bool check_left_right(node * head, node *& holder, int depth, int counter, bool changer);
};

// =========================== STOPLIGHT CLASS "HAS A" TO CAR MANAGER CLASS ==================== //
/*	This class handles the stop light changing colors by a char, checking the light color, light counter is the int that keeps the light cycleing by those
	three colors as the program progresses as our car moves through the map
*/
class StopLight
{

	public:
		StopLight(char color);
		char check_light();	
		int LightChange();

	protected:
		char Light_color;
		int lightcounter;

// ------------------ FUNCTIONS ----------------------------- //	
		int LightChange(int & lightcounter);
};

// =========================== CAR_MANAGER CLASS, LOWEST DERIVED CLASS, DERIVED FROM MY_CAR CLASS ==================== //

class Car_manager : public My_Car
{
	public:
		Car_manager(char color = 'G');
		~Car_manager();
			
		int build_list();		
		int cycler();

	protected:
		
		StopLight Light;	//Stoplight "has a" object
		car_node * c_head;	//car manager holds the head pointer for the car location pointers in the map ARR
		bool automated;

// ------------------ FUNCTIONS ----------------------------- //	
			//destroy the list	
		int list_destuctor(car_node *& head, car_node *& current);
			//build the LLL	
		int check_each_index(node ** start, node ** last_lane, int & first);
		int set_list(node * current, car_node *& c_head, int & first);
			//automated and user control movement
		int mover(node *& mycar,int & speed_saver,int depth, int index, int users_pick);
		int user_mover(node *& mycar, int & speed_saver, int depth, int index, int users_pick);
			//movement cases for changing lane
		int mover_changelane(node *& mycar, int index, int depth, int users_pick);
		int change_lane(node *& mycar, int index, int depth, int users_pick);
		int check_list(car_node *& c_head, node *& mycar, car_node *& to_match);
			//movement cases for light, and moving forward
		int mover_light(node *& mycar, int &speed_saver);
		int mover_forward(node *& mycar);
	
			//extra functions for assistance
		int user_options();
		int car_list_display(car_node *& c_head);
};

// ============================== MAIN SUPPORT FUNCTIONS ================================= //
/*	prompts, and displays for user in main.cpp
*/
void user_menu(Car_manager & object);
void user_rules(Car_manager & object);
void user_report(int reason);
int user_takeover();
int user_control();
