#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

const int SIZE = 100;
struct Airports { string airportCode, city; };

class MatrixGraph {
private:
	vector<Airports> airportInfo;
	vector<list<int>> allFlight;
	vector<vector<float>> edge;
	string flightNum[SIZE][SIZE];
	int airportCount;
	int airportIndex(const string& str) {
		for (int i = 0; i < airportCount; i++) {
			if (airportInfo[i].airportCode == str) {
				return i;
			}
		} return -1;
	}
	string airportCode(const int& airportIndex) {
		return airportInfo[airportIndex].airportCode;
	}
	float lowestCost(float cost[], bool set[]) {
		float min = INT_MAX, cheapestIndest;
		for (int v = 0; v < airportCount; v++) {
			if (set[v] == false && cost[v] <= min) {
				min = cost[v], cheapestIndest = v;
			}
		}
		return cheapestIndest;
	}
	void printPath(int parent[], int j, int destIndex) {
		if (parent[j] == -1) { return; }
		printPath(parent, parent[j], destIndex);
		cout << flightNum[parent[j]][j] << " --> " << airportCode(j);
		if (j != destIndex) { cout << " -- "; }
	}

	void printFlight(int sourceIndex, int destIndex, float cost[], int parent[]) {
		int cur = destIndex;
		cout << airportCode(sourceIndex) << " -- ";
		printPath(parent, cur, destIndex);
		cout << " = $" << cost[destIndex] << endl;
	}

