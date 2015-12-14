#include <iostream>
#include <queue>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>

using namespace std;

//============================================//
//				EIGHT PUZZLE CLASS
//============================================//
class puzzle
{
    public:
    int eight_puzzle [3][3];
    vector<int> backTracing;
    vector<int> costTracing;
    vector<int> distTracing;
    int zero_row;
    int zero_col;
    int RIGHT;
	int LEFT;
	int UP;
	int DOWN;
	int last_move;
	int last_last_move;
    int cost; 
    int dist;
    int prevDist;
    int prevAStarCalc;
    int a_star_calc;
    int misplacedNodes;
    
	//--------------------------------------//
	//		EIGHT PUZZLE CONSTRUCTOR
	//--------------------------------------//
    puzzle()
    {
        for (int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < 3; ++j)
            {
                eight_puzzle[i][j] = 0;
            }
        }
        zero_row = 0;
        zero_col = 0;
        RIGHT = 1;
        LEFT = 2;
        UP = 3;
        DOWN = 4;
        last_move = 0;
        cost = 0;
        dist = 0;
        a_star_calc = 0;
        misplacedNodes = 0;
    }

	//--------------------------------------//
	//		EIGHT PUZZLE = OPERATOR
	//--------------------------------------//
	//create a assignment operator
	puzzle& operator= (const puzzle p)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				eight_puzzle[i][j] = p.eight_puzzle[i][j];

			}
		}		    
	 
		last_move = p.last_move;
		zero_row = p.zero_row;
		zero_col = p.zero_col;
        cost = p.cost;
        dist = p.dist;
        unsigned size = p.backTracing.size();
        vector<int> newBack(size);
        newBack = p.backTracing;
        backTracing = newBack;
        
        vector<int> newDist(size);
        newDist = p.distTracing;
        distTracing = newDist;
        
        vector<int> newCost(size);
        newCost = p.costTracing;
        costTracing = newCost;

	    // return the existing object
	    return *this;
	}  

	//--------------------------------------//
	//		EIGHT PUZZLE == OPERATOR
	//--------------------------------------//
	//create a comparison operator
	bool operator== (const puzzle p)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if(eight_puzzle[i][j] != p.eight_puzzle[i][j])
				{
					return false;
				}
			}
		}		    
	 
	    // return the existing object
	    return true;
	}    

	//--------------------------------------//
	//		EIGHT PUZZLE != OPERATOR
	//--------------------------------------//
	//create a comparison operator
	bool operator!= (const puzzle p)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if(eight_puzzle[i][j] == p.eight_puzzle[i][j])
				{
					return true;
				}
			}
		}		    
	 
	    // return the existing object
	    return false;
	} 

    //------------------------------------------//
    //	    EIGHT PUZZLE USER INpUT FUNCTION
    //------------------------------------------//
    void userInput()
    {
    	int userInput;
		cout << "Enter the first row of the eight puzzle " 
			<< "(3 numbers seperated by spaces): " << endl;
		cin >> userInput; 	
		eight_puzzle[0][0] = userInput;

		cin >> userInput;
		eight_puzzle[0][1] = userInput;

		cin >> userInput;
		eight_puzzle[0][2] = userInput;

		cout << "Enter the next row of the eight puzzle " 
			<< "(3 numbers seperated by spaces): " << endl;
		cin >> userInput;
		eight_puzzle[1][0] = userInput;

		cin >> userInput;
		eight_puzzle[1][1] = userInput;

		cin >> userInput;
		eight_puzzle[1][2] = userInput;

		cout << "Enter the last row of the eight puzzle "
			<< "(3 numbers seperated by spaces): " << endl;
		cin >> userInput; 
		eight_puzzle[2][0] = userInput;

		cin >> userInput;
		eight_puzzle[2][1] = userInput;

		cin >> userInput;
		eight_puzzle[2][2] = userInput;

		//find the zero position to find out starting position
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if(eight_puzzle[i][j] == 0)
				{
					zero_col = j;
					zero_row = i;
				}
			}
		}		
	}

    //--------------------------------------------------------//
    //	    EIGHT PUZZLE READ PUZZLE FROM FILE FUNCTION
    //--------------------------------------------------------//
    //reads in the puzzle from an input file
    void readByFile(char *file)
    {
    	//set up the fstream and open the file
	    ifstream fin;
	    int puzzle_num;
	    
	    //open file and check if openned
	    fin.open(file);
	    if(!fin.is_open())
	    {
	        cout << "Error opening file" << endl;
	        return;
	    }

	    //read in the puzzle from the file and set it up
		for (int i = 0; i < 3; ++i)
		{
			for(int j = 0; j < 3; ++j)
			{
				fin >> puzzle_num;
				eight_puzzle[i][j] = puzzle_num;
			}
		}   	    

		//find the zero position to find out starting position
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if(eight_puzzle[i][j] == 0)
				{
					zero_col = j;
					zero_row = i;
				}
			}
		}		

	    //close the file
	    fin.close();

	}

	//--------------------------------------//
	//	    EIGHT PUZZLE RIGHT FUNCTION
	//--------------------------------------//
	//moves the 0 position down
	bool right()
	{
		//check if valid move
		if((zero_col + 1) > 2)
		{
			return false;
		}
		else
		{
			//swap the top value with the zero position
			int temp = eight_puzzle[zero_row][zero_col+1];
			eight_puzzle[zero_row][zero_col+1] = eight_puzzle[zero_row][zero_col];			
			eight_puzzle[zero_row][zero_col] = temp;
			
			//update zero position
			zero_col = zero_col + 1;
			last_last_move = last_move;
			last_move = RIGHT;
            backTracing.push_back(RIGHT);
            distTracing.push_back(dist);
            costTracing.push_back(cost);
            
            ++cost;
			return true;
		}
	}

	//--------------------------------------//
	//	    EIGHT PUZZLE LEFT FUNCTION
	//--------------------------------------//
	//moves the 0 position down
	bool left()
	{
		//check if valid move
		if((zero_col - 1) < 0)
		{
			return false;
		}
		else
		{
			//swap the top value with the zero position
			int temp = eight_puzzle[zero_row][zero_col-1];
			eight_puzzle[zero_row][zero_col-1] = eight_puzzle[zero_row][zero_col];			
			eight_puzzle[zero_row][zero_col] = temp;
			
			//update zero position and last move
			zero_col = zero_col - 1;
			last_last_move = last_move;
			last_move = LEFT;
            backTracing.push_back(LEFT);
            distTracing.push_back(dist);
            costTracing.push_back(cost);
            ++cost;    
			return true;
		}

	}

	//--------------------------------------//
	//		EIGHT PUZZLE UP FUNCTION
	//--------------------------------------//
	//moves the 0 position up
	bool up()
	{
		//check if valid move
		if((zero_row - 1) < 0)
		{
			return false;
		}
		else
		{
			//swap the top value with the zero position
			int temp = eight_puzzle[zero_row-1][zero_col];
			eight_puzzle[zero_row-1][zero_col] = eight_puzzle[zero_row][zero_col];			
			eight_puzzle[zero_row][zero_col] = temp;
			
			//update zero position
			zero_row = zero_row - 1;
			last_last_move = last_move;
			last_move = UP;
            backTracing.push_back(UP);
            distTracing.push_back(dist);
            costTracing.push_back(cost);
            ++cost;        
			return true;
		}

	}

	//--------------------------------------//
	//	    EIGHT PUZZLE DOWN FUNCTION
	//--------------------------------------//
	//moves the 0 position down
	bool down()
	{
		//check if valid move
		if((zero_row + 1) > 2)
		{
			return false;
		}
		else
		{
			//swap the top value with the zero position
			int temp = eight_puzzle[zero_row+1][zero_col];
			eight_puzzle[zero_row+1][zero_col] = eight_puzzle[zero_row][zero_col];			
			eight_puzzle[zero_row][zero_col] = temp;
			
			//update zero position
			zero_row = zero_row + 1;
			last_last_move = last_move;
			last_move = DOWN;
            backTracing.push_back(DOWN);
            distTracing.push_back(dist);
            costTracing.push_back(cost);
            ++cost;              
			return true;
		}

	}

	//--------------------------------------//
	//	EIGHT PUZZLE REVERT FUNCTION
	//--------------------------------------//
	//Revert the last move without changing any variables or updating 
	//anything in this class
	void revert()
	{
		//check if valid move
		if(last_move == RIGHT)
		{
		    //move left to fix
			int temp = eight_puzzle[zero_row][zero_col-1];
			eight_puzzle[zero_row][zero_col-1] = eight_puzzle[zero_row][zero_col];			
			eight_puzzle[zero_row][zero_col] = temp;
			
			//revert variable changes
			zero_col = zero_col - 1;
			last_move = last_last_move;
			dist = prevDist;
			a_star_calc = prevAStarCalc;
			backTracing.pop_back();
			distTracing.pop_back();
			costTracing.pop_back();
			--cost;
		}
        else if(last_move == LEFT)
        {
            //move right to fix
			int temp = eight_puzzle[zero_row][zero_col+1];
			eight_puzzle[zero_row][zero_col+1] = eight_puzzle[zero_row][zero_col];			
			eight_puzzle[zero_row][zero_col] = temp;  
			
			//revert variable changes
			zero_col = zero_col + 1;
			last_move = last_last_move;
			dist = prevDist;
			a_star_calc = prevAStarCalc;
			backTracing.pop_back();
			distTracing.pop_back();
			costTracing.pop_back();
			--cost;
        }
        else if(last_move == UP)
        {
            //move down to fix
			int temp = eight_puzzle[zero_row+1][zero_col];
			eight_puzzle[zero_row+1][zero_col] = eight_puzzle[zero_row][zero_col];			
			eight_puzzle[zero_row][zero_col] = temp;     
			
			//revert variable changes
			zero_row = zero_row + 1;
			last_move = last_last_move;
			dist = prevDist;
			a_star_calc = prevAStarCalc;
			backTracing.pop_back();
			distTracing.pop_back();
			costTracing.pop_back();
			--cost;
        }
        else if(last_move == DOWN)
        {
            //move up to fix
			int temp = eight_puzzle[zero_row-1][zero_col];
			eight_puzzle[zero_row-1][zero_col] = eight_puzzle[zero_row][zero_col];			
			eight_puzzle[zero_row][zero_col] = temp;   
			
			//revert variable changes
			zero_row = zero_row - 1;
			last_move = last_last_move;
			dist = prevDist;
			a_star_calc = prevAStarCalc;
			backTracing.pop_back();
			distTracing.pop_back();
			costTracing.pop_back();
			--cost;
        }
        else
        {
            //do nothing
        }
	}

    //=======================================//
    //		Manhattan Distance Calculations
    //=======================================//
    //calculates the h(n)
    void man_dist()
    {
        //declare a variable to hold the manhattan sum
        //as well as the goals
        int h_sum = 0; 
        int number = 0;
        int goalI = 0;
        int goalJ = 0;
        int goalSum = 0;
        
        //iterate through the puzzle
        for (int i = 0; i < 3; ++i)
        { 
            for (int j = 0; j < 3; ++j) 
            { 
            	//get the number at that position
                number = eight_puzzle[i][j]; 
        
                //ignore 0 when you see it
                if (number != 0) 
                { 
                	//calculate the goal state for each number and how far 
                	//it is away
                   	goalI = (number - 1) / 3; 
                    goalJ = (number - 1) % 3; 
                   	goalSum = (abs(i - goalI) + abs(j - goalJ));   
                   	h_sum += goalSum;
                }
            }
        }
        
        //return the calculated distance
        prevDist = dist;
        prevAStarCalc = a_star_calc;
        dist = h_sum;
        a_star_calc = cost + dist;
    }
    
    //=======================================//
    //		Manhattan Distance Calculations RETURN INT
    //=======================================//
    //calculates the h(n)
    int man_dist_re()
    {
        //declare a variable to hold the manhattan sum
        //as well as the goals
        int h_sum = 0; 
        int number = 0;
        int goalI = 0;
        int goalJ = 0;
        int goalSum = 0;
        
        //iterate through the puzzle
        for (int i = 0; i < 3; ++i)
        { 
            for (int j = 0; j < 3; ++j) 
            { 
            	//get the number at that position
                number = eight_puzzle[i][j]; 
        
                //ignore 0 when you see it
                if (number != 0) 
                { 
                	//calculate the goal state for each number and how far 
                	//it is away
                   	goalI = (number - 1) / 3; 
                    goalJ = (number - 1) % 3; 
                   	goalSum = (abs(i - goalI) + abs(j - goalJ));   
                   	h_sum += goalSum;
                }
            }
        }
        
        //return the calculated distance
        prevDist = dist;
        prevAStarCalc = a_star_calc;
        dist = h_sum;
        a_star_calc = cost + dist;
        return a_star_calc;
    }
    
    //=======================================//
    //		Misplaced NODES
    //=======================================//
    //calculates the h(n)
    int mis_calc_re()
    {
        //declare a variable to hold the manhattan sum
        int misplaced = 0; 
        int number = 0;
        int goalI = 0;
        int goalJ = 0;
        
        //iterate through the puzzle
        for (int i = 0; i < 3; ++i)
        { 
            for (int j = 0; j < 3; ++j) 
            { 
            	//get the number at that position
                number = eight_puzzle[i][j]; 
        
                //ignore 0 when you see it
                if (number != 0) 
                { 
                	//calculate the goal state for each number and how far 
                	//it is away
                   	goalI = (number - 1) / 3; 
                    goalJ = (number - 1) % 3;  
                 
                    if(i != goalI && j != goalJ)
                    {
                        ++misplaced;
                    }
                }
            }
        }
        misplacedNodes = misplaced + cost;
        return misplacedNodes;
    }    
    
    //=======================================//
    //		Misplaced NODES
    //=======================================//
    //calculates the h(n)
    int mis_calc()
    {
        //declare a variable to hold the manhattan sum
        int misplaced = 0; 
        int number = 0;
        int goalI = 0;
        int goalJ = 0;
        
        //iterate through the puzzle
        for (int i = 0; i < 3; ++i)
        { 
            for (int j = 0; j < 3; ++j) 
            { 
            	//get the number at that position
                number = eight_puzzle[i][j]; 
        
                //ignore 0 when you see it
                if (number != 0) 
                { 
                	//calculate the goal state for each number and how far 
                	//it is away
                   	goalI = (number - 1) / 3; 
                    goalJ = (number - 1) % 3;  
                 
                    if(i != goalI && j != goalJ)
                    {
                        ++misplaced;
                    }
                }
            }
        }
        misplacedNodes = misplaced + cost;
    } 
    
	//--------------------------------------//
	//		EIGHT PUZZLE RESET FUNCTION
	//--------------------------------------//
	//resets the eight puzzle to 0 values
	void reset()
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				eight_puzzle[i][j] = 0;
			}
		}
	}

    //--------------------------------------------------------//
    //	    EIGHT PUZZLE OUTPUT PUZZLE TO FILE FUNCTION
    //--------------------------------------------------------//
    //output the puzzle to an output file
    void outputByFile(char *file)
    {
		//create an output file to output the trace on and such and open it
		ofstream fout;
	    fout.open("puzzle_solution.txt");
	    if(!fout.is_open())
	    {
	        cout << "Error opening file" << endl;
	        return;
	    }

	    //close the output file
	    fout.close();	
	}

	//---------------------------------------//
	//		EIGHT PUZZLE PRINT FUNCTION
	//---------------------------------------//
	//prints out the eight puzzle in a nicely formated way
	void print()
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				cout << "| " << eight_puzzle[i][j] << " ";
			}
			cout << "|" << endl;
		}
	}
    
};
//===========================================================//

