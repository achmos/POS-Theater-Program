#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

//********************************
//*Global Variables and Constants*
//********************************

//constants used in program
const int row_size = 16;        //constant for row number
const bool selling = true;      //named constant that tells get change status to sell the seat
const bool returning = false;   //named constant that tells get change_status to return the seat

//global variables to hold info about the seats
//the arrays are all set one bigger so appending data to them is easier
bool theater[row_size][31];  //2D array to hold the status of all the seats 

//array that holds the prices for the rows; its initialized with default values
double price[row_size] = {0, 20, 20, 20, 20, 20, 15, 15, 15, 15, 15, 10, 10, 10, 10, 10};

//arrays used for running totals
int available[row_size] = {0, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30};
int unavailable[row_size] = {0};   //partial initialization
double sales_row[row_size] = {0};  //partial initialization

//global variables to hold the specific seat
int row;
int col;

//*********************
//*function prototypes*
//*********************

//functions that call a specific OS function (in this case DOS commands)
void pauseScreen();  //uses pause.exe
void clearScreen();  //uses cls.exe

//lower functions that perform basic tasks
void return_status();
int change_status(bool);
void get_seat();

//higher functions that call lower functions in organized procedures
void sell_seat();
void return_seat();
void seat_status();

//functions that do not use lower functions and are stand alone
void theater_options(const bool &);
void open_theater(bool &); 
void close_theater(bool &);
void row_status();
void output_chart(ostream &);

//Function Main
int main() {
    int choice;
    bool theater_open = false;
    
    do {
    	clearScreen();
        
        //menu screen that displays to the user the different functions the program has
        cout << endl;
        cout << setw(50) << "POS Theater Manager\n" << endl;
        
        cout << setw(45) << "Theater Status: "; 
        if (theater_open == true)
        	cout << "Open";
        else
        	cout << "Closed";
        cout << endl << endl;
        
        cout << setw(57) << "Please select on of the following\n" << endl;
        
        cout << setw(42) << "1: Open Theater\n";
        cout << setw(41) << "2: Sell a seat\n";
        cout << setw(43) << "3: Return a seat\n";
        cout << setw(47) << "4: Check seat status\n";
        cout << setw(46) << "5: Check row status\n";
        cout << setw(54) << "6: Seating Chart and Totals\n";
        cout << setw(43) << "7: Close Theater\n";
        cout << setw(42) << "8: Quit Program\n";
        cout << endl;
        
        //the user is then required to input their choice
        cout << setw(26) << ">:";
        cin >> choice;
        
        if (theater_open == false) {    
			//switch statement for when the theater is closed
            switch (choice) {
                case 1: open_theater(theater_open);     //open_theater function called
                break;
                case 2: theater_options(theater_open);  //theater_options tells the user this choice (and others) aren't available
                break;
                case 3: theater_options(theater_open);
                break;
                case 4: theater_options(theater_open);
                break;
                case 5: theater_options(theater_open);
                break;
                case 6: theater_options(theater_open);
                break;
                case 7: theater_options(theater_open);
                break;
                case 8: cout << setw(60) << "Thank you for using POS Theater Manager.\n";
                break;
                default: cout << setw(50) << "You must select 1-8\n";
                pauseScreen();
            }
        }
        else {   
			//switch statement for when the theater is open
            switch (choice) {
            	case 1: theater_options(theater_open);  //theater_options tells the user this choice (and others) aren't available
                break;
                case 2: sell_seat();      //function for selling a seat
                break;
                case 3: return_seat();    //function for returning a seat
                break;
                case 4: seat_status();    //function for checking a seat's status
                break;
                case 5: row_status();     //function for checking a row's status
                break;
                case 6: output_chart(cout);   //function for showing the totals
                        pauseScreen();
                        break;
                case 7: close_theater(theater_open);  //function to close the theater
                break;
                case 8: theater_options(theater_open);
                break;
                default: cout << setw(50) << "You must select 1-8\n";
                pauseScreen();
            }
    	}
    } while (choice != 8 || theater_open != false);
    pauseScreen();

	return 0;
} //end function main

