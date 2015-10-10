//David Harkins - Homework #3
#include <queue>
#include <vector>
#include <stdio.h>
#include <time.h>   
#include <stdlib.h>

	//Every 60 time units new people show up.
const unsigned int Period_New_Customers_Showup = 60;

	// How random objects are created. The goal is to make something flexable so percentages can be changed and more added.
struct Random_Object{

		//The change percentage out of 100%
	unsigned int Chance;

		//The result if it is picked.
	unsigned int Result;

	//Default constructor
	Random_Object(unsigned int Argument_Result, unsigned int Argument_Chance){

		Chance = 0;
		Result = 0;

		Chance = Argument_Chance;
		Result = Argument_Result;
	}

};

	//Contains the customer's information.
struct Customer{
		
		//The amount of time the customer waited in line.
	unsigned int Wait_Time_Line;

		//Contains the amount of total wait time, line + required time.
	unsigned int Wait_Time_Total;

		//used to calculate the amount of time in the line based off when they entered and when they were removed.
	unsigned int Enter_Line_Time;
		
		//Used for debugging mostly.
	unsigned int Exit_Line_Time; 

		//The time left the teller. Decreases until the customer gets to zero which means they are complete.
	unsigned int Required_Time_With_Teller;

	bool Empty;

	Customer(){
		Wait_Time_Line = 0;
		Wait_Time_Total = 0;
		Enter_Line_Time = 0;
		Exit_Line_Time = 0;
		Required_Time_With_Teller = 0;
		Empty = true;
	}
		
};

	//Handles the customer and waiting for the customer's time to run out.
class Bank_Teller{
	
		//The current customer the teller is helping.
	private: Customer Current_Customer; //Null for no active customer.

	public: Bank_Teller(){}

	public: void Set_Customer(Customer Argument){
		Current_Customer = Argument;
	}

		//Return the customer and null out the pointer so it wont continue to process.
	public: Customer Get_Customer(){

		Customer Temp = Current_Customer;

		Current_Customer.Empty = true;

		return Temp;
	}


		/* Advance the process of the customer by the teller.
			Useage: Tick(time unit (default of 10))
			Return: 
					False: Customer needs more time, do nothing
					True: Customer is done so add it to the stats and give it a new one.
		*/
	public: bool Tick(unsigned int Time = 10){

	
			//No customer, request a new one.
		if (Current_Customer.Empty){
			return true;
		}

		//If our time advance will finish with the customer.
		if (Current_Customer.Required_Time_With_Teller <= Time){
			//The customer is finished this tick, clean it up.

			Current_Customer.Required_Time_With_Teller = 0;

			return true;
		}else{
			//The customer will need more time to finish up so decrease the time and end.
			
			Current_Customer.Required_Time_With_Teller -= Time;

			return false;
		}
	}


};

	//Simulates the bank over a period of time, it uses Random_Object, Customer and Bank_Teller. 
class Simulation{

		//The total amount of time the customers waited in line.
	public: long long Average_Wait_Time_Line;
			
		//Contains the amount of total customers
	public: long long Average_Wait_Time_Total;
	
		//The amount of time left until the simulation will quit.
	private: int Time_Left;

		//Tracking for how long the customer has been waitting in the line.
	int Time_Past;

		//The amount time advances each tick.
	int Time_Unit;

		//The random object information.
	std::vector<Random_Object> Customer_Entering_Chance;
	std::vector<Random_Object> Customer_Time_Chance;

		//The list of employees, aka tellers.
	private: std::vector<Bank_Teller> Employees;

		//The waitting list of customers.
	private: std::queue<Customer> Customer_Line_List;

		//Default constructor
	public: Simulation(){ 

		Average_Wait_Time_Line = 0;
		Average_Wait_Time_Total = 0;
		Time_Left = 100000;
		Time_Past = 0;
		Time_Unit = 10;

		Load_Data(); //Generate the data for random.
	}

	private: void Process_Results(Customer Argument){

			//Add the stats from the customer to the total and increase the counter.
		Average_Wait_Time_Line += Argument.Wait_Time_Total + Argument.Wait_Time_Line;
			
		Average_Wait_Time_Total++;

	}

		//Generate the customer based off the random Customer_Time_Chance
	private: void Generate_Customer(){

		int Customers_Time_With_Teller = 0;

		unsigned int Current = 0;
		unsigned int Random_Number = rand() % 100 + 1; // 1 - 100

		/* When a customer enters the bank, it is assigned a random number of seconds that their interaction with the teller will take :

			Amount of time		Percentage
			30 						10
			40 						 5
			50 						10
			60 						10
			70 						 0
			80						15
			90						25
			100 					10
			110 					15
		*/		

			//Go through each percent and see if it tips it over the scales of being that number or not.
		for (unsigned int g = 0; g < Customer_Time_Chance.size(); g++){

			Current += Customer_Time_Chance[g].Chance;

			if (Random_Number <= Current){
				Customers_Time_With_Teller = Customer_Time_Chance[g].Result;
				break;
			}
		}


			//Create the new struct of the customer.
		Customer New_Customer;
		New_Customer.Required_Time_With_Teller = Customers_Time_With_Teller;
		New_Customer.Wait_Time_Line = 0;
		New_Customer.Wait_Time_Total = New_Customer.Required_Time_With_Teller; //We know how long he will be talking to the teller.
		New_Customer.Enter_Line_Time = Time_Past;
		New_Customer.Empty = false;

		Customer_Line_List.push(New_Customer);
		
	}