//======================================
//			GLOBALS
//======================================
    int    RIGHT = 1;
    int    LEFT = 2;
    int    UP = 3;
    int    DOWN = 4;

//===============================================================//

//============================================//
//		         BACKTRACING
//============================================//
//print out the backtrace and all of hte final information about the nodes and queue
void backTrace (vector<int> backTracing, vector<int> distTracing, 
vector<int> costTracing, puzzle main_puzzle, int totalNodes, int maxQueueNodes)
{
    cout << "---------------TRACEBACK----------------" << endl;
    cout << "Start Position: " << endl;
    main_puzzle.print();
    cout << endl;
    cout << "BackTrace Size: " << backTracing.size() << endl;
    for(int i = 0; i < backTracing.size(); ++i)
    {
        if(backTracing.at(i) == RIGHT)
        {
            if(main_puzzle.right())
            {
                cout << "Step: " << (i+1) << " Moved Right:" << endl;
                cout << "Dist: " <<  distTracing.at(i) << " Cost: " 
                <<  costTracing.at(i) << " MAN DIST: " 
                << (costTracing.at(i) +  distTracing.at(i)) << endl;
                main_puzzle.print();
                cout << endl;
            }		
        }
        if(backTracing.at(i) == LEFT)
        {
            if(main_puzzle.left())
            {
                cout << "Step: " << (i+1) << " Moved Left:" << endl;
                cout << "Dist: " <<  distTracing.at(i) << " Cost: " 
                <<  costTracing.at(i) << " MAN DIST: "
                << (costTracing.at(i) +  distTracing.at(i)) << endl;
                main_puzzle.print();
                cout << endl;
            }		
        }	
        if(backTracing.at(i) == UP)
        {
            if(main_puzzle.up())
            {
                cout << "Step: " << (i+1) << " Moved Up:" << endl;
                cout << "Dist: " <<  distTracing.at(i) << " Cost: " 
                <<  costTracing.at(i) << " MAN DIST: " 
                << (costTracing.at(i) +  distTracing.at(i)) << endl;
                main_puzzle.print();
                cout << endl;
            }
        }	
        if(backTracing.at(i) == DOWN)
        {
            if(main_puzzle.down())
            {
                cout << "Step: " << (i+1) << " Moved Down:" << endl;
                cout << "Dist: " << distTracing.at(i) << " Cost: " 
                <<  costTracing.at(i) << " MAN DIST: " << (costTracing.at(i) +  distTracing.at(i)) << endl;
                main_puzzle.print();
                cout << endl;
            }
        }	       
    }    
    cout << "----------------------------------------------" << endl;
	cout << "GOAL REACHED!" << endl;
	cout << "Expanded total nodes: " << totalNodes << endl;
	cout << "Maximum nodes in queue: " << maxQueueNodes << endl;
	cout << "Goal node depth: " << main_puzzle.cost << endl;
	cout << "----------------------------------------------" << endl;
}



