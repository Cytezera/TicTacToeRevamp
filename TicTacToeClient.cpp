#include <iostream>
#include <vector> 
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
using namespace std; 

void deserialize(const string& data){
	vector<vector<string>> board(3,vector<string>(3)); 
	stringstream rowStream(data); 	
	string row; 
	cout << data;		
}
void runGame(int clientSocket){


	char buffer[1024] = {0}; 
	recv(clientSocket, buffer, sizeof(buffer)); 
	deserialize(string(buffer,serializedBoard)); 
}
void connectServer(){
	int clientSocket; 
	struct sockaddr_in serverAddr ;

	clientSocket = socket(AF_INET, SOCK_STREAM, 0 ) ; 
	serverAddr.sin_family = AF_INET; 
	serverAddr.sin_port = htons(8080); 
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1") ; 
	connect(clientSocket,(struct sockaddr*)&serverAddr, sizeof(serverAddr)) ; 
	cout << "Connecting . . . " ; 
	runGame(clientSocket); 
}
int main () { 
	connectServer() ;
}
