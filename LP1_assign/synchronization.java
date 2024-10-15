 import java.util.Scanner;
 import java.util.concurrent.Semaphore;
 class ReaderWriterProblem {
 static Semaphore mutex = new Semaphore(1);
 static Semaphore writeBlock = new Semaphore(1);
 static int readCount = 0;
 static class Reader implements Runnable {
 @Override
 public void run() {
 try {
 mutex.acquire();
 readCount++;
 if (readCount == 1) {
 writeBlock.acquire();
 }
 mutex.release();
 System.out.println(Thread.currentThread().getName() + " is READING.");
 Thread.sleep(1000);
 System.out.println(Thread.currentThread().getName() + " has FINISHED
 READING.");
 mutex.acquire();
 readCount--;
 if (readCount == 0) {
 writeBlock.release();
 }
 mutex.release();
 } catch (InterruptedException e) {
 System.out.println(e.getMessage());
 }
 }
 }
 static class Writer implements Runnable {
 @Override
 public void run() {
 try {
 writeBlock.acquire();
System.out.println(Thread.currentThread().getName() + " is WRITING.");
 Thread.sleep(1500);
 System.out.println(Thread.currentThread().getName() + " has FINISHED
 WRITING.");
 writeBlock.release();
 } catch (InterruptedException e) {
 System.out.println(e.getMessage());
 }
 }
 }
 public static void main(String[] args) {
 Scanner sc = new Scanner(System.in);
 // Taking input from the user
 System.out.print("Enter the number of readers: ");
 int numReaders = sc.nextInt();
 System.out.print("Enter the number of writers: ");
 int numWriters = sc.nextInt();
 Reader reader = new Reader();
 Writer writer = new Writer();
 Thread[] readers = new Thread[numReaders];
 Thread[] writers = new Thread[numWriters];
 // Creating reader threads
 for (int i = 0; i < numReaders; i++) {
 readers[i] = new Thread(reader, "Reader " + (i + 1));
 }
 // Creating writer threads
 for (int i = 0; i < numWriters; i++) {
 writers[i] = new Thread(writer, "Writer " + (i + 1));
 }
 // Starting reader threads
 for (int i = 0; i < numReaders; i++) {
 readers[i].start();
 }
 // Starting writer threads
 for (int i = 0; i < numWriters; i++) {
 writers[i].start();
 }
 sc.close();
}
 }




/*

//  Testcase:
//  i/p :
//  Enter the number of readers: 3
//  Enter the number of writers: 2
//  o/p :
//  Reader 1 is READING.
//  Reader 2 is READING.
//  Reader 3 is READING.
//  Reader 1 has FINISHED READING.
//  Reader 2 has FINISHED READING.
//  Reader 3 has FINISHED READING.
//  Writer 1 is WRITING.
//  Writer 1 has FINISHED WRITING.
//  Writer 2 is WRITING.
//  Writer 2 has FINISHED WRITING

*/