#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <cstring> 
using namespace std; 

struct Turn{

	string move; 
	int playerTurn; 
};
void restartBoard(vector<vector<string>> &board){
	for (int i = 0 ; i < 3; i ++ ) {
		for (int j=0; j < 3 ; j ++ ) {
			board[i][j] = " "; 
		}
	}
}
bool checkResult (vector<vector<string>> board, Turn currentTurn) {
	for (int i =0 ; i < 3; i ++ ) 
	{
		if (( (board[i][0]== board[i][1]) && (board[i][1] == board[i][2])) || ((board[0][i] == board[1][i]) && (board[1][i]== board[2][i]))){
			return false;
		}

	} 
	if (((board[0][0] == board[1][1] ) && (board[1][1]== board[2][2])) || ((board[1][1] == board[0][2] ) && (board[1][1] == board[2][0]))){
		return false; 
	}
	return true; 
}
void chooseTurn(Turn &currentTurn, int client1, int client2) {

	if (currentTurn.playerTurn == client1)
	{
		currentTurn = {"O",client2}; 
	}else {
		currentTurn = {"X",client1}; 
	}
}	
void placeTile(vector<vector<string>> &board,  int client1, int client2,Turn currentTurn) {
	bool tilePlaced = false; 
	int player = currentTurn.playerTurn; 
	int row , col;
	string message ; 
	do{
		recv(player, &row, sizeof(row), 0) ; 
		recv(player, &col, sizeof(col), 0) ; 
		if (board[row][col] == " "){
			board[row][col] = currentTurn.move; 
			message = "Turn end" ; 
			tilePlaced = true; 	
		}else {
			message = "Invalid input, please try again " ; 
			tilePlaced = false; 
		}
		send(player,message.c_str(),message.size(), 0 ) ;	
	}while(!tilePlaced);
	
}
string serialize(vector<vector<string>> board) {
	string result; 
	for (int i = 0; i < 3; i ++ ) {
		for ( int j= 0; j <3 ; j ++ ){
			result += board[i][j];
		}
	}
	return result; 
}
void sendBoard(vector<vector<string>> board, int client1, int client2) {

	string serializedBoard = serialize(board); 
	send(client1, serializedBoard.c_str(), serializedBoard.size(), 0 ) ;
	send(client2,serializedBoard.c_str(), serializedBoard.size(), 0 ) ;

}
void startGame(int client1, int client2) {

	vector<vector<string>> board(3,vector<string>(3)); 
	restartBoard(board); 
	bool gameRunning = true; 
	Turn currentTurn = {"X",client1}; 
	Turn winner; 
	do{ 
		sendBoard(board,client1,client2); 
		placeTile(board,client1,client2,currentTurn); 	
		gameRunning = checkResult(board,currentTurn); 	
		if (!gameRunning) {
			winner = currentTurn; 
		}			
		chooseTurn(currentTurn,client1, client2) ;
	}while(gameRunning); 
	string res = "E";
	string message = "The winner is " + winner.move ; 
	send(client1,res.c_str() , sizeof(res), 0) ;
	send(client1,res.c_str() , sizeof(res), 0) ;
	send(client1,message.c_str(),message.size(),0 ); 
	send(client2,message.c_str(),message.size(),0); 
}
void connectSocket() { 
	int serverSocket, client1, client2; 
	struct sockaddr_in serverAddr, clientAddr ;
	socklen_t addr_size; 

	serverSocket = socket(AF_INET, SOCK_STREAM, 0 ) ;
	serverAddr.sin_family = AF_INET; 
	serverAddr.sin_port = htons(8080) ;
	serverAddr.sin_addr.s_addr = INADDR_ANY; 
	bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)); 

	listen(serverSocket, 2);
	cout << "Connecting . . . "  << endl;  
	addr_size = sizeof(clientAddr) ;
	client1 = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);
	cout << "Player 1 has joined" << endl; 
	client2 = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size) ;
	cout << "Player 2 has joined" << endl; 
	startGame(client1, client2) ;

	close(client1);
	close(client2) ; 

}
int main () { 
	connectSocket();
	return 0; 
}