	void allFlightsTemp(const string& source, const string& dest) {
		vector<list<int>> edges;
		list<int> path, temp;
		int sourceInt = airportIndex(source);
		int destInt = airportIndex(dest);
		allFlight.clear();
		bool* isVisited = new bool[airportCount];
		for (int i = 0; i < airportCount; i++) { isVisited[i] = false; }
		for (int i = 0; i < airportCount; i++) {
			temp.clear();
			for (int j = 0; j < airportCount; j++) {
				if (edge[i][j] != 0) {
					temp.push_back(j);
				}
			} edges.push_back(temp);
		}
		path.push_back(airportIndex(source));
		allFlightsThru(sourceInt, destInt, isVisited, path, edges);
	}
	void allFlightsThru(int sourceInt, int destInt, bool isVisited[], list<int> localpath, vector<list<int>> edges) {
		list<int>* temp;
		isVisited[sourceInt] = true;
		if (sourceInt == destInt) {
			temp = &localpath;
			allFlight.push_back(*temp);
		}
		int cur = sourceInt;
		for (list<int>::iterator p = edges[cur].begin(); p != edges[cur].end(); p++) {
			if (!isVisited[*p]) {
				localpath.push_back(*p);
				allFlightsThru(*p, destInt, isVisited, localpath, edges);
				localpath.remove(*p);
			}
		}
		isVisited[sourceInt] = false;
	}
public:
	MatrixGraph() {
		vector<float>* vec = new vector<float>;
		string city, airportCode;
		fstream airports, flights;
		int airportNum = 0;
		airportCount = -1;
		airports.open("Airports.txt");
		while (!airports.eof()) {
			airports >> airportCode;
			airportNum++;
			getline(airports, city);
			Airports* airport = new Airports{ airportCode, city };
			airportCount++;
			airportInfo.push_back(*airport);
			(*vec).push_back(0);
		}
		for (int k = 0; k < airportCount; k++) { edge.push_back(*vec); }
		flights.open("Flights.txt");
		string i, j, flight;
		int iNum, jNum;
		float price;
		while (!flights.eof()) {
			flights >> i >> j >> price >> flight;
			if (i == "LAX") { iNum = 0; } if (i == "SFO") { iNum = 1; } if (i == "DFW") { iNum = 2; }
			if (i == "ORD") { iNum = 3; } if (i == "BOS") { iNum = 4; } if (i == "JFK") { iNum = 5; }
			if (i == "MIA") { iNum = 6; } if (i == "MSY") { iNum = 7; } if (i == "SEA") { iNum = 8; }
			if (j == "LAX") { jNum = 0; } if (j == "SFO") { jNum = 1; } if (j == "DFW") { jNum = 2; }
			if (j == "ORD") { jNum = 3; } if (j == "BOS") { jNum = 4; } if (j == "JFK") { jNum = 5; }
			if (j == "MIA") { jNum = 6; } if (j == "MSY") { jNum = 7; } if (j == "SEA") { jNum = 8; }
			edge[iNum][jNum] = price;
			flightNum[iNum][jNum] = flight;
		}
	}
	void debugGraph() {
		cout << setw(58) << "Source Destination\n";
		for (int i = 0; i < airportCount; i++) { cout << "       " << airportInfo[i].airportCode; } cout << endl;
		for (int i = 0; i < airportCount; i++) {
			cout << airportInfo[i].airportCode << setw(7);
			for (int j = 0; j < airportCount; j++) {
				cout << edge[j][i];
				if (j != airportCount - 1)
					cout << setw(10);
			} cout << endl; cout << "-------------------------------------------------------------------------------------------\n";
		}
	}
	void airportInformation(const string& airportCode) {
		for (int i = 0; i < airportInfo.size(); i++) {
			if (airportInfo[i].airportCode == airportCode) {
				cout << airportInfo[i].airportCode << " " << airportInfo[i].city << endl;
				return;
			}
		} cout << "Error, code not valid.\n";
	}
	bool validateairportCode(const string& source, const string& dest) {
		bool validate;
		for (int i = 0; i < airportInfo.size(); i++) {
			if (airportInfo[i].airportCode == source) { validate = true; break; }
			else { validate = false; }
		}
		for (int j = 0; j < airportInfo.size(); j++) {
			if (airportInfo[j].airportCode == dest) { validate = true; break; }
			else { validate = false; }
		}
		return validate;
	}
	float cheapestFlight(const string& source, const string& dest) {
		float* cost = new float[airportCount];
		for (int i = 0; i < airportCount; i++) { cost[i] = INT_MAX; }
		int* parent = new int[airportCount];
		for (int i = 0; i < airportCount; i++) { parent[i] = -1; }
		bool* set = new bool[airportCount];
		for (int i = 0; i < airportCount; i++) { set[i] = false; }
		int sourceIndex = airportIndex(source);
		int destIndex = airportIndex(dest);
		cost[sourceIndex] = 0;
		for (int count = 0; count < airportCount - 1; count++) {
			int u = lowestCost(cost, set);
			set[u] = true;
			for (int v = 0; v < airportCount; v++)
				if (!set[v] && edge[u][v] && cost[u] + edge[u][v] < cost[v]) {
					parent[v] = u;
					cost[v] = cost[u] + edge[u][v];
				}
		}
		printFlight(sourceIndex, destIndex, cost, parent);
		return cost[destIndex];
	}
	void cheapestRoundTrip(const string& source, const string& dest) {
		float costA = cheapestFlight(source, dest);
		float costB = cheapestFlight(dest, source);
		cout << "Total Cost: $" << costA + costB << endl;
	}
	void addFlight(const string& source, const string& dest, const string& flight, const float& cost) {
		int i = airportIndex(source);
		int j = airportIndex(dest);
		edge[i][j] = cost;
		flightNum[i][j] = flight;
	}
	void deleteFlight(const string& source, const string& dest) {
		int i = airportIndex(source);
		int j = airportIndex(dest);
		edge[i][j] = NULL;
		flightNum[i][j] = "";
	}
	void shortestFlight(const string& source, const string& dest) {
		allFlightsTemp(source, dest);
		int cheapestIndest = 0;
		for (int i = 1; i < allFlight.size(); i++) {
			if (allFlight[cheapestIndest].size() > allFlight[i].size()) {
				cheapestIndest = i;
			}
		}
		int counter = allFlight[cheapestIndest].size();
		for (list<int>::iterator it = allFlight[cheapestIndest].begin(); it != allFlight[cheapestIndest].end(); it++) {
			cout << airportCode(*it);
			int airport = airportIndex(airportCode(*it));
			if (counter > 2) { it++; int airport2 = airportIndex(airportCode(*it)); cout << " -- " << flightNum[airport][airport2]; it--; }
			if (counter == 2) { it++; int airport2 = airportIndex(airportCode(*it)); cout << " -- " << flightNum[airport][airport2]; it--; }
			if (counter > 1) { cout << " --> "; }
			counter--;
		}
	}
	void allFlights(const string& source, const string& dest) {
		allFlightsTemp(source, dest);
		for (int i = 0; i < allFlight.size(); i++) {
			int counter = allFlight[i].size();
			for (list<int>::iterator it = allFlight[i].begin(); it != allFlight[i].end(); it++) {
				cout << airportCode(*it);
				int airport = airportIndex(airportCode(*it));
				if (counter > 2) { it++; int airport2 = airportIndex(airportCode(*it)); cout << " -- " << flightNum[airport][airport2]; it--; }
				if (counter == 2) { it++; int airport2 = airportIndex(airportCode(*it)); cout << " -- " << flightNum[airport][airport2]; it--; }
				if (counter > 1) { cout << " --> "; }
				counter--;
			}
			cout << endl;
		}
	}
	void outputFlightsFile(fstream& AirportRev) {
		ofstream clearAirportRev;
		clearAirportRev.open("FlightsRev1.txt", ofstream::out | ofstream::trunc);
		clearAirportRev.close();
		AirportRev.open("FlightsRev1.txt");
		AirportRev << fixed << showpoint << setprecision(2);
		for (int i = 0; i < airportCount; i++) {
			for (int j = 0; j < airportCount; j++) {
				if (edge[i][j] != 0)
					AirportRev << airportCode(i) << setw(6) << airportCode(j) << setw(9) << edge[i][j] << setw(8) << flightNum[i][j] << endl;
			}
		}
	}
};

