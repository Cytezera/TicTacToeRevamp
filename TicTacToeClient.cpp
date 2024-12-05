#include <iostream>
#include <vector> 
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
using namespace std; 

void deserialize(const string& data, vector<vector<char>> &board){
	int counter = 0 ; 
	for (int i = 0 ; i < 3; i ++ ) {
		for (int j=0; j < 3; j++ ) {
			board[i][j] = data[counter];
			counter++ ;
		}
		counter++ ;
	}
}
void displayBoard(vector<vector<char>> board){
	cout << "  0   1   2" << endl; 
	for (int i = 0 ; i< 3; i ++) {
		cout << i << " " ; 
		for (int j = 0; j < 3; j ++) {
			cout << board[i][j] << " " ; 
			if (j < 2) {
				cout << "| "; 
			}
			
		}
		if ( i < 2 ) {

			cout <<  "\n" <<  "  ----------" << endl;     
		}
	}
}
void chooseTile(int clientSocket) {
	int row, col; 
	cout << "Please choose the row " << endl; 
	cin >> row; 
	string rowStr = to_string(row); 
	send(clientSocket, rowStr.c_str(), rowStr.size(), 0 ) ;
       	cout << "Please choose the column " << endl; 
	cin >> col ; 
	string colStr = to_string(col); 
	send(clientSocket, colStr.c_str(), colStr.size(), 0 ) ; 	
}
void runGame(int clientSocket){

	vector<vector<char>> board(3,vector<char>(3)); 
	char buffer[1024] = {0}; 
	recv(clientSocket, buffer, sizeof(buffer),0); 
	deserialize(buffer,board); 
	displayBoard(board); 
	chooseTile(clientSocket); 
}
void connectServer(){
	int clientSocket; 
	struct sockaddr_in serverAddr ;

	clientSocket = socket(AF_INET, SOCK_STREAM, 0 ) ; 
	serverAddr.sin_family = AF_INET; 
	serverAddr.sin_port = htons(8080); 
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1") ; 
	connect(clientSocket,(struct sockaddr*)&serverAddr, sizeof(serverAddr)) ; 
	cout << "Connecting . . . " << endl; 
	runGame(clientSocket); 
}
int main () { 
	connectServer() ;
}
