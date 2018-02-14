/******************************************************************************

Author: Jan Gula (xgulaj00)
Project: LDAP Server [ISA 2017 FIT VUT] 
Description: This is my implementation of a parallel non-blocking LDAP server 

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <err.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <regex>
#include <fcntl.h>
#include <algorithm>
#include <ctype.h>
using namespace std;
	
#define BUFFER 1024         // buffer for incoming messages
#define QUEUE 2             // queue length for  waiting connections

void searchResEntryGenerator(vector <string> resArr, int resultCnt, int newsock, int searchSizeLimit);
void searchResDoneGenerator(int newsock);
vector <string> equalityMatch(unsigned char buffer[BUFFER], vector <string> CSData, int newsock);
vector <string> substrings(unsigned char buffer[BUFFER], vector <string> CSData, int newsock);
vector <string> logicalOperators(unsigned char buffer[BUFFER], vector <string> CSData, int newsock, int filterLength, int opFlag);
int bfrCnt = 0, errFlag = 0;

int main(int argc , char *argv[]){
	int fd, flags, newsock, c, i = 0, ll;
	// file descriptor , flags for nonblock , newsocket for socket comm, c for getopt, i iterator, ll length
	unsigned int len, portNum = 389, searchFilter, searchSizeLimit;
	// searchFilter determines the decimal value of the filter in searchRequest
	string fileName;
	struct sockaddr_in server;   // the server configuration (socket info)
	struct sockaddr_in from;     // configuration of an incoming client (socket info)
	unsigned char buffer[BUFFER];
	pid_t pid; long p;           // process ID number (PID)
	struct sigaction sa;         // a signal action when CHILD process is finished
	vector<string> dataArr;

  
   while ((c = getopt (argc, argv, "hp:f:")) != -1){
    switch (c) {
      case 'p':
        portNum = atoi(optarg);
        break;
      case 'f':
 		fileName = optarg;
        break;
      case 'h':
      	cout << "LDAP Server reacting to LDAP messages" << endl;
      	cout << "Usage: " << argv[0] << "[-p port] -f file_path" << endl;      
      	cout << "-p parameter sets the port for listening" << endl; 
      	cout << "-f declares the input .csv file" << endl; 
      	break;
      default:
      	cerr << "Invalid arguments" << endl;
      	cerr << "Usage: " << argv[0] << "[-p port] -f file_path" << endl;
      	break;
    }
  }

    if(fileName.empty()){
    	cerr << "No input file given, input file is a compulsory argument!" << endl;
		return -1;    	
    }

	ifstream in(fileName);

	if(!in) {
		cerr << "Cannot open input file.\n";
		return -1;
	}
	
	// dataArr contains the file data separated by \n  	
	char str[255];
	while(in) {
		in.getline(str, 255);  // delim defaults to '\n'
		dataArr.push_back(str);
	}
	in.close();

	// CSData contains the dataArr data separated by a ';'
	// this means each user has it's cn on i%3 line uid on i%3+1 and mail on i%3+2
	string token;
	vector <string> CSData;
	for(unsigned int j=0; j<dataArr.size(); ++j){
		istringstream ss(dataArr[j]);
		while(getline(ss, token, ';')) {
		    CSData.push_back(token);
		}
	}
  // handling SIG_CHILD for concurrent processes
  // necessary for correct processing of child's PID 

  sa.sa_handler = SIG_IGN;      // ignore signals - no specific action when SIG_CHILD received
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);     // no mask needed 
  if (sigaction(SIGCHLD,&sa,NULL) == -1)  // when SIGCHLD received, no action required
    err (1,"sigaction() failed");


  if ((fd = socket(AF_INET , SOCK_STREAM, 0)) == -1)   //create a non blocking server socket
    err(1,"socket(): could not create the socket");
  
  if ((flags = fcntl(fd,F_GETFL,0)) < 0)
    flags |= O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flags) < 0) {
    cerr << "non blocking socket error." << endl;
    exit(1);
  }
  cout << "The socket successfully created by socket()" << endl;

  server.sin_family = AF_INET;             // initialize server's sockaddr_in structure
  server.sin_addr.s_addr = INADDR_ANY;     // wait on every network interface, see <netinet/in.h>
  server.sin_port = htons(portNum);  // set the port where server is waiting
     

  if( bind(fd,(struct sockaddr *)&server , sizeof(server)) < 0)  //bind the socket to the port
    err(1,"bind() failed");

  cout << "socket succesfully bounded to the port using bind()" << endl;
     
  if (listen(fd,QUEUE) != 0)   //set a queue for incoming connections
    err(1,"listen() failed");
     
  // accept connection from an incoming client
  // parameter "from" gets information about the client
  // - if we don't need it, put NULL instead; e.g., newsock=accept(fd,NULL,NULL);
  // newsock is a file descriptor to a new socket where incoming connection is processed

  len = sizeof(from);
  while(1){  // wait for incoming connections (concurrent server)

    // accept a new connection
    if ((newsock = accept(fd, (struct sockaddr *)&from, (socklen_t*)&len)) == -1)
      err(1,"accept failed");

    if ((pid = fork()) > 0){  // this is parent process
      close(newsock);
    }
    else if (pid == 0){  // this is a child process that will handle an incoming request
		p = (long) getpid(); // current child's PID
		close(fd);
		cout << "A new connection accepted from " << inet_ntoa(from.sin_addr) << " port " << ntohs(from.sin_port) << "(" << from.sin_port << ")" << " by process " << p << endl;

	  // process incoming message from the client using "newsock" socket

		read(newsock, buffer, BUFFER);

		bfrCnt = 5; // 5th byte determines the protocolOp
		if (((int)buffer[bfrCnt]) == 96){ // 96 is bindRequest in decimal
			bfrCnt += 4; // LL 0x02 0x01 [1..127]
			bfrCnt +=2; //0x04	LL		
			ll = (int)buffer[bfrCnt];
			int i = 0;
			while(i < ll){
				i++;
				++bfrCnt;
			}
			bfrCnt +=2; //0x80 LL
			ll = (int)buffer[bfrCnt];
			i = 0;
			while(i < ll){
				i++;
				++bfrCnt;
			}
		}
		bfrCnt = 0;
		unsigned char bindBuffer[14];
		memset(bindBuffer, 0, sizeof(bindBuffer));
		// Bind response message byte set
		bindBuffer[0] = 0x30;	bindBuffer[1] = 0x0c;	bindBuffer[2] = 0x02;	bindBuffer[3] = 0x01;	bindBuffer[4] = 0x01;
		bindBuffer[5] = 0x61;	bindBuffer[6] = 0x07;	bindBuffer[7] = 0x0a;	bindBuffer[8] = 0x01;	bindBuffer[9] = 0x00;
		bindBuffer[10] = 0x04;	bindBuffer[11] = 0x00;	bindBuffer[12] = 0x04;	bindBuffer[13] = 0x00;
		i = write(newsock,bindBuffer,14);    // send bind response
		if (i == -1)                     // check if data was successfully sent out
		  err(1,"write() failed.");
		else if (i != 14)
		  err(1,"write(): buffer written partially");
	  
		memset(buffer, 0, sizeof(buffer));
		read(newsock, buffer, BUFFER); // read search request

		bfrCnt = 5; // 5th byte determines protocolOp
		if((int)buffer[bfrCnt] == 99) {
			bfrCnt += 3; //LL 0x04 LL  bfrCnt = 8
			ll = (int)buffer[bfrCnt];
			i = 0;
			while(i < ll){
				i++;
				++bfrCnt;
			}
			bfrCnt += 3; //0x0a 0x01 [0,1,2]
			bfrCnt += 3; //0x0a 0x01 [0..3]
			bfrCnt += 3; // 0x02 0x0[1..4] [0 .. 2^31  - 1]
			searchSizeLimit = (int)buffer[bfrCnt];
			bfrCnt += 3; // 0x02 0x0[1..4] [0 .. 2^31  - 1]
			bfrCnt += 3; // 0x01 0x01 BOOL
			bfrCnt++;// 0xa[0..9]
			searchFilter = (int)buffer[bfrCnt];
			bfrCnt++;//  LL
			int filterLength = (int)buffer[bfrCnt];
			vector <string> realRes;
			switch(searchFilter) {
				case 160: //0xa0 AND
					realRes = logicalOperators(buffer, CSData, newsock, filterLength, 0);
					searchResEntryGenerator(realRes, 0, newsock, searchSizeLimit);
					searchResDoneGenerator(newsock);					
				break;
				case 161: //0xa1 OR
					realRes = logicalOperators(buffer, CSData, newsock, filterLength, 1);
					searchResEntryGenerator(realRes, 0, newsock, searchSizeLimit);
					searchResDoneGenerator(newsock);					
				break;
				case 162: //0xa2 NOT
					realRes = logicalOperators(buffer, CSData, newsock, filterLength, 2);
					searchResEntryGenerator(realRes, 0, newsock, searchSizeLimit);
					searchResDoneGenerator(newsock);					
				break;
				case 163://0xa3 EQUALITY MATCH
					realRes = equalityMatch(buffer, CSData, newsock);
					searchResEntryGenerator(realRes, 0, newsock, searchSizeLimit);
					searchResDoneGenerator(newsock);					
				break;
				case 164: //0xa4 SUBSTRINGS
					realRes = substrings(buffer, CSData, newsock);
					searchResEntryGenerator(realRes, 0, newsock, searchSizeLimit);
					searchResDoneGenerator(newsock);					
				break;
				default:
					cerr << "unknown command " << searchFilter << endl;
					break;
			} // switch



		close(newsock);                          // close the new socket
		exit(0);                                 // exit the child process
	    } 

	} else
    	err(1,"fork() failed");
  } //while
  
  // close the server 
  cout << "closing an original socket" << endl;
  close(fd);                               // close the original server socket
  return 0;
}

	/**
	* A generator for result entries.
	* @param resArr a string array containing the results of our query.
	* @param newsock value of a socket used for our client-server communication, needed for the write() function.
	* @param searchSizeLimit maximal value of entries, if this value is exceeded the global err flag is set to true
	* and a searchResDone with a size exceeded flag is generated.
	*/