		//Generate how many new customers are created based off Customer_Enter_Chance
	private: void Check_For_New_Customers(){

		int Customers_To_Generate = 0;
		
		unsigned int Current = 0;
		unsigned int Random_Number = rand() % 100 + 1; // 1 - 100


		/*
		Number of Customers 	Percentage chance
		0 							15
		1							20
		2 							25
		3 							10
		4 							30
		*/

			//Go through each percent and see if it tips it over the scales of being that number or not.
		for (unsigned int g = 0; g < Customer_Entering_Chance.size(); g++){			

			Current += Customer_Entering_Chance[g].Chance;

			if (Random_Number <= Current){
				Customers_To_Generate = Customer_Entering_Chance[g].Result;
				break;
			}
		}


		for (int i = 0; i < Customers_To_Generate; i++){
			Generate_Customer();
		}
				
	}

		//The main function to simulate the bank.
	public: void Process(){

		printf("Starting Simulation: Employees: %d\n", (unsigned int) Employees.size());

			//Simulate 
		while (Time_Left > 0){

				//The amount of time we are advancing.
			int Tick_Amount = 0;


				//Special case where there is less then a full unit of time so we only advance what we have left.
			if (Time_Unit > Time_Left){

					//Use just what is left.
				Tick_Amount = Time_Left;
				Time_Left = 0;
			}else{

					//Use the full unit.
				Tick_Amount = Time_Unit;
				Time_Left -= Time_Unit;
			}
			

			//check if it's on the beginning of the minute so we can add more customers.			
			if (Time_Past % Period_New_Customers_Showup == 0){

					//Check for the new customers.
				Check_For_New_Customers();
			}
			


				//Process all of the employees.
			for (unsigned int i = 0; i < Employees.size(); i++){
				bool Result = Employees[i].Tick(Tick_Amount);

				//Check if they need a new customer.
				if (Result){
					
						//They need a new customer.
					Customer Result_Customer = Employees[i].Get_Customer();

						//Try to give them a new customer.
					if (Customer_Line_List.size() > 0){
							//Pull a customer from the list.
						Customer NewCustomer = Customer_Line_List.front();
						Customer_Line_List.pop();

						NewCustomer.Exit_Line_Time = Time_Past;
						
						//Calculate how long they have been in the waitting list.
						NewCustomer.Wait_Time_Line = Time_Past - NewCustomer.Enter_Line_Time;


							//Send them to a teller.
						Employees[i].Set_Customer(NewCustomer);						
					}

						//Check if the customer they return is real or not
					if (Result_Customer.Empty == false){

						//Process their information.
						Process_Results(Result_Customer);

						//After having the customer fill out a survey sacrifice them to the almight space lord.
					}
					
				}
			}


			Time_Past += Tick_Amount;
		}


	}

		//Used to generate the random data based off the problem Nate T posted.
	void Load_Data(){

		/*
		Number of Customers 	Percentage chance
			0 							15
			1							20
			2 							25
			3 							10
			4 							30
		*/

		Random_Object Customer_Enter_1(0, 15);
		Random_Object Customer_Enter_2(1, 20);
		Random_Object Customer_Enter_3(2, 25);
		Random_Object Customer_Enter_4(3, 10);
		Random_Object Customer_Enter_5(4, 30);

		Customer_Entering_Chance.push_back(Customer_Enter_1);
		Customer_Entering_Chance.push_back(Customer_Enter_2);
		Customer_Entering_Chance.push_back(Customer_Enter_3);
		Customer_Entering_Chance.push_back(Customer_Enter_4);
		Customer_Entering_Chance.push_back(Customer_Enter_5);
		
		/* When a customer enters the bank, it is assigned a random number of seconds that their interaction with the teller will take :

			Amount of time		Percentage
			30 						10
			40 						 5
			50 						10
			60 						10
			70 						 0
			80						15
			90						25
			100 					10
			110 					15
		*/

		Random_Object Customer_Time_1(30, 10);
		Random_Object Customer_Time_2(40, 5);
		Random_Object Customer_Time_3(50, 10);
		Random_Object Customer_Time_4(60, 10);
		Random_Object Customer_Time_5(70, 0); //Note this will actually never be used because it is 0%
		Random_Object Customer_Time_6(80, 15);
		Random_Object Customer_Time_7(90, 25);
		Random_Object Customer_Time_8(100, 10);
		Random_Object Customer_Time_9(110, 15);

		Customer_Time_Chance.push_back(Customer_Time_1);
		Customer_Time_Chance.push_back(Customer_Time_2);
		Customer_Time_Chance.push_back(Customer_Time_3);
		Customer_Time_Chance.push_back(Customer_Time_4);
		Customer_Time_Chance.push_back(Customer_Time_5);
		Customer_Time_Chance.push_back(Customer_Time_6);
		Customer_Time_Chance.push_back(Customer_Time_7);
		Customer_Time_Chance.push_back(Customer_Time_8);
		Customer_Time_Chance.push_back(Customer_Time_9);


		}
	
		//Change the amount of employees to use in the simulation.
	public: void Set_Employee_Count(unsigned int Argument){

			//If we have too many employees delete some until we get to the correct amount.
		while (Employees.size() > Argument){
			Employees.pop_back();
		}


			//If we need more employees add more.
		Bank_Teller Temp;
		while (Employees.size() < Argument){
			Employees.push_back(Temp);
		}
	}

};


int main(){

		//Credit dawg, get some.
	printf("David Harkins - \"I am friends with a robot, my friend is my phone.\"\n");

		//Generate a random seed.
	srand( (unsigned int) time(NULL));
	
		//Simulate the bank with different amounts of tellers.
	for (unsigned int i = 2; i < 10; i++){

		Simulation Bank;

		Bank.Set_Employee_Count(i);

		Bank.Process();

		int results = (int) (Bank.Average_Wait_Time_Line / Bank.Average_Wait_Time_Total);

		printf("%d tellers %d wait time\n", i , results);
	}

	return 0;
}

