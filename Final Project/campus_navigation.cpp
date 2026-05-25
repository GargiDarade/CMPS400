// Compile: g++ campus_navigation.cpp -o campus_navigation.exe

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <iomanip>
#include <limits>
#include <cmath>
#include <algorithm>
#include <conio.h>

using namespace std;

// =====================================================
// STRUCTURES
// =====================================================

struct Edge
{
    string destination;
    int distance;
    int time;
};

struct Node
{
    string id;
    string name;
    double x;
    double y;
};

// =====================================================
// GLOBAL VARIABLES
// =====================================================

map<string, Node> campusNodes;
map<string, vector<Edge>> graph;

// =====================================================
// LOAD XML DATA INTO MEMORY
// =====================================================

void loadCampusData()
{
    // Nodes

    campusNodes["LIB"] = {"LIB", "Wilson Library", 1, 5};
    campusNodes["FH"]  = {"FH", "Founders Hall", 3, 5};
    campusNodes["HB"]  = {"HB", "Hoover Building", 1, 3};
    campusNodes["ACC"] = {"ACC", "Abraham Campus Center", 3, 3};
    campusNodes["CH"]  = {"CH", "Citrus Hall", 3, 1};
    campusNodes["VLV"] = {"VLV", "Vista La Verne", 5, 1};
    campusNodes["SAP"] = {"SAP", "Sports Science & Athletics Pavilion", 0, 0};
    campusNodes["PG"]  = {"PG", "Parking Garage", 5, 3};
    campusNodes["G1"]  = {"G1", "Main Gate", 6, 5};

    // Graph Edges (Undirected)

    graph["LIB"].push_back({"FH", 120, 2});
    graph["FH"].push_back({"LIB", 120, 2});

    graph["LIB"].push_back({"HB", 80, 1});
    graph["HB"].push_back({"LIB", 80, 1});

    graph["FH"].push_back({"ACC", 150, 3});
    graph["ACC"].push_back({"FH", 150, 3});

    graph["FH"].push_back({"G1", 140, 2});
    graph["G1"].push_back({"FH", 140, 2});

    graph["HB"].push_back({"ACC", 100, 2});
    graph["ACC"].push_back({"HB", 100, 2});

    graph["HB"].push_back({"SAP", 170, 3});
    graph["SAP"].push_back({"HB", 170, 3});

    graph["ACC"].push_back({"CH", 90, 2});
    graph["CH"].push_back({"ACC", 90, 2});

    graph["ACC"].push_back({"PG", 110, 2});
    graph["PG"].push_back({"ACC", 110, 2});

    graph["CH"].push_back({"VLV", 130, 3});
    graph["VLV"].push_back({"CH", 130, 3});

    graph["VLV"].push_back({"PG", 160, 3});
    graph["PG"].push_back({"VLV", 160, 3});

    graph["SAP"].push_back({"G1", 200, 4});
    graph["G1"].push_back({"SAP", 200, 4});

    graph["PG"].push_back({"G1", 180, 4});
    graph["G1"].push_back({"PG", 180, 4});
}

// =====================================================
// PRESS ANY KEY
// =====================================================

void waitForKey()
{
    cout << "\nPress any key to return to Main Menu!\n";
    getch();
}

// =====================================================
// OPTION 1 - CAMPUS VISUALIZATION
// =====================================================

void displayCampusGraph()
{
    //system("cls");

    cout << "=========== CAMPUS GRAPH VISUALIZATION ===========" << endl;
    cout << endl;

    cout << "Wilson Library ----120m---- Founders Hall ----140m---- Main Gate" << endl;
    cout << "       |                          |" << endl;
    cout << "      80m                       150m" << endl;
    cout << "       |                          |" << endl;
    cout << "Hoover Building ----100m---- Abraham Campus Center ----110m---- Parking Garage" << endl;
    cout << "       |                          |                               |" << endl;
    cout << "     170m                        90m                            160m" << endl;
    cout << "       |                          |                               |" << endl;
    cout << "Sports Pavilion             Citrus Hall ----130m---- Vista La Verne" << endl;

    waitForKey();
}

// =====================================================
// OPTION 2 - LIST CAMPUS NODES
// =====================================================

void listCampusNodes()
{
    //system("cls");

    cout << "================ CAMPUS NODES ================" << endl;
    cout << endl;

    cout << left << setw(10) << "Node ID"
         << setw(45) << "Building" << endl;

    cout << "--------------------------------------------------------" << endl;

    for (auto node : campusNodes)
    {
        cout << left << setw(10) << node.second.id
             << setw(45) << node.second.name << endl;
    }

    waitForKey();
}

// =====================================================
// DISPLAY PATH
// =====================================================

void displayPath(map<string, string>& previous,
                 string start,
                 string destination)
{
    vector<string> path;

    string current = destination;

    while (current != "")
    {
        path.push_back(current);
        current = previous[current];
    }

    reverse(path.begin(), path.end());

    cout << "\nRoute Graph:\n\n";

    for (int i = 0; i < path.size(); i++)
    {
        cout << campusNodes[path[i]].name;

        if (i != path.size() - 1)
            cout << " --> ";
    }

    cout << endl;
}

// =====================================================
// VALID NODE INPUT
// =====================================================

string getValidNode(string message)
{
    string nodeId;

    while (true)
    {
        cout << message;
        cin >> nodeId;

        if (campusNodes.find(nodeId) != campusNodes.end())
        {
            return nodeId;
        }
        else
        {
            cout << "\nERROR: Invalid Node ID!" << endl;
            cout << "Please enter a valid Node ID.\n" << endl;
        }
    }
}

// =====================================================
// OPTION 3 - DIJKSTRA ALGORITHM
// =====================================================

