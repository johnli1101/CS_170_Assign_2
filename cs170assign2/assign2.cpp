#include <iostream>
#include <queue>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

//--------------------------------------------------------//
//              GLOBAL VARIABLES
//--------------------------------------------------------//
vector<vector<double> > featureList;
vector<double> featureListRow;
vector<vector<double> > featureSolution;
vector<double> featureSolutionRow;
vector<double> featureSolutionAccuracy;

//--------------------------------------------------------//

//--------------------------------------------------------//
//          Distance Struct with accuracy
//--------------------------------------------------------//
//for nearest neighbor purposes
struct DistanceStruct
{
    double distance;
    double class_of_thing;
};

class MyAlgStruct
{
    public:
    int index;
    double accuracy;
    
    MyAlgStruct()
    {
        accuracy = 0; index = 0;
    }
    
    MyAlgStruct& operator= (const MyAlgStruct p)
	{
        accuracy = p.accuracy;
        index = p.index;
    }
};
//--------------------------------------------------------//
//	         read feature by file and normalize
//--------------------------------------------------------//
void readByFile(string file)
{
    //set up the fstream and open the file
    ifstream fin;
    double feature = 0.0;
    unsigned i = 0; 
    unsigned j = 0;
    double sum = 0;
    
    //open file and check if openned
    fin.open(file.c_str());
    if(!fin.is_open())
    {
        cout << "Error opening file" << endl;
        return;
    }

    //read in first one
    fin >> feature;
    featureList.push_back(featureListRow);
    featureList.at(0).push_back(feature);

    //read in the puzzle from the file and set it up
    while(fin >> feature)
    {
        //if the feature is a class then add a new row
        if(feature == 1.0000000000 || feature == 2.0000000000)
        {
            featureList.push_back(featureListRow);
            ++i;
        }
        
        //push back the feature and compute the sum for the mean 
        //sum += feature;
        featureList.at(i).push_back(feature);
    }		

    //close the file
    fin.close();
}

//--------------------------------------------------------//
//                  NORMALIZE DATA
//--------------------------------------------------------//
void normalizeData(int featureColumn)
{
    double sum = 0.0;
    double mean = 0.0;
    double stanDev = 0.0;
    
    //this is to iterate for each instance
    for(int i = 0; i < featureList.size(); ++i)
    {
        sum += featureList.at(i).at(featureColumn);
    }
    
    mean = sum / (double)featureList.size();
    sum = 0;
    
    //compute standard deviation
    for(int i = 0; i < featureList.size(); ++i)
    {
        sum += pow((featureList.at(i).at(featureColumn) - mean), 2);
    }
    
    stanDev = sum/(double)featureList.size();
    
    //apply the normalization formula to everything now
    for(int i = 0; i < featureList.size(); ++i)
    {
        featureList.at(i).at(featureColumn) = (featureList.at(i).at(featureColumn) - mean)/stanDev;
    }
}

//--------------------------------------------------------//
//          SORT VECTOR IN ASCENDING ORDER
//--------------------------------------------------------//
void sortVector(vector<int> toSort)
{
    int temp;
    for(unsigned i = 0; i < toSort.size()-1; ++i)
    {
        if(toSort.at(i) > toSort.at(i+1))
        {
            temp = toSort.at(i+1);
            toSort.at(i+1) = toSort.at(i);
            toSort.at(i) = temp;
        }
    }
}

//=======================================//
//        PRIORITY COMPARATOR CLASS
//=======================================//
//comparator made for the priority queue
class DistanceCom
{
    public: 
    
     bool operator() (DistanceStruct a, DistanceStruct b)
     {
         return a.distance > b.distance;
     }
};

class AccuracyCom
{
    public: 
    
     bool operator() (MyAlgStruct a, MyAlgStruct b)
     {
         return a.accuracy < b.accuracy;
     }
};