//function to clear the screen
void clearScreen() {
    system("cls");
}
//function to pause processing
void pauseScreen() {
    system("pause");
}
//function to open the theater
void open_theater(bool &refvar) {
    clearScreen();
     
    //variables
    double temp;         //variable to hold the the price of a row before it is accepted as a valid price
    bool valid = false;  //variable used for loop control within this function
    refvar = true;       //change the theater_status bool to "true"
    int count;           //variable to control "for" loops
    char choice;         //variable to hold the user answer
    char acceptable;     //varaible to hold the user answer that controls the loop for assigning prices
     
    //the default values for the seats are displayed to the screen
    for (count = 1; count < 16; count++) 
    	cout << setw(37) << "Row " << count << ": " << price[count] << endl;    
     
    //loop to ask user if they want to use defaults or they own values
    do {
        cout << endl << "Would you like to open the theater with these default values? [Y/N] ";
        cin >> choice;
          
        //if statements to validate user input
        if (choice == 'y' || choice == 'Y')
            valid = true;
        else if (choice == 'n' || choice == 'N')
            valid = true;
        else 
            cout << endl << setw(54) << "You must select either Y or N." << endl;
    } while (valid == false);
		
    //if the user doesn't want defaults, then they input their owm values
    if (choice == 'n' || choice == 'N') {
        //main loop that is for getting new values
        do {
            cout << endl << setw(57) << "Please assign prices to each row:\n";
               
            //loop to obtain prices for each row
            for (count = 1; count < 16; count++) {
                valid = false;   //valid is reset to false
                do {
                    cout << setw(37) << "Row " << count << ": ";
                    cin >> temp;
                         
                    //if statements to test if the price is valid
                    if (temp < 1)
                        cout << setw(52) << "That is an invalid price\n";
                    else {
                        if (temp > 300)
                            cout << setw(52) << "That is too high a price\n";
                        else {
                            price[count] = temp; //the price is then written to its repective part of the array
                            valid = true;        //the loop is termanated
                        }
                    }
                } while (valid == false);
            }
            valid = false;   //valid is reset to false for the next loop
               
            //the values the user just entered are now outputed to the screen
            cout << endl << setw(58) << "These are the Row Prices you entered:" << endl << endl;
            
            for (count = 1; count < 16; count++)
                cout << setw(37) << "Row " << count << ": " << price[count] << endl;
               
            //loop to ask user if the values they entered are correct
            do {
                cout << endl << setw(56) << "Are these values acceptable? [Y/N] ";
                cin >> acceptable;
 
                //if statements to validate user input
                if (acceptable == 'y' || acceptable == 'Y')
                    valid = true;
                else if (acceptable == 'n' || acceptable == 'N')
                    valid = true;
                else 
                    cout << endl << setw(54) << "You must select either Y or N." << endl;
            } while (valid == false);
        } while (acceptable == 'n' || acceptable == 'N');
        //main loop is done
    }
    pauseScreen();
}
//function to give a message that the theater is closed or open
void theater_options(const bool &open) {
    if (open == false)
        cout << setw(61) << "You can't do that, the theater is closed!\n";
    else
        cout << setw(60) << "You can't do that, the theater is open!\n";
    pauseScreen();
}
//higher function for selling a seat
void sell_seat() {
    clearScreen();
    int val_sell;  //variable to hold the value returned form change_status
     
    cout << endl << setw(46) << "Sell a Seat\n" << endl;
     
    //get_seat is called to obtain the specific seat
    get_seat();
    val_sell = change_status(selling);
    if (val_sell == 1)
        return_status();
    else
        cout << setw(52) << "The seat has been sold.\n";
    pauseScreen();
}
//higher function for returning a seat
void return_seat() {
    clearScreen();
    int val_return;  //variable to hold the value returned form change_status
     
    cout << endl << setw(46) << "Return a Seat\n" << endl;
     
    //get_seat is called to obtain the specific seat
    get_seat();
    val_return = change_status(returning);
    if (val_return == 1) 
        return_status();
    else
        cout << setw(54) << "The seat has been returned.\n";
    pauseScreen();
}
//higher function to check on a specific seat's status
void seat_status() {
    clearScreen();
     
    cout << setw(52) << "Check the Status of a seat" << endl;
     
    get_seat();         //get_seat called to obtain the seat
    return_status();    //return_status tells the user what the status of the seat is
    pauseScreen();
}

//function to see if the row is still available
void row_status() {
    clearScreen();
     
    cout << setw(52) << "Check the Status of a Row\n" << endl;
     
    bool valid = false; // variable to control loop
    do {
        cout << endl << "Please enter the row number that is being checked\n";
          
        //ask user for the row number
        cin >> row;
          
        //test the user input for validation
        if (row > 15 || row < 1)
            cout << "That is an invalid row number\n";
        else
            valid = true; //if input is valid, the loop is terminated
    } while (valid == false);
    //the status of the row is then given
    cout << "This row has " << available[row] << " seat(s) available and " << unavailable[row] << " seat(s) taken\n";
    pauseScreen();
}

//lower function that tests the status of a seat
void return_status() {
    if (theater[row][col] == false)
        cout << setw(56) << "This seat is already available.\n";
    else
        cout << setw(54) << "This seat is already taken.\n";
	return;
}

