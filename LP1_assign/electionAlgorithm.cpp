// Assignment- C2
//  Title : Simulation of election algorithms ( Bully and Ring )
//  Code:
//  Bully Algorithm :


 #include <iostream>
 #include <vector>
 using namespace std;
 class BullyAlgorithm {
 private:
 int numProcesses;
 vector<int> processes;
 int coordinator;
 public:
 BullyAlgorithm(int n) {
 numProcesses = n;
 processes.resize(n);
 coordinator =-1;
 }
 void inputProcesses() {
 cout << "Enter process IDs in ascending order: \n";
 for (int i = 0; i < numProcesses; i++) {
 cin >> processes[i];
 }
 coordinator = processes[numProcesses- 1]; 
 cout << "Initial Coordinator is process " << coordinator << endl;
 }
 void crashCoordinator() {
 cout << "Coordinator process " << coordinator << " has crashed." << endl;
 coordinator =-1; // No current coordinator
 }
 void startElection() {
 int initiator;
 cout << "Enter the ID of the process initiating the election: ";
 cin >> initiator;
 int initiatorIndex = find(processes.begin(), processes.end(), initiator)
processes.begin();
 for (int i = initiatorIndex + 1; i < numProcesses; i++) {
cout << "Election message sent from " << processes[initiatorIndex] << " to " <<
 processes[i] << endl;
 }
 // Highest process becomes the coordinator
 coordinator = processes[numProcesses- 1];
 cout << "Process " << coordinator << " becomes the new coordinator." << endl;
 }
 };
 int main() {
 int numProcesses;
 cout << "Enter number of processes: ";
 cin >> numProcesses;
 BullyAlgorithm bully(numProcesses);
 bully.inputProcesses();
 int choice;
 while (true) {
 cout << "\n1. Crash coordinator\n2. Start election\n3. Exit\n";
 cin >> choice;
 switch (choice) {
 case 1:
 bully.crashCoordinator();
 break;
 case 2:
 bully.startElection();
 break;
 case 3:
 return 0;
 default:
 cout << "Invalid choice!" << endl;
 }
 }
 }
 Ring Algorithm :
 #include <iostream>
 #include <vector>
 using namespace std;
 class RingAlgorithm {
 private:
 int numProcesses;
vector<int> processes;
 int coordinator;
 public:
 RingAlgorithm(int n) {
 numProcesses = n;
 processes.resize(n);
 coordinator =-1;
 }
 void inputProcesses() {
 cout << "Enter process IDs in any order: \n";
 for (int i = 0; i < numProcesses; i++) {
 cin >> processes[i];
 }
 }
 void startElection() {
 int initiator;
 cout << "Enter the ID of the process initiating the election: ";
 cin >> initiator;
 int initiatorIndex = find(processes.begin(), processes.end(), initiator)
processes.begin();
 int maxID = initiator;
 cout << "Election message sent from " << initiator << " in a ring" << endl;
 int current = (initiatorIndex + 1) % numProcesses;
 while (current != initiatorIndex) {
 cout << "Election message sent to process " << processes[current] << endl;
 if (processes[current] > maxID) {
 maxID = processes[current]; // Track the highest ID
 }
 current = (current + 1) % numProcesses;
 }
 coordinator = maxID;
 cout << "Process " << coordinator << " becomes the new coordinator." << endl;
 }
 void displayCoordinator() {
 cout << "Current coordinator is process " << coordinator << endl;
 }
 };
 int main() {
 int numProcesses;
cout << "Enter number of processes: ";
 cin >> numProcesses;
 RingAlgorithm ring(numProcesses);
 ring.inputProcesses();
 int choice;
 while (true) {
 cout << "\n1. Start election\n2. Display coordinator\n3. Exit\n";
 cin >> choice;
 switch (choice) {
 case 1:
 ring.startElection();
 break;
 case 2:
 ring.displayCoordinator();
 break;
 case 3:
 return 0;
 default:
 cout << "Invalid choice!" << endl;
 }
 }
 }



 /*
 Output :
 Bully Algorithm :
 Enter number of processes: 5
 Enter process IDs in ascending order:
 10 20 30 40 50
 Initial Coordinator is process 50
 1. Crash coordinator
 2. Start election
 3. Exit
 1
 Coordinator process 50 has crashed.
 1. Crash coordinator
 2. Start election
 3. Exit
 2
 Enter the ID of the process initiating the election: 30
 Election message sent from 30 to 40
Election message sent from 30 to 50
 Process 50 becomes the new coordinator.
 Ring Algorithm :
 Enter number of processes: 5
 Enter process IDs in any order:
 30 10 40 50 20
 1. Start election
 2. Display coordinator
 3. Exit
 1
 Enter the ID of the process initiating the election: 30
 Election message sent from 30 in a ring
 Election message sent to process 10
 Election message sent to process 40
 Election message sent to process 50
 Election message sent to process 20
 Process 50 becomes the new coordinator.
 1. Start election
 2. Display coordinator
 3. Exit
 2
 Current coordinator is process 50 */