//--------------------------------------------------------//
//           NEAREST NEIGHBOR K = 1
//--------------------------------------------------------//
double leave_one_out_NN(vector<int> featureToCheck)
{
    //run through all the rows of the feature list
    vector<double> distances;
    DistanceStruct top1;
    priority_queue<DistanceStruct, vector<DistanceStruct>, DistanceCom> order_distance_queue;
    double sum = 0;
    double difference = 0;
    double distance = 0;
    int accuracy_counter = 0;
    DistanceStruct toQueue;
    int isOne = 0;
    int isTwo = 0;
    double finalClass = 0;
    double currentClass = 0;
    double finalAccuracy = 0;
    double lowest_distance = 10000;
    
    //the outer for loop is the one doing the checking
    for(int i = 0; i < featureList.size(); ++i)
    {
        //assign the current class
        currentClass = featureList.at(i).at(0);
        
        //this is what were checking i with 
        for(int j = 0; j < featureList.size(); ++j)
        {
            if(j != i)
            {
                for(int m = 0; m < featureToCheck.size(); ++m)
                {
                    //subtract j feature with i feature and sum them up
                    difference = featureList.at(j).at(featureToCheck.at(m)) - 
                        featureList.at(i).at(featureToCheck.at(m));
                    sum += pow(difference, 2);
                }
                
                //compute the distance
                distance = sqrt(sum);
                
                //store in the struct to push onto the queue along with the class
                if(distance < lowest_distance)
                {
                    lowest_distance = distance;
                    toQueue.distance = distance;
                    toQueue.class_of_thing = featureList.at(j).at(0);
                }
                //store it into a priority queue of distances - TODO
                //order_distance_queue.push(toQueue);
                
                //reset the sum and difference variable afterwards
                sum = 0;
                difference = 0;
                distance = 0;
            }
        }
        //grab the top 3 in the priority queue and then check their classes
        //and do the voting to see which majority is in the 3
        finalClass = toQueue.class_of_thing;

        //if their class are the same then add 1 to the counter
        if(finalClass == currentClass)
        {
            ++accuracy_counter;
        }
                
        //if not then dont do anything
        //reset the counter for ones
        currentClass = 0;
        finalClass = 0;
        lowest_distance = 10000;
    }
    
    //now compute the average and find out the accuracy of this data set
    finalAccuracy = accuracy_counter / ((double)(featureList.size()));
    accuracy_counter = 0;
    
    return finalAccuracy;
}

//--------------------------------------------------------//
//           NEAREST NEIGHBOR MY ALGORITHM
//--------------------------------------------------------//
//ONLY CHECKING first 10 DATA SETS FOR MA
double leave_one_out_MA(vector<int> featureToCheck, int amountChecking)
{
    //run through all the rows of the feature list
    vector<double> distances;
    DistanceStruct top1;
    double sum = 0;
    double difference = 0;
    double distance = 0;
    int accuracy_counter = 0;
    DistanceStruct toQueue;
    int isOne = 0;
    int isTwo = 0;
    double finalClass = 0;
    double currentClass = 0;
    double finalAccuracy = 0;
    double lowest_distance = 10000;
    
    //the outer for loop is the one doing the checking
    for(int i = 0; i < amountChecking; ++i)
    {
        //assign the current class
        currentClass = featureList.at(i).at(0);
        
        //this is what were checking i with 
        for(int j = 0; j < amountChecking; ++j)
        {
            if(j != i)
            {
                for(int m = 0; m < featureToCheck.size(); ++m)
                {
                    //subtract j feature with i feature and sum them up
                    difference = featureList.at(j).at(featureToCheck.at(m)) - 
                        featureList.at(i).at(featureToCheck.at(m));
                    sum += pow(difference, 2);
                }
                
                //compute the distance
                distance = sqrt(sum);
                
                //store in the struct to push onto the queue along with the class
                if(distance < lowest_distance)
                {
                    lowest_distance = distance;
                    toQueue.distance = distance;
                    toQueue.class_of_thing = featureList.at(j).at(0);
                }
                //store it into a priority queue of distances - TODO
                //order_distance_queue.push(toQueue);
                
                //reset the sum and difference variable afterwards
                sum = 0;
                difference = 0;
                distance = 0;
            }
        }
        //grab the top 3 in the priority queue and then check their classes
        //and do the voting to see which majority is in the 3
        finalClass = toQueue.class_of_thing;

        //if their class are the same then add 1 to the counter
        if(finalClass == currentClass)
        {
            ++accuracy_counter;
        }
                
        //if not then dont do anything
        //reset the counter for ones
        currentClass = 0;
        finalClass = 0;
        lowest_distance = 10000;
    }
    
    //now compute the average and find out the accuracy of this data set
    finalAccuracy = accuracy_counter / ((double)(amountChecking));
    accuracy_counter = 0;
    
    return finalAccuracy;
}