void searchResEntryGenerator(vector <string> resArr, int resultCnt, int newsock, int searchSizeLimit) {
	string hexaFormat, hexaFormatThree; // hexaFormat = user name hexaFormatThree = user mail - Three because email is always on the third position in my comma separated value data
	int bfrCounter; // buffer counter
	unsigned char buffer[BUFFER];
	string cnS, mailS; // used to write the type of our result
	cnS = "cn"; 
	mailS = "mail";

	bool status = true;
    string temp1,temp2,temp3;
    // if any changes were commited run the sort again
    // we need this sort because (|(uid=xb*)(uid=xa*)) in this implementation stored the xb values first and xa values last and
    // ldapsearch always sorts the array alphabetically
    while (status) {
		status = false;

		for (unsigned int i = 0; i < resArr.size()/3 - 1; i++) {
			if (resArr[(3*i)+1] > resArr[(3*i)+4]){
				temp1 = resArr[3*i+3];
				temp2 = resArr[3*i+4];
				temp3 = resArr[3*i+5];
				resArr[3*i+3] = resArr[3*i];
				resArr[3*i+4] = resArr[3*i+1];
				resArr[3*i+5] = resArr[3*i+2];
				resArr[3*i] = temp1;
				resArr[3*i+1] = temp2;
				resArr[3*i+2] = temp3;
				status = true;
			}
		}
	}

	for(int m=0; (unsigned int)m < (resArr.size()/3); m++) { // size / 3 because we always send cn uid and mail 
		if(m == searchSizeLimit && searchSizeLimit != 0) // size limit exceeded check
			errFlag = 1;
		hexaFormat = resArr[m * 3]; // cn
		hexaFormatThree = resArr[(m * 3) + 2]; // mail
		int buffLen = 31 + (hexaFormat.length()) + (hexaFormatThree.length() - 1);	//header + cn length and mail length
		int origBuffLen = buffLen;
		char entryBuffer[buffLen + 1];
		memset(buffer, 0, sizeof(entryBuffer));
		// one result entry byte set
		entryBuffer[0] = 0x30; entryBuffer[1] = buffLen; 
		entryBuffer[2] = 0x02; entryBuffer[3] = 0x01; entryBuffer[4] = 0x02;
		entryBuffer[5] = 0x64; entryBuffer[6] = buffLen - 5; //bufflen -5 
		entryBuffer[7] = 0x04; entryBuffer[8] = 0x00;
		bfrCounter = 9; 
		entryBuffer[bfrCounter++] = 0x30;
		entryBuffer[bfrCounter++] = buffLen - 9;
		buffLen = 8 + (hexaFormat.length());
		entryBuffer[bfrCounter++] = 0x30; entryBuffer[bfrCounter++] = buffLen;
		entryBuffer[bfrCounter++] = 0x04; entryBuffer[bfrCounter++] = 0x02;
		// declaring the type of our response cn in this case
		for(unsigned int j = 0; j < 2; j++, bfrCounter++){
			entryBuffer[bfrCounter] = cnS[j];
		} 
		entryBuffer[bfrCounter++] = 0x31;
		buffLen = 2 + (hexaFormat.length());	
		entryBuffer[bfrCounter++] = buffLen;
		entryBuffer[bfrCounter++] = 0x04;
		entryBuffer[bfrCounter++] = hexaFormat.length();
		for(unsigned int j = 0; j < (hexaFormat.length()); j++, bfrCounter++){
			entryBuffer[bfrCounter] = hexaFormat[j];
		} 
		entryBuffer[bfrCounter++] = 0x30;
		buffLen = 10 + (hexaFormatThree.length());
		entryBuffer[bfrCounter++] = buffLen - 1;
		entryBuffer[bfrCounter++] = 0x04;
		entryBuffer[bfrCounter++] = 0x04;
		for(unsigned int j = 0; j < 4; j++, bfrCounter++){
			entryBuffer[bfrCounter] = mailS[j];
		} 
		entryBuffer[bfrCounter++] = 0x31;
		entryBuffer[bfrCounter++] = hexaFormatThree.length() + 2 - 1;
		entryBuffer[bfrCounter++] = 0x04;
		entryBuffer[bfrCounter++] = hexaFormatThree.length() - 1;
		for(unsigned int j = 0; j < hexaFormatThree.length() +1; j++, bfrCounter++){
			entryBuffer[bfrCounter] = hexaFormatThree[j];
		} 
		// write result entry to newsock where the client is listening
		int y = write(newsock,entryBuffer,origBuffLen+2);
		if (y == -1)                     // check if data was successfully sent out
		  err(1,"write() failed.");
		else if (y != origBuffLen+2)
		  err(1,"write(): buffer written partially");
		
	}

}

	/**
	* A generator for result done.
	* @param newsock value of a socket used for our client-server communication, needed for the write() function.
	*/

