// Assignment- C1
//  Title : Interprocess Communication using socket programming implemented multithreaded
//  echo sever.
//  Code:
//  Server Code :
//  import java.io.*;
//  import java.net.*;
//  public class EchoServer {
//  private static final int PORT = 1234;
//  public static void main(String[] args) {
//  try (ServerSocket serverSocket = new ServerSocket(PORT)) {
//  System.out.println("Server started on port " + PORT);
//  while (true) {
//  Socket clientSocket = serverSocket.accept();
//  System.out.println("Client connected: " + clientSocket.getInetAddress());
//  new ClientHandler(clientSocket).start();
//  }
//  } catch (IOException e) {
//  System.err.println("Error starting server: " + e.getMessage());
//  }
//  }
//  }
//  class ClientHandler extends Thread {
//  private Socket clientSocket;
//  public ClientHandler(Socket socket) {
//  this.clientSocket = socket;
//  }
//  public void run() {
//  try (
//  BufferedReader in = new BufferedReader(new
//  InputStreamReader(clientSocket.getInputStream()));
//  PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true)
//  ) {
//  String inputLine;
//  while ((inputLine = in.readLine()) != null) {
//  System.out.println("Received from client: " + inputLine);
//  out.println("Echo: " + inputLine);
//  }
//  } catch (IOException e) {
// System.err.println("Error in communication: " + e.getMessage());
//  } finally {
//  try {
//  clientSocket.close();
//  System.out.println("Client disconnected.");
//  } catch (IOException e) {
//  System.err.println("Error closing client socket: " + e.getMessage());
//  }
//  }
//  }
//  }
//  Client Code :
//  import java.io.*;
//  import java.net.*;
//  public class EchoClient {
//  private static final String SERVER_ADDRESS = "localhost";
//  private static final int SERVER_PORT = 1234;
//  public static void main(String[] args) {
//  try (Socket socket = new Socket(SERVER_ADDRESS, SERVER_PORT);
//  BufferedReader userInput = new BufferedReader(new
//  InputStreamReader(System.in));
//  BufferedReader in = new BufferedReader(new
//  InputStreamReader(socket.getInputStream()));
//  PrintWriter out = new PrintWriter(socket.getOutputStream(), true)) {
//  System.out.println("Connected to the server.");
//  String message;
//  while (true) {
//  System.out.print("Enter message: ");
//  message = userInput.readLine();
//  if ("bye".equalsIgnoreCase(message)) {
//  break;
//  }
//  out.println(message);
//  String response = in.readLine();
//  System.out.println("Server response: " + response);
//  }
//  } catch (IOException e) {
//  System.err.println("Error communicating with the server: " + e.getMessage());
//  }
// }
//  }
// //  Output :
// //  Server :
// //  Server started on port 1234
// //  Client connected: /127.0.0.1
// //  Received from client: Hello
// //  Received from client: How are you?
// //  Client disconnected.
// //  Client :
// //  Connected to the server.
// //  Enter message: Hello
// //  Server response: Echo: Hello
// //  Enter message: How are you?
// //  Server response: Echo: How are you?
// //  Enter message: by



// To execute the multithreaded socket programming assignment, follow these steps:

// ### Step 1: Compile the Java code

// You need to compile both the server and client code using the `javac` command:

// 1. Open a terminal/command prompt and navigate to the directory containing your Java files.
// 2. Compile the server code:
//    ```bash
//    javac EchoServer.java
//    ```
// 3. Compile the client code:
//    ```bash
//    javac EchoClient.java
//    ```

// ### Step 2: Run the Server

// Start the server first to listen for incoming client connections:

// 1. In the terminal/command prompt, run the server:
//    ```bash
//    java EchoServer
//    ```
//    - This will start the server on port 1234 and wait for clients to connect.

// ### Step 3: Run the Client

// Open a **new terminal/command prompt** (separate from the server window) and connect to the server using the client:

// 1. Run the client:
//    ```bash
//    java EchoClient
//    ```
//    - This will connect the client to the server at `localhost` on port `1234`.

// ### Step 4: Interact with the Client

// Once the client is connected, follow these steps to interact with the server:

// 1. Enter a message on the client terminal (e.g., "Hello").
// 2. The server will respond with an "Echo" message (e.g., "Echo: Hello").
// 3. Continue sending messages until you type `bye` to close the client connection.

// ### Expected Output:

// #### Server:
// ```
// Server started on port 1234
// Client connected: /127.0.0.1
// Received from client: Hello
// Received from client: How are you?
// Client disconnected.
// ```

// #### Client:
// ```
// Connected to the server.
// Enter message: Hello
// Server response: Echo: Hello
// Enter message: How are you?
// Server response: Echo: How are you?
// Enter message: bye
// ```

// ### Notes:
// - Ensure that the server is running before you start the client.
// - The client and server should be in the same network (or `localhost` for testing on the same machine).