//lower function that changes the status for a seat
int change_status(bool value) {   
	//the value that is passed to the function is the status the seat has to change to
    if (theater[row][col] == value)
        return 1;
        //the seat is checked to see if it is already set to that value, and return "1" if it is
    else {   
		//the seat is changed to the value if it previously wasn't
        theater[row][col] = value;
          
        //depending if the seat was sold or returned, running totals are changed to reflect it
        if (value == true) {
			//statements if the seat was sold
            available[row]--;
            unavailable[row]++;
            sales_row[row] += price[row];
        }
        else if (value == false) { 
			//statements if the seat was returned 
            available[row]++;
            unavailable[row]--;
            sales_row[row] -= price[row];
        }
        return 0;
    }
}
//lower function to get the seat's number from the user
void get_seat() {
    //variable to control loop
    bool valid = false;
     
    //loop to obtain data
    do {
        cout << setw(67) << "Please enter the row number of the seat, and the seat \n";
        cout << setw(69) << "number itself, seperated by a space, and then press enter.\n";
          
        //ask user for the specific seat
        cin >> row >> col;
          
        //test the user input for validation
        //the row is tested first
        if (row > 15 || row < 1)
            cout << setw(55) << "That is an invalid row number\n" << endl;
        else {    
			//then the seat number is tested
            if (col < 1 || col > 30)
            	cout << setw(55) << "That is an invalid seat number\n" << endl;
            else
                valid = true; //if input is valid, the loop is terminated
        }
    } while (valid == false);
}

//function to close the theater
void close_theater(bool &refvar) {
    refvar = false;     //theater_open is changed to false
    char save;          //variable that holds answer to save data or not
    char filename[81];  //variable to hold the name of the file the user specifies
    bool valid = false; //variable used for loop control
    int index = 0;
    int count = 0;
     
    //output chart is called to restate tbe totals
    clearScreen();
    output_chart(cout);   
    pauseScreen();
     
    //loop for asking user to save or not
    do {
        cout << endl << "Would you like to save the information stored to a text file? [Y/N] ";
        cin >> save;
 
        //if statements to test user input
        if (save == 'y' || save == 'Y')
            valid = true;
        else if (save == 'n' || save == 'N')
            valid = true;
        else 
            cout << endl << setw(54) << "You must select either Y or N." << endl;
    } while (valid == false);

    if (save == 'Y' || save == 'y') {
        cout << endl << setw(21) << "Please enter a name for the text file that\n";
        cout << setw(21) << "will be created, followed by a \".txt\": ";
        cin >> filename;  //the user enters a filename followed by .txt
          
        //a file stream object is opened for output only and close after seating chart has been outputed to the file
        ofstream OutputFile;
        OutputFile.open(filename);
        output_chart(OutputFile);
        OutputFile.close();
    }
     
    //loop to reset values to defaults
    for (index = 1; index < 16; index++) {
    	//seats available and sold, and sales for each row are reset
        available[index] = 30;
        unavailable[index] = 0;
        sales_row[index] = 0;
         
        //prices are reset to default
        if (index >= 1 && index <= 5)
            price[index] = 20;
        else if (index >= 6 && index <= 10)
            price[index] = 15;
        else if (index >= 11 && index <= 15)
            price[index] = 10;
         
        //seats are set to false
        for (count = 1; count < 31; count++)
            theater[index][count] = false;
    }
    pauseScreen();
}

//function to output seating and totals chart (to cout or a ofstream object
void output_chart(ostream &obj) {
     
    int total_sold = 0;       //running total for sold seats
    int total_unsold = 0;     //running total for available seats
    double total_sales = 0;   //running total for sales made
    int index;   //variables for loop control
    int count;
     
    //column headers are outputed to screen
    obj << endl;
    obj << setw(36) << "Seat Numbers\n";
     
    obj << setw(7) << "Row";
    obj << setw(10) << "Price";
    obj << setw(37) << "123456789012345678901234567890";
    obj << setw(12) << "Available";
    obj << setw(8) << "Sales";
    obj << endl;
     
    //main loop for rows
    for (index = 1; index < 16; index++) {
        //row number and price are outputed
        obj << setw(6) << index;
        obj << setw(12) << setprecision(2) << fixed << price[index];
    	obj << "      ";
         
        //inner loop for individual seats
        for (count = 1; count < 31; count++) {
            //seatting chart
            if (theater[index][count] == true)
                obj << "*";
            else
                obj << "#";
        }
         
        //the number of available seats, and sales recieved is outputed
        obj << setw(8) << available[index];
        obj << setw(12) << setprecision(2) << fixed << sales_row[index];
        obj << endl;
         
        //running totals are calculated
        total_sold += available[index];
        total_unsold += unavailable[index];
        total_sales += sales_row[index];
    }
    //running totals are outputed
    obj << setw(74) << "_______________" << endl;
    obj << setw(34) << "  * = Sold" << setw(24) << "Total Avail" << setw(4) << total_sold;
    obj << setw(12) << total_sales << endl;
    obj << setw(34) << "# = Unsold" << setw(24) << " Total Sold" << setw(4) << total_unsold << endl;
}