void searchResDoneGenerator(int newsock) {
	unsigned char doneBuffer[14];
	doneBuffer[0] = 0x30; doneBuffer[1] = 0x0c; doneBuffer[2] = 0x02;
	doneBuffer[3] = 0x01; doneBuffer[4] = 0x02; doneBuffer[5] = 0x65;
	doneBuffer[6] = 0x07; doneBuffer[7] = 0x0a; doneBuffer[8] = 0x01;
	if(errFlag) // global error flag
		doneBuffer[9] = 0x04; // IF SIZE EXCEEDED ERROR CHANGE BYTES
 	else
		doneBuffer[9] = 0x00;

	doneBuffer[10] = 0x04; doneBuffer[11] = 0x00;
	doneBuffer[12] = 0x04; doneBuffer[13] = 0x00;
	int y = write(newsock,doneBuffer,14);
	if (y == -1)                     // check if data was successfully sent out
	err(1,"write() failed.");
	else if (y != 14)
	err(1,"write(): buffer written partially");
}

	/**
	* A function for substrings e.g. uid=xg*00.
	* @param buffer[BUFFER] this is the search request buffer with a size of 1024 defined as a constant.
	* @param CSData an array of comma separated data, first line contains the name(cn) of a user second line uid
	* third line mail, for each user in our file so the size of CSData is 3*(number of users in file)
	* @param newsock value of a socket used for our client-server communication, needed for the write() function.
	* @return an array of results
	*/

