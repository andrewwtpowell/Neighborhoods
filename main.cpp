#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <array>

using namespace std;

/*
Data Structures Needed:
    2-Dimensional Cells Array
    2-Dimensional Neighborhoods Array
    Height
    Width
    Distance Threshold
    Distance Travelled
    Homes Array
    Coordinate Stack

Functions:
    Find Neighborhoods

*/

const int arr_height = 100;
const int arr_width = 100;

void findNeighborhoods(int cells[arr_height][arr_width], int neighborhoods[arr_height][arr_width], int height, int width, int dt) {

    //Iterate through the cells array searching for positive cells
    //When a positive cell is found push the coordinates onto a stack
    //Calculate adjacent cells and push those coordinates onto the stack
    //Continue to do so until the distance threshold is reached
    //If another positive cell is located at one of the adjacent cells, recalculate distance

    //Enumeration for cells in neighborhoods array
    // 0 = non-neighborhood cell
    // 1 = neighborhood cell

    //Declare stack to track adjacent coordinates
    stack<pair<int, int>> st;

    //Declare variable to track distance traveled from positive cell
    int distance_traveled;

    //Initialize direction vectors for adjacent cells
    int d_x[] = { 0, 1, 0, -1 };
    int d_y[] = { -1, 0, 1, 0 };

    //Declare x and y variables for use tracking coordinates
    int x, y;

    //Declare array for tracking positive cell (home) locations
    pair<int, int> homes[100];

    //Initialize variable for iterating through homes array
    int itr = 0;

    //Iterate through cells array until a positive cell is located
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {

            //Check if cell is positive and not already part of a neighborhood
            if(cells[i][j] == 1 && neighborhoods[i][j] == 0) {

                //Push coordinates onto stack
                st.push({i,j});

                //Reset distance traveled variable
                distance_traveled = 0;

                //Add location to homes array
                homes[itr] = {i,j};
                itr++;

                //Iterate through stack until empty
                while(!st.empty()) {

                    //Get value of top element in stack and pop
                    pair<int, int> curr = st.top();
                    st.pop();

                    y = curr.first;
                    x = curr.second;

                    //Check that coordinates are outside array bounds, if so skip
                    if(y < 0 || x < 0 || y >= height || x >= width) 
                        continue;

                    //Check if cell has already been visited, skip if so
                    if(neighborhoods[y][x] != 0)
                        continue;

                    int smallest_distance, temp_distance;

                    smallest_distance = 100;

                    //Calculate distance traveled from nearest home
                    for(int i = 0; i < itr; i++) {

                        //Calculate Manhattan distance from current cell to home
                        temp_distance = abs(y - homes[i].first) + abs(x - homes[i].second);

                        //If calculated distance is less than the current smallest distance, smallest distance becomes temp distance
                        if(temp_distance < smallest_distance)
                            smallest_distance = temp_distance;
                    }

                    //Set distance_travelled to smallest calculated distance
                    distance_traveled = smallest_distance;
                    
                    //Check if within distance threshold (distance_traveled < dt)
                    if(distance_traveled <= dt) {

                        //Mark current cell as in neighborhood
                        neighborhoods[y][x] = 1;

                        //Check if cell is positive
                        if(cells[y][x] > 0) {

                            //Initialize flag to signify home presence
                            bool home_present = false;

                            //Check if cell does not exist in homes array
                            for(int i = 0; i < itr; i++) {
                                
                                //If cell exists in array, set presence flag to true
                                if(homes[i].first == y && homes[i].second == x)
                                    home_present = true;
                            }

                            //Add location to homes array if not already present
                            if(!home_present) {
                                homes[itr] = {y,x};
                                itr++;
                            }

                        }

                        //Push adjacent cells onto stack
                        for(int i = 0; i < 4; i++) {
                            
                            //Ensure all cells on adjacent vectors and within distance threshold are pushed onto stack
                            for(int j = 1; j <= dt; j++) {

                                int adjx = x + j*d_x[i];
                                int adjy = y + j*d_y[i];
                                st.push({adjy, adjx});

                                //For example given y = 2, x = 2 or (2,2) with a distance threshold of 2:
                                //(2,0), (4,2), (2,4), (0,2) and
                                //(2,1), (3,2), (2,3), (1,2) will be pushed onto the stack

                                //This fixes an issue where a cell within the distance threshold will be "hidden" 
                                //due to its adjacent cells already being captured by a previous home's neighborhood
                            }
                        }

                    }

                }

                //Clean up homes array
                for(int i = 0; i < itr; i++)
                    homes[i] = {0,0};

                itr = 0;
            }
        }
    }

}