int main() {

	cout << "Airport Menu:\n1. Display airport information\n2. Find a cheapest flight from on airport to another airport\n3. Find a cheapest roundtrip from one airport to another airport\n"
		<< "4. Add a flight from one airport to another airport\n5. Delete a flight from one airport to another airport\n"
		<< "6. Find a flight with fewest stops from one airport to another airport\n7. Find all flights from one airport to another airport\n"
		<< "Q. Exit\n\n";

	MatrixGraph graph;
	fstream AirportRev;
	char input;

	cout << "Enter an option: ";
	cin >> input;
	while (input != 'Q') {
		if (input == '0') {
			graph.debugGraph();
			cout << "\nEnter an option: "; cin >> input;
		}
		else if (input == '1') {
			string airportCode;
			cout << "Enter airport airportCode: "; cin >> airportCode;
			graph.airportInformation(airportCode);
			cout << "\nEnter an option: "; cin >> input;
		}
		else if (input == '2') {
			string source, dest;
			cout << "Enter source: "; cin >> source;
			cout << "Enter destination: "; cin >> dest;
			if (graph.validateairportCode(source, dest)) {
				cout << "Cheapest flight: ";
				graph.cheapestFlight(source, dest);
			}
			else { cout << "Error, source or destination invalid.\n"; }
			cout << "\nEnter an option: "; cin >> input;
		}
		else if (input == '3') {
			string source, dest;
			cout << "Enter source: "; cin >> source;
			cout << "Enter destination: "; cin >> dest;
			if (graph.validateairportCode(source, dest)) {
				cout << "Cheapest round trip: \n";
				graph.cheapestRoundTrip(source, dest);
			}
			else { cout << "Error, source or destination invalid.\n"; }
			cout << "\nEnter an option: "; cin >> input;
		}
		else if (input == '4') {
			string source, dest, flight;
			float price;
			cout << "Enter source: "; cin >> source;
			cout << "Enter destination: "; cin >> dest;
			if (graph.validateairportCode(source, dest)) {
				cout << "Enter flight number: "; cin >> flight;
				cout << "Enter price: "; cin >> price;
				graph.addFlight(source, dest, flight, price);
				cout << "Flight added: ";
				graph.cheapestFlight(source, dest);
			}
			else { cout << "Error, source or destination invalid.\n"; }
			cout << "\nEnter an option: "; cin >> input;
		}
		else if (input == '5') {
			string source, dest, flight;
			cout << "Enter source: "; cin >> source;
			cout << "Enter destination: "; cin >> dest;
			if (graph.validateairportCode(source, dest)) {
				graph.deleteFlight(source, dest);
				cout << "Flight deleted.\n";
			}
			else { cout << "Error, source or destination invalid.\n"; }
			cout << "\nEnter an option: "; cin >> input;
		}
		else if (input == '6') {
			string source, dest;
			cout << "Enter source: "; cin >> source;
			cout << "Enter destination: "; cin >> dest;
			if (graph.validateairportCode(source, dest)) {
				graph.shortestFlight(source, dest);
			}
			else { cout << "Error, source or destination invalid.\n"; }
			cout << "\n\nEnter an option: "; cin >> input;
		}
		else if (input == '7') {
			string source, dest;
			cout << "Enter source: "; cin >> source;
			cout << "Enter destination: "; cin >> dest;
			if (graph.validateairportCode(source, dest)) {
				graph.allFlights(source, dest);
			}
			else { cout << "Error, source or destination invalid.\n"; }
			cout << "\nEnter an option: "; cin >> input;
		}
		else {
			cout << "Option invalid, please try again.";
			cout << "\nEnter an option: "; cin >> input;
		}
	}
	graph.outputFlightsFile(AirportRev);

	return 0;
}