vector <string> equalityMatch(unsigned char buffer[BUFFER], vector <string> CSData, int newsock) {
	unsigned int resultCnt = 0;
	int i = 0;
	vector <string> resArr; // array of results
	string attDes, assertVal; // attribute Description e.g. uid , assertion Value the value of our attribute

	bfrCnt += 2; //  0x04 LL
	int ll = (int)buffer[bfrCnt];
	while(i < ll){
		attDes += buffer[++bfrCnt];
		i++;
	}
	i = 0;
	bfrCnt += 2; //0x04 LL
	ll = (int)buffer[bfrCnt];
	while(i < ll){
		assertVal += buffer[++bfrCnt];
		i++;
	}
	i = 0;
	transform(attDes.begin(), attDes.end(), attDes.begin(), ::tolower); // because ldapsearch is case insensitive
	transform(assertVal.begin(), assertVal.end(), assertVal.begin(), ::tolower);
	for(unsigned int j=0; j < CSData.size(); ++j) { // cycling through our data if we find our attribute compare assertion value with the value in our data array and push to the result array if it's the same
		if((attDes.compare("uid")) == 0){
			if((CSData[j].compare(assertVal)) == 0){
				resArr.push_back(CSData[j-1]); // we have to preserve the structure first cn second uid third mail
				resArr.push_back(CSData[j]);
				resArr.push_back(CSData[j+1]);
				resultCnt++;
			}
		} else if((attDes.compare("mail")) == 0) {
			if((CSData[j].compare(assertVal)) == 0){
				resArr.push_back(CSData[j-2]);
				resArr.push_back(CSData[j-1]);
				resArr.push_back(CSData[j]);
				resultCnt++;
			}
		} else if((attDes.compare("cn")) == 0) {
			if((CSData[j].compare(assertVal)) == 0){
				resArr.push_back(CSData[j]);
				resArr.push_back(CSData[j+1]);
				resArr.push_back(CSData[j+2]);
				resultCnt++;
			}
		} else {
			cerr << "Unknown attribute description." <<  (int) buffer[bfrCnt] <<endl;
		}
	} // for
	return resArr;
}
	/**
	* A function for substrings e.g. uid=xg*00.
	* @param buffer[BUFFER] this is the search request buffer with a size of 1024 defined as a constant.
	* @param CSData an array of comma separated data, first line contains the name(cn) of a user second line uid
	* third line mail, for each user in our file so the size of CSData is 3*(number of users in file)
	* @param newsock value of a socket used for our client-server communication, needed for the write() function.
	* @return an array of results
	*/