void findNeighborhoodsWrapped(int cells[arr_height][arr_width], int neighborhoods[arr_height][arr_width], int height, int width, int dt) {

    //Functions the same as findNeighborhoods but includes wrapping neighborhoods when a home is located near an edge

    //Declare stack to track adjacent coordinates
    stack<pair<int, int>> st;

    //Declare variable to track distance traveled from positive cell
    int distance_traveled;

    //Initialize direction vectors for adjacent cells
    int d_x[] = { 0, 1, 0, -1 };
    int d_y[] = { -1, 0, 1, 0 };

    //Declare x and y variables for use tracking coordinates
    int x, y;

    //Declare variables for tracking x and y distances when wrapping
    int y_distance, x_distance;
    bool x_wrapped_over, x_wrapped_under, y_wrapped_over, y_wrapped_under;

    //Declare array for tracking positive cell (home) locations
    pair<int, int> homes[100];

    //Initialize variable for iterating through homes array
    int itr = 0;

    //Iterate through cells array until a positive cell is located
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {

            //Check if cell is positive and not already part of a neighborhood
            if(cells[i][j] == 1 && neighborhoods[i][j] == 0) {

                //Push coordinates onto stack
                st.push({i,j});

                //Reset distance traveled variable
                distance_traveled = 0;

                //Add location to homes array
                homes[itr] = {i,j};
                itr++;

                //Iterate through stack until empty
                while(!st.empty()) {

                    //Get value of top element in stack and pop
                    pair<int, int> curr = st.top();
                    st.pop();

                    y = curr.first;
                    x = curr.second;

                    //Initialize variables for bounded x and y values for wrapping
                    int bound_y = y;
                    int bound_x = x;

                    //Check if coordinates are outside array bounds, if so recalibrate coordinates to wrap to otherside
                    if(y < 0) {
                        bound_y = height + y;
                    }
                    else if(x < 0){
                        bound_x = width + x;
                    } 
                    else if(y >= height) {
                        bound_y = y - height;
                    } 
                    else if(x >= width) {
                        bound_x = x - width;
                    }

                    //Check if cell has already been visited, skip if so
                    if(neighborhoods[bound_y][bound_x] != 0)
                        continue;

                    int smallest_distance, temp_distance;

                    smallest_distance = 100;

                    //Calculate distance traveled from nearest home
                    for(int i = 0; i < itr; i++) {

                        //Calculate x and y distances from current cell to home
                        temp_distance = abs(y - homes[i].first) + abs(x - homes[i].second);

                        //If calculated distance is less than the current smallest distance, smallest distance becomes temp distance
                        if(temp_distance < smallest_distance)
                            smallest_distance = temp_distance;
                    }

                    //Set distance_travelled to smallest calculated distance
                    distance_traveled = smallest_distance;
                    
                    //Check if within distance threshold (distance_traveled < dt)
                    if(distance_traveled <= dt) {

                        //Mark current cell as in neighborhood
                        neighborhoods[bound_y][bound_x] = 1;

                        //Check if cell is positive
                        if(cells[bound_y][bound_x] > 0) {

                            //Initialize flag to signify home presence
                            bool home_present = false;

                            //Check if cell does not exist in homes array
                            for(int i = 0; i < itr; i++) {
                                
                                //If cell exists in array, set presence flag to true
                                if((homes[i].first == y && homes[i].second == x) || (homes[i].first == bound_y && homes[i].second == bound_x))
                                    home_present = true;
                            }

                            //Add location to homes array if not already present
                            if(!home_present) {
                                homes[itr] = {bound_y,bound_x};
                                itr++;

                                //Also add unbound coordinates in the case a home is found directly after wrapping
                                //unbound coordinates will be needed to properly calculate distance 
                                homes[itr] = {y,x};
                                itr++;
                            }

                        }

                        //Push adjacent cells onto stack
                        for(int i = 0; i < 4; i++) {

                            //Ensure all cells on adjacent vectors and within distance threshold are pushed onto stack
                            for(int j = 1; j <= dt; j++) {

                                int adjx = x + j*d_x[i];
                                int adjy = y + j*d_y[i];
                                st.push({adjy, adjx});

                                //For example given y = 2, x = 2 or (2,2) with a distance threshold of 2:
                                //(2,0), (4,2), (2,4), (0,2) and
                                //(2,1), (3,2), (2,3), (1,2) will be pushed onto the stack

                                //This fixes an issue where a cell within the distance threshold will be "hidden" 
                                //due to its adjacent cells already being captured by a previous home's neighborhood
                            }
                        }

                    }

                }

                //Clean up homes array
                for(int i = 0; i < itr; i++)
                    homes[i] = {0,0};

                itr = 0;
            }
        }
    }
}

