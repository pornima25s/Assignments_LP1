// Assignment- B3
//  Title : Memory Management using First Fit , Best Fit , Next Fit , Worst Fit.
//  Code:



 import java.util.Scanner;
 public class MemoryManagement {
 static void firstFit(int blockSize[], int m, int processSize[], int n) {
 int allocation[] = new int[n];
 for (int i = 0; i < n; i++) allocation[i] =-1;
 for (int i = 0; i < n; i++) {
 for (int j = 0; j < m; j++) {
 if (blockSize[j] >= processSize[i]) {
 allocation[i] = j;
 blockSize[j]-= processSize[i];
 break;
 }
 }
 }
 System.out.println("Process No.\tProcess Size\tBlock No.");
 for (int i = 0; i < n; i++) {
 System.out.print(" " + (i + 1) + "\t\t" + processSize[i] + "\t\t");
 if (allocation[i] !=-1)
 System.out.print(allocation[i] + 1);
 else
 System.out.print("Not Allocated");
 System.out.println();
 }
 }
 static void bestFit(int blockSize[], int m, int processSize[], int n) {
 int allocation[] = new int[n];
 for (int i = 0; i < n; i++) allocation[i] =-1;
 for (int i = 0; i < n; i++) {
 int bestIdx =-1;
 for (int j = 0; j < m; j++) {
 if (blockSize[j] >= processSize[i]) {
 if (bestIdx ==-1 || blockSize[bestIdx] > blockSize[j])
 bestIdx = j;
 }
 }
 if (bestIdx !=-1) {
allocation[i] = bestIdx;
 blockSize[bestIdx]-= processSize[i];
 }
 }
 System.out.println("Process No.\tProcess Size\tBlock No.");
 for (int i = 0; i < n; i++) {
 System.out.print(" " + (i + 1) + "\t\t" + processSize[i] + "\t\t");
 if (allocation[i] !=-1)
 System.out.print(allocation[i] + 1);
 else
 System.out.print("Not Allocated");
 System.out.println();
 }
 }
 static void worstFit(int blockSize[], int m, int processSize[], int n) {
 int allocation[] = new int[n];
 for (int i = 0; i < n; i++) allocation[i] =-1;
 for (int i = 0; i < n; i++) {
 int worstIdx =-1;
 for (int j = 0; j < m; j++) {
 if (blockSize[j] >= processSize[i]) {
 if (worstIdx ==-1 || blockSize[worstIdx] < blockSize[j])
 worstIdx = j;
 }
 }
 if (worstIdx !=-1) {
 allocation[i] = worstIdx;
 blockSize[worstIdx]-= processSize[i];
 }
 }
 System.out.println("Process No.\tProcess Size\tBlock No.");
 for (int i = 0; i < n; i++) {
 System.out.print(" " + (i + 1) + "\t\t" + processSize[i] + "\t\t");
 if (allocation[i] !=-1)
 System.out.print(allocation[i] + 1);
 else
 System.out.print("Not Allocated");
 System.out.println();
 }
 }
 static void nextFit(int blockSize[], int m, int processSize[], int n) {
 int allocation[] = new int[n];
for (int i = 0; i < n; i++) allocation[i] =-1;
 int j = 0;
 for (int i = 0; i < n; i++) {
 while (j < m) {
 if (blockSize[j] >= processSize[i]) {
 allocation[i] = j;
 blockSize[j]-= processSize[i];
 break;
 }
 j = (j + 1) % m;
 }
 }
 System.out.println("Process No.\tProcess Size\tBlock No.");
 for (int i = 0; i < n; i++) {
 System.out.print(" " + (i + 1) + "\t\t" + processSize[i] + "\t\t");
 if (allocation[i] !=-1)
 System.out.print(allocation[i] + 1);
 else
 System.out.print("Not Allocated");
 System.out.println();
 }
 }
 public static void main(String[] args) {
 Scanner sc = new Scanner(System.in);
 System.out.print("Enter number of blocks: ");
 int m = sc.nextInt();
 int[] blockSize = new int[m];
 System.out.println("Enter size of each block:");
 for (int i = 0; i < m; i++) {
 blockSize[i] = sc.nextInt();
 }
 System.out.print("Enter number of processes: ");
 int n = sc.nextInt();
 int[] processSize = new int[n];
 System.out.println("Enter size of each process:");
 for (int i = 0; i < n; i++) {
 processSize[i] = sc.nextInt();
 }
 System.out.println("\nFirst Fit Allocation:");
 firstFit(blockSize.clone(), m, processSize, n);
System.out.println("\nBest Fit Allocation:");
 bestFit(blockSize.clone(), m, processSize, n);
 System.out.println("\nWorst Fit Allocation:");
 worstFit(blockSize.clone(), m, processSize, n);
 System.out.println("\nNext Fit Allocation:");
 nextFit(blockSize.clone(), m, processSize, n);
 }
 }

/*
//  Testcase:
//  i/p :
//  Enter the number of memory blocks: 5
//  Enter the size of each block:
//  100 500 200 300 600
//  Enter the number of processes: 4
//  Enter the size of each process:
//  212 417 112 426
//  o/p :
//  First Fit Allocation:
//  Process No. Process Size Block No.
//  1
//  212
//  2
//  3
//  4
//  417
//  112
//  426
//  Best Fit Allocation:
//  2
//  5
//  2
//  Not Allocated
//  Process No. Process Size Block No.
//  1
//  212
//  2
//  3
//  4
//  417
//  112
//  426
//  Next Fit Allocation:
//  4
//  5
//  3
//  Not Allocated
//  Process No. Process Size Block No.
//  1
//  212
//  2
//  3
//  4
//  417
//  112
//  426
//  2
//  5
//  3
//  Not Allocated
// Worst Fit Allocation:
//  Process No. Process Size Block No.
//  1
//  212
//  2
//  3
//  4
//  417
//  112
//  426
//  5
//  2
//  4
//  Not Allocated
 */