//--------------------------------------------------------//
//            CHECK IF VALUE IS IN THE VECTOR
//--------------------------------------------------------//
bool notInVector(vector<int> checkVector, int value)
{
    for(int i = 0; i < checkVector.size(); ++i)
    {
        if(checkVector.at(i) == value)
        {
            return false;
        }
    }
    
    return true;
}

//--------------------------------------------------------//
//                  BACKWARD ELIMATION
//--------------------------------------------------------//
void backwardElimination(int numOfFeatures)
{
    //run a for loop and apply nearest neightbor to all points
    vector<int> featureToCheck;
    vector<int> currentFeatureSolutionToRemove;
    vector<int> featureRemoved;
    int featureToCheckI = 0;
    int currentNumOfFeatures = 0;
    int finalIndex = 0;
    int solutionIndex = 0;
    int lastIndex = 0;
    int toRemoveIndex = 0;
    bool isDone = false;
    double removedElement = 0;
    double finalAccuracy = 0;
    double accuracy = 0;
    double newAccuracy = 0;
    currentFeatureSolutionToRemove.push_back(0);
    
    //fill in the features to check with all the number of features
    for(int i = 0; i < numOfFeatures; ++i)
    {
        featureToCheck.push_back(i+1);
    }
    
    int STOPTHISMADNESS = 0;
    //while the featureToCheck vector is not left with 1 element
    while(featureToCheck.size() > 0 && !isDone)
    {
        //run through all the current features
        for(int i = 0; i < numOfFeatures; ++i)
        {
            if(notInVector(currentFeatureSolutionToRemove, i+1))
            {
                //remove this feature and apply nearest neighbor to it
                //update accuracy after applying nearest neighbor
                //find the index of the feature to be removed

                for(int k = 0; k < featureToCheck.size(); ++k)
                {
                    if(featureToCheck.at(k) == i+1)
                    {
                        toRemoveIndex = k;
                    }
                }
                
                //swap feature to the end of the vector and pop back
                for(int k = toRemoveIndex; k < featureToCheck.size()-1; ++k)
                {
                    int temp = featureToCheck.at(k+1);
                    featureToCheck.at(k+1) = featureToCheck.at(k);
                    featureToCheck.at(k) = temp;
                }    
                featureToCheck.pop_back();
                
                newAccuracy = leave_one_out_NN(featureToCheck);
                
                //print out message
                cout << "Using features(s) {"; 
                for(int k = 0; k < featureToCheck.size(); ++k)
                {
                    if(k == featureToCheck.size()-1)
                        cout << featureToCheck.at(k);
                    else
                        cout << featureToCheck.at(k) << ", ";
                }
                cout << "} accuracy is " << newAccuracy * 100 << "%" << endl;
                
                //if the solution accuracy is is less than new accuracy then update
                if(accuracy < newAccuracy)
                {
                    accuracy = newAccuracy;
                    currentFeatureSolutionToRemove.at(featureToCheckI) = i+1;
                }

                //put it back in and test the new one
                featureToCheck.push_back(i+1);
                for(int k = toRemoveIndex-1; k > 1; --k)
                {
                    int temp = featureToCheck.at(k-1);
                    featureToCheck.at(k-1) = featureToCheck.at(k);
                    featureToCheck.at(k) = temp; 
                }
            }
        }
        
        //push stuff into the solution vectors
        featureSolution.push_back(featureSolutionRow);
        featureSolutionAccuracy.push_back(accuracy);
        
        //remove from feature to check for next iteration
        for(int f = 0; f < featureToCheck.size(); ++f)
        {
            if(featureToCheck.at(f) == currentFeatureSolutionToRemove.at(featureToCheckI))
            {
                //swap feature to the end of the vector and pop back
                for(int k = f; k < featureToCheck.size()-1; ++k)
                {
                    int temp = featureToCheck.at(k+1);
                    featureToCheck.at(k+1) = featureToCheck.at(k);
                    featureToCheck.at(k) = temp;
                }    
                featureToCheck.pop_back();
            }
        }
        
        //push in solution index
        for(int c = 0; c < featureToCheck.size(); ++c)
        {
            featureSolution.at(solutionIndex).push_back(featureToCheck.at(c));
        }
        
        //print out the solution vector
        cout << endl;
        cout << "Feature set {";
        for(int b = 0; b < featureSolution.at(solutionIndex).size(); ++b)
        {
            if(b == featureSolution.at(solutionIndex).size()-1)
            {
                cout << featureSolution.at(solutionIndex).at(b);
            }
            else
            {
                cout << featureSolution.at(solutionIndex).at(b) << ", ";
            }
        }    
        cout << "} was best, accuracy is " << (featureSolutionAccuracy.at(solutionIndex) *100) << "%" << endl;
        cout << endl;
        
        //decrement the number of features to check and increment solution index
        ++solutionIndex;
        
        //increment featuretocheckI to show that we are now checking more columns
        currentFeatureSolutionToRemove.push_back(0);
        ++featureToCheckI;
        accuracy = 0;
        newAccuracy = 0;
        
        //check if accuracy is worst if it is then stop 
        if(featureSolutionAccuracy.size() > 1)
        {
            if(featureSolutionAccuracy.at(solutionIndex-1) < featureSolutionAccuracy.at(solutionIndex-2))
            {
                isDone = true;
            }
        }
    }
    
    for(int e = 0; e < featureSolutionAccuracy.size(); ++e)
    {
        if(finalAccuracy < featureSolutionAccuracy.at(e))
        {
            finalAccuracy = featureSolutionAccuracy.at(e);
            finalIndex = e;
        }
    }
    
    //print out final results
    cout << "Finished search!! The best feature subset is {";
    for(int e = 0; e < featureSolution.at(finalIndex).size(); ++e)
    {
        if(e == featureSolution.at(finalIndex).size()-1)
            cout << featureSolution.at(finalIndex).at(e);
        else
            cout << featureSolution.at(finalIndex).at(e) << ", ";
    }
    cout << "}, which has an accuracy of " << (featureSolutionAccuracy.at(finalIndex) * 100) << "%" << endl;
}