vector <string> substrings(unsigned char buffer[BUFFER], vector <string> CSData, int newsock) {
	int length, ite = 0;
	string type, regEx; // for the type of substring regEx to save our regular Expression
	bfrCnt++; // 0x04
	length = (int)buffer[++bfrCnt]; // LL
	while(ite < length){	
		type += buffer[++bfrCnt];
		ite++;
	}
	ite = 0;
	bfrCnt += 2;
	switch((int)buffer[++bfrCnt]) {
		case 128:
			//initial
			length = buffer[++bfrCnt];;							
			while(ite < length){
				regEx += buffer[++bfrCnt];
				ite++;
			}
			ite = 0;

			if((int)buffer[++bfrCnt] == 129){ //initial -> any
				while(((int)buffer[bfrCnt]) == 129) {
					length = buffer[++bfrCnt];;							
					regEx += ".*";
					while(ite < length){
						regEx += buffer[++bfrCnt];
						ite++;
					}
					ite = 0;
				}
				if(((int)buffer[bfrCnt]) == 130){ //initial -> any -> final
					length = buffer[++bfrCnt];
					regEx += ".*";
					while(ite < length){
						regEx += buffer[++bfrCnt];
						ite++;
					}
					ite = 0;
				} else { // initial -> any
					regEx += ".*";
				}
			} else if((int)buffer[bfrCnt] == 130){ //initial -> final
				length = buffer[++bfrCnt];							
				regEx += ".*";
				while(ite < length) {
					regEx += buffer[++bfrCnt];
					ite++;
				}
			} else { //initial only we have to return the one byte eaten by the first if
				regEx += ".*";
				bfrCnt--;
			}
		break;
		case 129:
			//any
			while(((int)buffer[bfrCnt]) == 129) { // load any
				length = buffer[++bfrCnt];;							
				regEx += ".*";
				while(ite < length){
					regEx += buffer[++bfrCnt];
					ite++;
				}
				ite = 0;
			}
			if(((int)buffer[++bfrCnt]) == 130){ //any->final
				length = buffer[++bfrCnt];
				regEx += ".*";
				while(ite < length){
					regEx += buffer[++bfrCnt];
					ite++;
				}
				ite = 0;
			} else {//any only
				bfrCnt--;
				regEx += ".*";
			}
		break;
		case 130:
		// final
			length = buffer[++bfrCnt];							
			regEx += ".*";
			while(ite < length) {
				regEx += buffer[++bfrCnt];
				ite++;
			}
		break;
		default:
			cerr << "Undefined substring value!" << endl;
			break;
	}
	smatch match;
	transform(regEx.begin(), regEx.end(), regEx.begin(), ::tolower); // regular expressions have to be case insensitive too
	regex re(regEx);
	string result;
	vector <string> resArr;
	int resultCnt = 0;
	unsigned int j = 0;
	// These two for cycles are because regular expressions dont work properly for strings with spaces in them
	// that is the case for our cn type, so we had to erase the space and store these spaceless strings into a new array named helpArr
	vector <string> helpArr;
	for(unsigned int it=0; it < CSData.size(); it++){
		helpArr.push_back(CSData[it]);
	}
	for(unsigned int it=0; it < helpArr.size(); it++){
		if(it%3==0){ // because cn is always the first value out of the 3 in our CSData
			helpArr[it].erase(remove_if(helpArr[it].begin(), helpArr[it].end(), ::isspace), helpArr[it].end());
			transform(helpArr[it].begin(), helpArr[it].end(), helpArr[it].begin(), ::tolower);
		}
	}

	transform(type.begin(), type.end(), type.begin(), ::tolower); // take CN UID MAIL too

	for(j=0; j< CSData.size(); j++){
		if (regex_match(helpArr[j], match, re)) {
			if(!(type.compare("uid"))){
				if((j % 3) == 1){
					resArr.push_back(CSData[j - 1]);
					resArr.push_back(CSData[j]);
					resArr.push_back(CSData[j + 1]);
					resultCnt++;
				}
			} else if(!(type.compare("cn"))){
				if((j % 3) == 0){
					resArr.push_back(CSData[j]);
					resArr.push_back(CSData[j + 1]);
					resArr.push_back(CSData[j + 2]);
					resultCnt++;
				}
			} else if(!(type.compare("mail"))) {
				if((j % 3) == 2){
					resArr.push_back(CSData[j - 2]);
					resArr.push_back(CSData[j - 1]);
					resArr.push_back(CSData[j]);
					resultCnt++;
				}
			}
		}
	}
	return resArr;
}

	/**
	* A function for all logical operators [AND OR NOT] also used for recursive calls.
	* @param buffer[BUFFER] this is the search request buffer with a size of 1024 defined as a constant.
	* @param CSData an array of comma separated data, first line contains the name(cn) of a user second line uid
	* third line mail, for each user in our file so the size of CSData is 3*(number of users in file)
	* @param newsock value of a socket used for our client-server communication, needed for the write() function.
	* @param filterLength defines how long is the current operator in bytes.
	* @param opFlag defines which operator was used to call this function 0 = AND 1 = OR 2 = NOT.
	* @return an array of results
	*/