int main(int argc, char** argv) {

    int height, width, dt;
    bool test = false;

    //create 2 dimensional array of fixed height and width
    int cells[arr_height][arr_width];

    //Random Setup
    if (!test) {

        srand(time(0));

        //If user does not enter parameters, randomly generate Height, Width, and Distance Threshold
        //Height >= 1
        //Width >= 1
        //Distance Threshold > 0 (upper bound?)
        if(argc == 1) {
            
            height = rand() % 100 + 1;
            width = rand() % 100 + 1;
            dt = rand() % 10 + 1;
        }
        //User enters parameters (Array Height, Array Width, Distance Threshold)
        else if(argc == 4) {
            height = stoi(argv[1]);
            width = stoi(argv[2]);
            dt = stoi(argv[3]);

            //if height, width, or distance threshold < 1, terminate the program
            if(height < 1 || width < 1 || dt < 1) {
                cout << "Invalid Arguments. Please pass in only non-zero, positive integers." << endl;
                return 0;
            }
        }
        //Invalid number of arguments passed
        else {
            cout << "Invalid Number of Arguments. Please pass in Height, Width, and Distance Threshold only." << endl;
            return 0;
        }

        //initialize array to have either 1 or -1
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                
                int num = rand();
                if(num % 17 == 0)
                    cells[i][j] = 1;
                else
                    cells[i][j] = -1;
            }
        }

    }
    //Test Setup
    else {
        height = 10;
        width = 10;
        dt = 2;

        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                cells[i][j] = -1;
            }
        }

        cells[0][1] = 1;
        cells[2][0] = 1;
        cells[2][8] = 1;
        //cells[2][2] = 1;
        //cells[3][3] = 1;
        //cells[4][2] = 1;
    }

    //Print initial array 
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(cells[i][j] == 1) {
                if(j == width-1)
                    cout << " +" << endl;
                else
                    cout << " +";
            }
            else {
                if(j == width-1)
                    cout << " ." << endl;
                else
                    cout << " .";
            }
        }
    }

    //Print breaker
    cout << "*******************************************************" << endl;

    //Declare an array that is of equivalent size to cells to track neighborhoods
    int neighborhoods[arr_height][arr_width];

    //Initialize the neighborhoods array to 0s (signifying not yet visited)
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            neighborhoods[i][j] = 0;
        }
    }

    //Find neighborhoods
    findNeighborhoodsWrapped(cells, neighborhoods, height, width, dt);

    int n_cells = 0;

    //Print neighborhoods array
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(neighborhoods[i][j] == 1) {
                if(j == width-1)
                    cout << " o" << endl;
                else
                    cout << " o";
                n_cells++;

            }
            else {
                if(j == width-1)
                    cout << "  " << endl;
                else
                    cout << "  ";
            }
        }
    }

    cout << "There are " << n_cells << " cells within neighborhoods." << endl;

    return 0;
}