//--------------------------------------------------------//
//          NOT AS MINE ALGORITHM FEAT. EAMONN
//--------------------------------------------------------//
void notAsMineAlgorithm(int numOfFeatures)
{
    //run a for loop and apply nearest neightbor to all points
    vector<int> featureToCheck;
    vector<int> currentFeatureSolution;
    priority_queue<MyAlgStruct, vector<MyAlgStruct>, AccuracyCom> top3_queue;
    MyAlgStruct toQueue;
    int featureToCheckI = 0;
    int currentNumOfFeatures = 0;
    int finalIndex = 0;
    int solutionIndex = 0;
    double finalAccuracy = 0;
    double accuracy = 0;
    double newAccuracy = 0;
    int passIntoMA = sqrt(featureList.size());
    MyAlgStruct top3FeatureCheck;
    bool isDone = false;
    featureToCheck.push_back(0);    //push back the first feature to check for
    currentFeatureSolution.push_back(0);
    //set up solution vector and set default accuracy to 0
    
    //shuffle the vectors
    random_shuffle(featureList.begin(), featureList.end());
    
    //while the featureToCheck vector is not the num of total features
    while(featureToCheck.size() < numOfFeatures+1 && !isDone)
    {
        //featureToCheck.at(featureToCheckJ)
        //run through all the current features
        for(int i = 0; i < numOfFeatures; ++i)
        {
            //check if the feature is already added
            if(notInVector(featureToCheck, i+1))
            {
                //add in feature to the vector and apply nearest neighbor
                //update accuracy after applying nearest neighbor
                featureToCheck.at(featureToCheckI) = i+1; 
                newAccuracy = leave_one_out_MA(featureToCheck, passIntoMA);
                
                //now store it into a priority queue of accuracys
                toQueue.accuracy = newAccuracy;
                toQueue.index = i+1;
                top3_queue.push(toQueue);
                
                //print out message
                cout << "Using features(s) {"; 
                for(int k = 0; k < featureToCheck.size(); ++k)
                {
                    if(k == featureToCheck.size()-1)
                        cout << featureToCheck.at(k);
                    else
                        cout << featureToCheck.at(k) << ", ";
                }
                cout << "} accuracy is " << newAccuracy * 100 << "%" << endl;
            }
        }
        
        //Grab the top 3 priorities and run them again with nn and compare it to the entire list
        cout << "Checking top 3 features with entire data set: ";
        for(int c = 0; c < 3; ++c)
        {
            if(c < top3_queue.size())
            {
                top3FeatureCheck = top3_queue.top();
                top3_queue.pop();
                featureToCheck.at(featureToCheckI) = top3FeatureCheck.index;
                
                if(c == top3_queue.size()-1 || c == 2)
                {
                    cout << top3FeatureCheck.index;
                }
                else
                {
                    cout << top3FeatureCheck.index << ", ";
                }
                
                //run NN on entire set and grab final accuracies
                newAccuracy = leave_one_out_NN(featureToCheck);
                if(accuracy < newAccuracy)
                {
                    accuracy = newAccuracy;
                    currentFeatureSolution.at(featureToCheckI) = top3FeatureCheck.index;
                }
            }
        }
        
        cout << endl;
        
        //push stuff into the solution vectors
        featureSolution.push_back(featureSolutionRow);
        featureSolutionAccuracy.push_back(accuracy);
        featureToCheck.at(featureToCheckI) = currentFeatureSolution.at(featureToCheckI);
        
        for(int c = 0; c < featureToCheck.size(); ++c)
        {
            featureSolution.at(solutionIndex).push_back(featureToCheck.at(c));
        }
        
        //print out the solution vector
        cout << endl;
        cout << "Feature set {";
        for(int b = 0; b < featureSolution.at(solutionIndex).size(); ++b)
        {
            if(b == featureSolution.at(solutionIndex).size()-1)
            {
                cout << featureSolution.at(solutionIndex).at(b);
            }
            else
            {
                cout << featureSolution.at(solutionIndex).at(b) << ", ";
            }
        }    
        cout << "} was best, accuracy is " << (featureSolutionAccuracy.at(solutionIndex) *100) << "%" << endl;
        cout << endl;
       
        if(featureSolutionAccuracy.size() > 1)
        {
            if(featureSolutionAccuracy.at(solutionIndex) < featureSolutionAccuracy.at(solutionIndex-1))
            {
                isDone = true;
            }
        }
        
        //decrement the number of features to check and increment solution index
        ++solutionIndex;
        
        //increment featuretocheckI to show that we are now checking more columns
        featureToCheck.push_back(0);
        currentFeatureSolution.push_back(0);
        ++featureToCheckI;
        accuracy = 0;
        newAccuracy = 0;
        
        //empty out priority queue and start over
        for(int i = 0; i < top3_queue.size(); ++i)
        {
            top3_queue.pop();
        }
        
    }
    
    for(int e = 0; e < featureSolutionAccuracy.size(); ++e)
    {
        if(finalAccuracy < featureSolutionAccuracy.at(e))
        {
            finalAccuracy = featureSolutionAccuracy.at(e);
            finalIndex = e;
        }
    }
    
    //print out final results
    cout << "Finished search!! The best feature subset is {";
    for(int e = 0; e < featureSolution.at(finalIndex).size(); ++e)
    {
        if(e == featureSolution.at(finalIndex).size()-1)
            cout << featureSolution.at(finalIndex).at(e);
        else
            cout << featureSolution.at(finalIndex).at(e) << ", ";
    }
    cout << "}, which has an accuracy of " << (featureSolutionAccuracy.at(finalIndex) * 100) << "%" << endl;
}

