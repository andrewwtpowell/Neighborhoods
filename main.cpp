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

void findNeighborhoods(int cells[arr_height][arr_width], int neighborhoods[arr_height][arr_width], int height, int width, int n) {

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
                    
                    //Check if within distance threshold (distance_traveled < n)
                    if(distance_traveled <= n) {

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
                            for(int j = 1; j <= n; j++) {

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

void findNeighborhoodsWrapped(int cells[arr_height][arr_width], int neighborhoods[arr_height][arr_width], int height, int width, int n) {

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
                    
                    //Check if within distance threshold (distance_traveled < n)
                    if(distance_traveled <= n) {

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
                            for(int j = 1; j <= n; j++) {

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

int calcNeighborhoodArea(int cells[arr_height][arr_width], int height, int width, int n) {

    //Initialize neighborhood_area variable to 0
    int neighborhood_area = 0;

    //Declare array to contain coordinates of homes
    pair<int,int> homes[100];
    int homes_itr = 0;

    //Initialize array to contain only (-1,-1)
    for(int i = 0; i < 100; i++) {
        homes[i].first = -1;
        homes[i].second = -1;
    }

    //Find cells that contain a positive integer
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {

            if(cells[i][j] == 1) {
                homes[homes_itr].first = i;
                homes[homes_itr].second = j;
                homes_itr++;
            }
        }
    }

    //Declare pair variable to be compared against
    pair<int,int> p;

    //Declare distance, area, overlap, and overhang variables
    int d, d_x, d_y;
    int area = 0;
    float overlap;
    int overhang = 0;
    int overhang_overlap = 0;

    //Loop through homes array
    for(int i = 0; i < homes_itr; i++) {

        //Reset overhang and overhang_overlap
        overhang = 0;
        overhang_overlap = 0;

        //Set p to current home
        p.first = homes[i].first;
        p.second = homes[i].second;

        //Calculate area for p using equation n^2 + (n+1)^2
        area += n*n + (n+1)*(n+1);

        //Account for lower bound x edge case (x < n)
        if(homes[i].second < n) {

            //Calculate overhang area as (n - x)^2
            overhang += (n - homes[i].second)*(n - homes[i].second);
        }

        //Account for upper bound x edge case (x > width - 1 - n)
        if(homes[i].second > (width - 1 - n)) {

            //Calculate overhang area as (n - (width - 1- x))^2
            overhang += (n - (width - 1- homes[i].second))*(n - (width - 1- homes[i].second));
        }
        
        //Account for lower bound y edge case (y < n)
        if(homes[i].first < n) {

            //Calculate overhang area as (n - y)^2
            overhang += (n - homes[i].first)*(n - homes[i].first);
        }

        //Account for upper bound y edge case (y > height - 1 - n)
        if(homes[i].first > (height - 1 - n)) {

            //Calculate overhang area as (n - (height - 1- y))^2
            overhang += (n - (height - 1- homes[i].first))*(n - (height - 1- homes[i].first));
        }

        //Account for Upper Left corner case (y < n && x < n)
        if(homes[i].first < n && homes[i].second < n) {

            //Calculate overlap from overhang calculations in edge cases

            //Overlap limit is (n - y) + (n - x) - n - 1
            //Simplified down to limit = n - y - x - 1
            int overlap_limit = n - homes[i].first - homes[i].second - 1;
            
            //Add up  overlap until limit is reached
            for(int i = 1; i <= overlap_limit; i++) {
                overhang_overlap += i;
            }
        }

        //Account for Upper Right corner case (y < n && x > width - 1 - n)
        if(homes[i].first < n && homes[i].second > (width - 1 - n)) {

            //Calculate overlap from overhang calculations in edge cases

            //Overlap limit is (n - y) + (n - (width - 1 - x)) - n - 1
            //Simplified down to limit = n - y - width + x
            int overlap_limit = n - homes[i].first - width + homes[i].second;
            
            //Add up  overlap until limit is reached
            for(int i = 1; i <= overlap_limit; i++) {
                overhang_overlap += i;
            }
        }

        //Account for Bottom Left corner case (y > height - 1 - n && x < n)
        if(homes[i].first > (height - 1 - n) && homes[i].second < n) {

            //Calculate overlap from overhang calculations in edge cases

            //Overlap limit is (n - (height - 1 - y)) + (n - x) - n - 1
            //Simplified down to limit = n - height + y - x
            int overlap_limit = n - height + homes[i].first - homes[i].second;
            
            //Add up  overlap until limit is reached
            for(int i = 1; i <= overlap_limit; i++) {
                overhang_overlap += i;
            }
        }

        //Account for Bottom Right corner case (y > height - 1 - n && x > width - 1 - n)
        if(homes[i].first > (height - 1 - n) && homes[i].second > (width - 1 - n)) {

            //Calculate overlap from overhang calculations in edge cases

            //Overlap limit is (n - (height - 1 - y)) + (n - (width - 1 - x)) - n - 1
            //Simplified down to limit = n - height + y - width + x + 1
            int overlap_limit = n - height + homes[i].first - width + homes[i].second + 1;
            
            //Add up  overlap until limit is reached
            for(int i = 1; i <= overlap_limit; i++) {
                overhang_overlap += i;
            }
        }

        area = area - overhang + overhang_overlap;
        

        //Loop through homes array again, starting from i
        for(int j = i; j < homes_itr; j++) {

            //Reset overlap
            overlap = 0;

            //Calculate distance between p and current home
            d_y = abs(p.first - homes[j].first);
            d_x = abs(p.second - homes[j].second);
            d = d_y + d_x;

            if(d == 0)
                continue;

            //Check if distance between p and current home is less than 2*n + 1
            if( d < 2*n + 1 ) {

                //Calculate overlap
                //Overlap limit is max number of diagonal lines overlapping between points
                // 2*n + 1 is max distance between points to cause overlap, d is manhattan distance between points
                int overlap_limit = 2*n + 1 - d;

                for(int k = 1; k <= overlap_limit; k++) {
                    
                    //Calculate overlap as the summation of all the overlapping diagonal lines between points
                    //Length of each diagonal line is calculated as n - |(d_x - d_y) / 2| + (k % 2)
                    overlap += n - abs((d_x - d_y) / 2) + (k % 2);
                }
            }

            //Subtract the overlap area calculated from the total area calculated
            area = area - (int)overlap;
        }
        
    }

    return area;

}

int main(int argc, char** argv) {

    int height, width, n;
    bool random = false;

    //create 2 dimensional array of fixed height and width
    int cells[arr_height][arr_width];

    //Random Setup
    if (random) {

        srand(time(0));

        //If user does not enter parameters, randomly generate Height, Width, and Distance Threshold
        //Height >= 1
        //Width >= 1
        //Distance Threshold >= 0 (upper bound?)
        if(argc == 1) {
            
            height = rand() % 100 + 1;
            width = rand() % 100 + 1;
            n = rand() % 10;
        }
        //User enters parameters (Array Height, Array Width, Distance Threshold)
        else if(argc == 4) {
            height = stoi(argv[1]);
            width = stoi(argv[2]);
            n = stoi(argv[3]);

            //if height, width, or distance threshold < 1, terminate the program
            if(height < 1 || width < 1 || n < 1) {
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

        //Prompt user for array height (0 to 100)
        cout << "Please enter desired array height (1 to 100)" << endl;
        cin >> height;

        //Prompt user for array width (0 to 100)
        cout << "Please enter desired array width (1 to 100)" << endl;
        cin >> width;

        //Prompt user for neighborhood radius (n >= 0)
        cout << "Please enter desired neighborhood radius (n >= 0)" << endl;
        cin >> n;

        //Initialize array to have all negative integers
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                cells[i][j] = -1;
            }
        }

        //Prompt user to enter positive integer locations in row, col format
        cout << "Enter locations for positive integers in row, column format. Enter -1 to finish." << endl;
        int row, col = 0;
        while(row >= 0 && col >= 0) {

            cout << "Row: ";
            cin >> row;
            if(row < 0)
                break;
            
            cout << "Column: ";
            cin >> col;
            if(col < 0)
                break;

            cells[row][col] = 1;
        }

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
    findNeighborhoods(cells, neighborhoods, height, width, n);

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

    cout << "calcNeighborhoodArea output: " << calcNeighborhoodArea(cells, height, width, n) << endl;

    return 0;
}