//------------------------------------------------

//=======================================//
//		   UNIFORM COST SEARCH
//=======================================//
void uniform_cost_search (puzzle main_puzzle, puzzle goal_state)
{
	//initilize "tree and queue" and variables
	queue<puzzle> curr_queue;
	puzzle goaled;
	puzzle popped_puzzle;
	bool is_goal_state = false;
	puzzle temp;
	int maxNodes = 0;
	int maxQueueSize = 0;

	//check if the first state is the goal state
	if(goal_state == main_puzzle)
	{
		is_goal_state = true;
	}

    //push the main puzzle onto the queue and increment max nodes
    curr_queue.push(main_puzzle);
    ++maxNodes;

	//keep running until the queue is either empty or goal state is found
	while(!curr_queue.empty() && !is_goal_state )
	{
		//pop the first element off the stack and set this puzzle to popped puzzle
		popped_puzzle = curr_queue.front();

		//then pop it off
		curr_queue.pop();

		//--------------------CHECK RIGHT--------------------
		if(popped_puzzle.right() && (popped_puzzle.last_move != LEFT) && !is_goal_state)
		{
			//push into the queue and update
			//if shifting it is the goal state then end the process
			if(popped_puzzle == goal_state)
			{
			    curr_queue.push(popped_puzzle);
				is_goal_state = true;
				goaled = popped_puzzle;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
			else
			{
				//push into the queue and update and move it back in place and increment nodes
				curr_queue.push(popped_puzzle);
				popped_puzzle.revert();
				++maxNodes;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
		}		

		//---------------------CHECK LEFT----------------------
		if(popped_puzzle.left() && (popped_puzzle.last_move != RIGHT) && !is_goal_state )
		{
			//push into the queue and update
			//if shifting it is the goal state then end the process
			if(popped_puzzle == goal_state)
			{
			    curr_queue.push(popped_puzzle);
				is_goal_state = true;
				goaled = popped_puzzle;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
				
			}
			else
			{
				//push into the queue and update and move it back in place and increment nodes
				curr_queue.push(popped_puzzle);
				popped_puzzle.revert();
				++maxNodes;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
		}	
		
		//-------------------------CHECK UP------------------
		if(popped_puzzle.up() && (popped_puzzle.last_move != DOWN) && !is_goal_state)
		{
			//push into the queue and update
			//if shifting it is the goal state then end the process
			if(popped_puzzle == goal_state)
			{
			    curr_queue.push(popped_puzzle);
				is_goal_state = true;
				goaled = popped_puzzle;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
			else
			{
				//push into the queue and update and move it back in place and increment nodes
				curr_queue.push(popped_puzzle);
				popped_puzzle.revert();
				++maxNodes;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
		}	

		//----------------------CHECK DOWN-----------------------
		if(popped_puzzle.down() && (popped_puzzle.last_move != UP) && !is_goal_state)
		{
			//push into the queue and update
			//if shifting it is the goal state then end the process
			if(popped_puzzle == goal_state)
			{
			    curr_queue.push(popped_puzzle);
				is_goal_state = true;
				goaled = popped_puzzle;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
			else
			{
				//push into the queue and update and move it back in place and increment nodes
				curr_queue.push(popped_puzzle);
				++maxNodes;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
		}
	}
        
    //print out backtrace after goal is met
    backTrace(goaled.backTracing, goaled.distTracing, goaled.costTracing, main_puzzle, maxNodes, maxQueueSize);
}

//------------------------------------------------

//=======================================//
//        PRIORITY COMPARATOR CLASS
//=======================================//
//comparator made for the priority queue
class ComparatorMis
{
    public: 
    
     bool operator() (puzzle distA, puzzle distB)
     {
         return distA.mis_calc_re() >= distB.mis_calc_re();
     }
};

//=======================================//
//		A* Misplaced Nodes Search
//=======================================//
void a_star_mis_search (puzzle main_puzzle, puzzle goal_state)
{
	//initilize "tree and queue" and variables
	priority_queue<puzzle, vector<puzzle>, ComparatorMis> curr_queue;
	puzzle goaled;
	puzzle popped_puzzle;
	bool is_goal_state = false;
	puzzle temp;
	int maxNodes = 0;
	int maxQueueSize = 0;
	
	int k = 0;

	//check if the first state is the goal state
	if(goal_state == main_puzzle)
	{
		is_goal_state = true;
	}

    //push the main puzzle onto the queue and increment max nodes
    main_puzzle.mis_calc();
    curr_queue.push(main_puzzle);
    ++maxNodes;

	//keep running until the queue is either empty or goal state is found
	while(!curr_queue.empty() && !is_goal_state)
	{
		//pop the first element off the stack and set this puzzle to popped puzzle
		popped_puzzle = curr_queue.top();
		//then pop it off
		curr_queue.pop();

		//--------------------CHECK RIGHT--------------------
		if(popped_puzzle.right() && !is_goal_state)
		{
			//push into the queue and update
			//if shifting it is the goal state then end the process
			if(popped_puzzle == goal_state)
			{
			    popped_puzzle.mis_calc();
			    curr_queue.push(popped_puzzle);
				is_goal_state = true;
				goaled = popped_puzzle;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
			else
			{
				//push into the queue and update and move it back in place and increment nodes
				popped_puzzle.mis_calc();
				curr_queue.push(popped_puzzle);
				popped_puzzle.revert();
				++maxNodes;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
		}		

		//---------------------CHECK LEFT----------------------
		if(popped_puzzle.left() && !is_goal_state )
		{
			//push into the queue and update
			//if shifting it is the goal state then end the process
			if(popped_puzzle == goal_state)
			{
			    popped_puzzle.mis_calc();
			    curr_queue.push(popped_puzzle);
				is_goal_state = true;
				goaled = popped_puzzle;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
				
			}
			else
			{
				//push into the queue and update and move it back in place and increment nodes
				popped_puzzle.mis_calc();
				curr_queue.push(popped_puzzle);
				popped_puzzle.revert();
				++maxNodes;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
		}	
		
		//-------------------------CHECK UP------------------
		if(popped_puzzle.up() && !is_goal_state)
		{
			//push into the queue and update
			//if shifting it is the goal state then end the process
			if(popped_puzzle == goal_state)
			{
			    popped_puzzle.mis_calc();
			    curr_queue.push(popped_puzzle);
				is_goal_state = true;
				goaled = popped_puzzle;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
			else
			{
				//push into the queue and update and move it back in place and increment nodes
				popped_puzzle.mis_calc();
				curr_queue.push(popped_puzzle);
				popped_puzzle.revert();
				++maxNodes;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
		}	

		//----------------------CHECK DOWN-----------------------
		if(popped_puzzle.down() && !is_goal_state)
		{
			//push into the queue and update
			//if shifting it is the goal state then end the process
			if(popped_puzzle == goal_state)
			{
			    popped_puzzle.mis_calc();
			    curr_queue.push(popped_puzzle);
				is_goal_state = true;
				goaled = popped_puzzle;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
			else
			{
				//push into the queue and update and move it back in place and increment nodes
				popped_puzzle.mis_calc();
				curr_queue.push(popped_puzzle);
				++maxNodes;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
		}
		
		//++k;
	}
        
    //print out backtrace after goal is met
    backTrace(goaled.backTracing, goaled.distTracing, goaled.costTracing, main_puzzle, maxNodes, maxQueueSize);
}
 
//=======================================//
//        PRIORITY COMPARATOR CLASS
//=======================================//
//comparator made for the priority queue
class ComparatorMan
{
    public: 
    
     bool operator() (puzzle distA, puzzle distB)
     {
         return distA.man_dist_re() >= distB.man_dist_re();
     }
};

//=======================================//
//		A* Manhattan Distance Search
//=======================================//
void a_star_man_search (puzzle main_puzzle, puzzle goal_state)
{
	//initilize "tree and queue" and variables
	priority_queue<puzzle, vector<puzzle>, ComparatorMan> curr_queue;
	puzzle goaled;
	puzzle popped_puzzle;
	bool is_goal_state = false;
	puzzle temp;
	int maxNodes = 0;
	int maxQueueSize = 0;
	
	int k = 0;

	//check if the first state is the goal state
	if(goal_state == main_puzzle)
	{
		is_goal_state = true;
	}

    //push the main puzzle onto the queue and increment max nodes
    main_puzzle.man_dist();
    curr_queue.push(main_puzzle);
    ++maxNodes;

	//keep running until the queue is either empty or goal state is found
	while(!curr_queue.empty() && !is_goal_state)
	{
		//pop the first element off the stack and set this puzzle to popped puzzle
		popped_puzzle = curr_queue.top();
		//then pop it off
		curr_queue.pop();

		//--------------------CHECK RIGHT--------------------
		if(popped_puzzle.right() && !is_goal_state)
		{
			//push into the queue and update
			//if shifting it is the goal state then end the process
			if(popped_puzzle == goal_state)
			{
			    popped_puzzle.man_dist();
			    curr_queue.push(popped_puzzle);
				is_goal_state = true;
				goaled = popped_puzzle;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
			else
			{
				//push into the queue and update and move it back in place and increment nodes
				popped_puzzle.man_dist();
				curr_queue.push(popped_puzzle);
				popped_puzzle.revert();
				++maxNodes;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
		}		

		//---------------------CHECK LEFT----------------------
		if(popped_puzzle.left() && !is_goal_state )
		{
			//push into the queue and update
			//if shifting it is the goal state then end the process
			if(popped_puzzle == goal_state)
			{
			    popped_puzzle.man_dist();
			    curr_queue.push(popped_puzzle);
				is_goal_state = true;
				goaled = popped_puzzle;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
				
			}
			else
			{
				//push into the queue and update and move it back in place and increment nodes
				popped_puzzle.man_dist();
				curr_queue.push(popped_puzzle);
				popped_puzzle.revert();
				++maxNodes;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
		}	
		
		//-------------------------CHECK UP------------------
		if(popped_puzzle.up() && !is_goal_state)
		{
			//push into the queue and update
			//if shifting it is the goal state then end the process
			if(popped_puzzle == goal_state)
			{
			    popped_puzzle.man_dist();
			    curr_queue.push(popped_puzzle);
				is_goal_state = true;
				goaled = popped_puzzle;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
			else
			{
				//push into the queue and update and move it back in place and increment nodes
				popped_puzzle.man_dist();
				curr_queue.push(popped_puzzle);
				popped_puzzle.revert();
				++maxNodes;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
		}	

		//----------------------CHECK DOWN-----------------------
		if(popped_puzzle.down() && !is_goal_state)
		{
			//push into the queue and update
			//if shifting it is the goal state then end the process
			if(popped_puzzle == goal_state)
			{
			    popped_puzzle.man_dist();
			    curr_queue.push(popped_puzzle);
				is_goal_state = true;
				goaled = popped_puzzle;
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
			else
			{
				//push into the queue and update and move it back in place and increment nodes
				popped_puzzle.man_dist();
				curr_queue.push(popped_puzzle);
				
				
				//if queue size is bigger than current queue size then set new max
				if(curr_queue.size() > maxQueueSize)
				{
				    maxQueueSize = curr_queue.size();
				}
			}
		}
		
		//++k;
	}
        
    //print out backtrace after goal is met
    backTrace(goaled.backTracing, goaled.distTracing, goaled.costTracing, main_puzzle, maxNodes, maxQueueSize);
}

//=======================================//
//				INT MAIN
//=======================================//
int main()
{
	//---------------------------//
	//	puzzle creation
	//---------------------------//
	//initilize variables and the eight puzzle
	puzzle main_puzzle;	
	puzzle goal_puzzle;

	//-----------user input process-------------- 
	cout << "Select (1) User Input or (2) Read By File Puzzle_Start.txt: ";
	int userinput = 0;
	cin >> userinput;
	
	if(userinput == 1)
	{
	    main_puzzle.userInput();
	    goal_puzzle.readByFile("puzzle_goal.txt");
	}
	else if(userinput == 2)
	{
    	main_puzzle.readByFile("puzzle_start.txt");
    	goal_puzzle.readByFile("puzzle_goal.txt");
	}
	//--------------select the algorithm mode------------------
	int mode = 1;

	cout << "Select the algorithm of choice for the eight puzzle: " << endl;
	cout << "1.) Uniform Search" << endl;
	cout << "2.) A* with the Misplaced Tile heuristic." << endl;
	cout << "3.) A* with the Manhattan Distance heuristic." << endl;

	cin >> mode;

	//----------------UNIFORM COST SEARCH-----------------------
	if (mode == 1)
	{
		uniform_cost_search(main_puzzle, goal_puzzle);
	}
	else if (mode == 2)
	{
	    a_star_mis_search(main_puzzle, goal_puzzle);
	}
	else if (mode == 3)
	{
		a_star_man_search(main_puzzle, goal_puzzle);
	}


	return 0;
}