//--------------------------------------------------------//
//                  FORWARD SELECTION
//--------------------------------------------------------//
void forwardSelection(int numOfFeatures)
{
    //run a for loop and apply nearest neightbor to all points
    vector<int> featureToCheck;
    vector<int> currentFeatureSolution;
    int featureToCheckI = 0;
    int currentNumOfFeatures = 0;
    int finalIndex = 0;
    int solutionIndex = 0;
    bool isDone = false;
    double finalAccuracy = 0;
    double accuracy = 0;
    double newAccuracy = 0;
    featureToCheck.push_back(0);    //push back the first feature to check for
    currentFeatureSolution.push_back(0);
    //set up solution vector and set default accuracy to 0

    
    //while the featureToCheck vector is not the num of total features
    while(featureToCheck.size() < numOfFeatures+1 && !isDone)
    {
        //featureToCheck.at(featureToCheckJ)
        //run through all the current features
        for(int i = 0; i < numOfFeatures; ++i)
        {
            //check if the feature is already added
            if(notInVector(featureToCheck, i+1))
            {
                //add in feature to the vector and apply nearest neighbor
                //update accuracy after applying nearest neighbor
                featureToCheck.at(featureToCheckI) = i+1; 
                newAccuracy = leave_one_out_NN(featureToCheck);
                
                //print out message
                cout << "Using features(s) {"; 
                for(int k = 0; k < featureToCheck.size(); ++k)
                {
                    if(k == featureToCheck.size()-1)
                        cout << featureToCheck.at(k);
                    else
                        cout << featureToCheck.at(k) << ", ";
                }
                cout << "} accuracy is " << newAccuracy * 100 << "%" << endl;
                
                //if the solution accuracy is is less than new accuracy then update
                if(accuracy < newAccuracy)
                {
                    accuracy = newAccuracy;
                    currentFeatureSolution.at(featureToCheckI) = i+1;
                }
            }
        }
        
        //push stuff into the solution vectors
        featureSolution.push_back(featureSolutionRow);
        featureSolutionAccuracy.push_back(accuracy);
        featureToCheck.at(featureToCheckI) = currentFeatureSolution.at(featureToCheckI);
        
        for(int c = 0; c < featureToCheck.size(); ++c)
        {
            featureSolution.at(solutionIndex).push_back(featureToCheck.at(c));
        }
        
        //print out the solution vector
        cout << endl;
        cout << "Feature set {";
        for(int b = 0; b < featureSolution.at(solutionIndex).size(); ++b)
        {
            if(b == featureSolution.at(solutionIndex).size()-1)
            {
                cout << featureSolution.at(solutionIndex).at(b);
            }
            else
            {
                cout << featureSolution.at(solutionIndex).at(b) << ", ";
            }
        }    
        cout << "} was best, accuracy is " << (featureSolutionAccuracy.at(solutionIndex) *100) << "%" << endl;
        cout << endl;
        
        //decrement the number of features to check and increment solution index
        ++solutionIndex;
        
        //increment featuretocheckI to show that we are now checking more columns
        featureToCheck.push_back(0);
        currentFeatureSolution.push_back(0);
        ++featureToCheckI;
        accuracy = 0;
        newAccuracy = 0;
        
        if(featureSolutionAccuracy.size() > 1)
        {
            if(featureSolutionAccuracy.at(solutionIndex-1) < featureSolutionAccuracy.at(solutionIndex-2))
            {
                isDone = true;
            }
        }
    }
    
    for(int e = 0; e < featureSolutionAccuracy.size(); ++e)
    {
        if(finalAccuracy < featureSolutionAccuracy.at(e))
        {
            finalAccuracy = featureSolutionAccuracy.at(e);
            finalIndex = e;
        }
    }
    
    //print out final results
    cout << "Finished search!! The best feature subset is {";
    for(int e = 0; e < featureSolution.at(finalIndex).size(); ++e)
    {
        if(e == featureSolution.at(finalIndex).size()-1)
            cout << featureSolution.at(finalIndex).at(e);
        else
            cout << featureSolution.at(finalIndex).at(e) << ", ";
    }
    cout << "}, which has an accuracy of " << (featureSolutionAccuracy.at(finalIndex) * 100) << "%" << endl;
}