void dijkstra(string start, string destination)
{
    map<string, int> distance;
    map<string, int> travelTime;
    map<string, string> previous;

    for (auto node : campusNodes)
    {
        distance[node.first] = numeric_limits<int>::max();
        travelTime[node.first] = numeric_limits<int>::max();
        previous[node.first] = "";
    }

    distance[start] = 0;
    travelTime[start] = 0;

    priority_queue<pair<int, string>,
                   vector<pair<int, string>>,
                   greater<pair<int, string>>> pq;

    pq.push({0, start});

    while (!pq.empty())
    {
        string current = pq.top().second;
        int currentDistance = pq.top().first;

        pq.pop();

        for (auto edge : graph[current])
        {
            int newDistance = currentDistance + edge.distance;

            if (newDistance < distance[edge.destination])
            {
                distance[edge.destination] = newDistance;

                travelTime[edge.destination] =
                    travelTime[current] + edge.time;

                previous[edge.destination] = current;

                pq.push({newDistance, edge.destination});
            }
        }
    }

    cout << "\n=========== DIJKSTRA SHORTEST PATH ===========" << endl;

    cout << "\nStart Node       : "
         << campusNodes[start].name << endl;

    cout << "Destination Node : "
         << campusNodes[destination].name << endl;

    displayPath(previous, start, destination);

    cout << "\nShortest Distance : "
         << distance[destination] << " meters" << endl;

    cout << "Estimated Travel Time : "
         << travelTime[destination] << " minutes" << endl;
}

// =====================================================
// HEURISTIC FUNCTION FOR A*
// =====================================================

double heuristic(string current, string destination)
{
    double dx =
        campusNodes[current].x -
        campusNodes[destination].x;

    double dy =
        campusNodes[current].y -
        campusNodes[destination].y;

    return sqrt(dx * dx + dy * dy);
}

// =====================================================
// OPTION 4 - A* SEARCH ALGORITHM
// =====================================================

void aStar(string start, string destination)
{
    map<string, double> gCost;
    map<string, double> fCost;
    map<string, int> travelTime;
    map<string, string> previous;

    for (auto node : campusNodes)
    {
        gCost[node.first] = numeric_limits<double>::max();
        fCost[node.first] = numeric_limits<double>::max();
        travelTime[node.first] = numeric_limits<int>::max();
        previous[node.first] = "";
    }

    gCost[start] = 0;
    fCost[start] = heuristic(start, destination);
    travelTime[start] = 0;

    priority_queue<pair<double, string>,
                   vector<pair<double, string>>,
                   greater<pair<double, string>>> openSet;

    openSet.push({fCost[start], start});

    while (!openSet.empty())
    {
        string current = openSet.top().second;
        openSet.pop();

        if (current == destination)
            break;

        for (auto edge : graph[current])
        {
            double tentativeG =
                gCost[current] + edge.distance;

            if (tentativeG < gCost[edge.destination])
            {
                previous[edge.destination] = current;

                gCost[edge.destination] = tentativeG;

                fCost[edge.destination] =
                    tentativeG +
                    heuristic(edge.destination, destination);

                travelTime[edge.destination] =
                    travelTime[current] + edge.time;

                openSet.push(
                    {fCost[edge.destination],
                     edge.destination});
            }
        }
    }

    cout << "\n=========== A* SEARCH RESULT ===========" << endl;

    cout << "\nStart Node       : "
         << campusNodes[start].name << endl;

    cout << "Destination Node : "
         << campusNodes[destination].name << endl;

    displayPath(previous, start, destination);

    cout << "\nEstimated Distance : "
         << (int)gCost[destination]
         << " meters" << endl;

    cout << "Estimated Travel Time : "
         << travelTime[destination]
         << " minutes" << endl;
}

// =====================================================
// DIJKSTRA MENU
// =====================================================

void dijkstraMenu()
{
    //system("cls");

    cout << "=========== DIJKSTRA ALGORITHM ===========" << endl;
    cout << endl;

    string start =
        getValidNode("Enter Start Node ID       : ");

    string destination =
        getValidNode("Enter Destination Node ID : ");

    dijkstra(start, destination);

    waitForKey();
}

// =====================================================
// A* MENU
// =====================================================

void aStarMenu()
{
    //system("cls");

    cout << "=========== A* SEARCH ALGORITHM ===========" << endl;
    cout << endl;

    string start =
        getValidNode("Enter Start Node ID       : ");

    string destination =
        getValidNode("Enter Destination Node ID : ");

    aStar(start, destination);

    waitForKey();
}

// =====================================================
// MAIN MENU
// =====================================================

void showMenu()
{
    cout << "*************** Student Campus Navigation System ***************"
         << endl;

    cout << "\nType a number to choose an action:\n"
         << endl;

    cout << "1. Campus Visualization" << endl;

    cout << "2. List campus nodes" << endl;

    cout << "3. Dijkstra's Algorithm "
         << "(find the shortest path between two locations)"
         << endl;

    cout << "4. A* Search Algorithm "
         << "(Uses heuristics to reach the destination faster)"
         << endl;

    cout << "0. Exit" << endl;

    cout << "\nEnter Choice: ";
}

// =====================================================
// MAIN FUNCTION
// =====================================================

int main()
{
    loadCampusData();

    int choice;

    do
    {
        //system("cls");

        showMenu();

        cin >> choice;

        switch (choice)
        {
            case 1:
                displayCampusGraph();
                break;

            case 2:
                listCampusNodes();
                break;

            case 3:
                dijkstraMenu();
                break;

            case 4:
                aStarMenu();
                break;

            case 0:
                cout << "\nThank you for using the Student Campus Navigation System! Goodbye!\n";
                break;

            default:
                cout << "\nInvalid Choice!" << endl;
                waitForKey();
        }

    } while (choice != 0);

    return 0;
}