vector<string> logicalOperators(unsigned char buffer[BUFFER], vector <string> CSData, int newsock, int filterLength, int opFlag) {
	vector <string> mergeResArr, helpArr, defResArr; 
	// mergeResArr is the result of eqMatch or substrings
	// helpArr is either the result of the last logicalOperators call or stores all values of two subsequent queries e.g. uid=xa* uid=xb* , helpArr will store all xa* or xb* values 
	// defResArr definite Result array stores all results after the whole function passed
	int length, found = 0, filtLen, andCounter = 0;
	while(filterLength > 0){
		switch((int)buffer[++bfrCnt]) {
			case 160:
				filtLen = (int)buffer[++bfrCnt];
				helpArr = logicalOperators(buffer, CSData, newsock, filtLen, 0);
				filterLength -= (filtLen + 4); // because we skip 2 bytes and their lengths
			break;
			case 161:
				filtLen = (int)buffer[++bfrCnt];
				helpArr = logicalOperators(buffer, CSData, newsock, filtLen, 1);
				filterLength -= (filtLen + 4);
			break;
			case 163: // EQUALITY MATCH
				if(opFlag == 0){ 
				// if AND take all results of the current query and compare them to the result array of last querry 
				//if any match check if they are in the defResultArray already if not add them 
					length = (int)buffer[++bfrCnt];
					mergeResArr = equalityMatch(buffer, CSData, newsock);
					for(unsigned int iter=0; iter < mergeResArr.size(); iter++){
						for(unsigned int twiter=0; twiter < helpArr.size(); twiter++){
							if(!(helpArr[twiter].compare(mergeResArr[iter]))){
								for(unsigned int resIt=0; resIt < defResArr.size(); resIt++){
									if(!(defResArr[resIt].compare(mergeResArr[iter])))
										found = 1;
								}
								if(!(found))
									defResArr.push_back(mergeResArr[iter]);
								found = 0;								
							}
						}
					helpArr.push_back(mergeResArr[iter]);
					}
					if(mergeResArr.size() == 0) // if we have an AND operator and the result is empty resultArr will be empty too
						defResArr.clear();
					filterLength -= (length + 2);

				} else if (opFlag == 1){ // OR 
					// Add each result from the last query to our resultArray if it's not already there
					length = (int)buffer[++bfrCnt];
					mergeResArr = equalityMatch(buffer, CSData, newsock);
					for(unsigned int iter=0; iter < helpArr.size(); iter++){
						mergeResArr.push_back(helpArr[iter]);
					}
					for(unsigned int iter=0; iter < mergeResArr.size(); iter++){
						for(unsigned int twiter=0; twiter < defResArr.size(); twiter++){
							if(!(defResArr[twiter].compare(mergeResArr[iter]))){
								found = 1;
							}
						}
						if(!(found))
							defResArr.push_back(mergeResArr[iter]);
						found = 0;								
					}
					filterLength -= (length + 2);					
				} else if (opFlag == 2) { // NOT
					// Store the results of our query to mergeResArr and add every line from CSData to resultArr except the ones in mergeResArr
					length = (int)buffer[++bfrCnt];
					mergeResArr = equalityMatch(buffer, CSData, newsock);
					for(unsigned int it=0; it < CSData.size(); it++){
						for(unsigned int ite = 0; ite<mergeResArr.size(); ite++){
							if(!(mergeResArr[ite].compare(CSData[it])))
								found = 1;
						}
						if(!found)
							defResArr.push_back(CSData[it]);
						found = 0;
					}
					filterLength -= (length + 2);					
				}
			break;
			case 164: // SUBSTRINGS - the principle is the same as in EQUALITY MATCH
				if(opFlag == 0) {
					andCounter++;
					if(andCounter > 2)
						defResArr.clear();
					length = (int)buffer[++bfrCnt];
					mergeResArr = substrings(buffer, CSData, newsock);
					for(unsigned int iter=0; iter < mergeResArr.size(); iter++){
						for(unsigned int twiter=0; twiter < helpArr.size(); twiter++){
							if(!(helpArr[twiter].compare(mergeResArr[iter]))){
								for(unsigned int resIt=0; resIt < defResArr.size(); resIt++){
									if(!(defResArr[resIt].compare(mergeResArr[iter])))
										found = 1;
								}
								if(!(found))
									defResArr.push_back(mergeResArr[iter]);
								found = 0;								
							}
						}
						helpArr.push_back(mergeResArr[iter]);
					}
					if(mergeResArr.size() == 0){
						defResArr.clear();
					}
					filterLength -= (length + 2);

				} else if (opFlag == 1) {
					length = (int)buffer[++bfrCnt];
					mergeResArr = substrings(buffer, CSData, newsock);
					for(unsigned int iter=0; iter < helpArr.size(); iter++){
						mergeResArr.push_back(helpArr[iter]);
					}
					for(unsigned int iter=0; iter < mergeResArr.size(); iter++){
						for(unsigned int twiter=0; twiter < defResArr.size(); twiter++){
							if(!(defResArr[twiter].compare(mergeResArr[iter]))){
								found = 1;
							}
						}
						if(!(found))
							defResArr.push_back(mergeResArr[iter]);
						found = 0;								
					}
					filterLength -= (length + 2);					
				} else if (opFlag == 2) {
					length = (int)buffer[++bfrCnt];
					mergeResArr = substrings(buffer, CSData, newsock);
					for(unsigned int it=0; it < CSData.size(); it++){
						for(unsigned int ite = 0; ite<mergeResArr.size(); ite++){
							if(!(mergeResArr[ite].compare(CSData[it])))
								found = 1;
						}
						if(!found)
							defResArr.push_back(CSData[it]);
						found = 0;
					}
					filterLength -= (length + 2);					
				}
			break;
			default:
				cout << "Unexpected byte: " << (int)buffer[bfrCnt] << endl;
				filterLength = 0; // This is an ugly emergency stop if our program started to cycle
			break;
		} // switch
	} // while
	return defResArr;
} // fnc