//--------------------------------------------------------//
//	            print list (Just for debugging)
//--------------------------------------------------------//
void printList()
{
    for(unsigned i = 0; i < featureList.size(); ++i)
    {
        for(unsigned j = 0; j < featureList.at(i).size(); ++j)
        {
            cout << featureList.at(i).at(j) << " ";
        }
        cout << endl;
    }
}

//-------------------------MAIN---------------------------//
int main(int argc, char* argv[])
{
    //Ask user for what algorithm to run
    string userInput;
    int userInput2;
    vector<int> tempFeatures;
    
    //Ask user what file they want to read in
    cout << "Read in default file small80.txt (1) or read in your own file(2)? ";
    cin >> userInput2;
    
    if(userInput2 == 1)
    {
        readByFile("cs_170_small80.txt");
    }
    else
    {
        cout << "What file do you want to read in?" << endl;
        cin >> userInput;
        readByFile(userInput);
    }
    
    //do forward selection
    cout << "Type the number of the algorithm you want to run." << endl;
    cout << "1.) Foward Selection" << endl;
    cout << "2.) Backward Elmination" << endl;
    cout << "3.) John Li's 'Algorithm'" << endl;
    cin >> userInput2;
    cout << endl;
    
    //give user info on the dataset they entered in
    cout << "This dataset has " << featureList.at(0).size()-1
    << " features (not including the class attribute), with "
    << featureList.size() << " instances." << endl;
    cout << endl;
    
    cout << "Please wait while I normlize everything... ";
    
    //normalize everything
    for(int i = 0; i < featureList.at(0).size()-1; ++i)
    { 
        normalizeData(i+1);
    }
    cout << "Done." << endl;
    cout << endl;
    
    //find out what the accuracy is when running on all the features and print it out
    for(int i = 0; i < featureList.at(0).size()- 1; ++i)
    {
        tempFeatures.push_back(i+1);
        
    }
    
    cout << "Running nearest algorithm with all " << featureList.at(0).size()-1
        << " features, using “leaving-one-out” evaluation, I get an accuracy of "
        << (leave_one_out_NN(tempFeatures))*100 << "%" << endl;
    cout << endl;
    cout << "Beginning Search..." << endl;
    
    if(userInput2 == 1)
    {
        forwardSelection(featureList.at(0).size()-1);
    }
    else if(userInput2 == 2)
    {
        backwardElimination(featureList.at(0).size()-1);        
    }
    else if(userInput2 == 3)
    {
        notAsMineAlgorithm(featureList.at(0).size()-1);
    }
    else
    {
        cout << "Invalid Input." << endl;
    }
    
    return 0;
}
