/*
*  Materials downloaded from the web. See relevant web sites listed on OLT
*  Collected and modified for teaching purpose only by Jinglan Zhang, Aug. 2006
*/


#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>

FILE * clientDetails;
FILE * toAuthenticate;
FILE * accountsFile;

  //TRANSACTIONS STRUCT
 typedef struct Transactions{
	int fromAccount;
	int toAccount;
	float amount;
	char transType[15];
  }Transactions;
Transactions* transactions = NULL;
	int tranSize = 0;

void append_transaction(int toAcc, int fromAcc, float amount, char* transtype)
	{
		int pos = tranSize;
		transactions = realloc(transactions, (tranSize + 1) * sizeof(Transactions));
		tranSize++;
                transactions[pos].fromAccount = fromAcc;
		transactions[pos].toAccount = toAcc;
		transactions[pos].amount = amount;
		strcpy(transactions[pos].transType , transtype);
	}
  //AUTH STRUCT
  struct Clients {
    char username[15];
    int pinNumber;
    int clientNumber;
  };
  struct Clients clients[14];

  //CLIENT INFO STRUCT
  struct ClientsInfo {
    char name[15];
    char surname[15];
    int clientnum;
		struct Figs{
		float open;
		float close;
		}figs;
	struct Figs2{
		float open;
		float close;
		}figs2;
	struct Figs3{
		float open;
		float close;
		}figs3;
    int account1;
    int account2;
    int account3;
  };
  struct ClientsInfo clientsinfo[10];


	struct Accounts{
		int accNum;
		float openBal;
		float closeBal;
	};struct Accounts accounts[24];


	#define ARRAY_SIZE 30  /* Size of array to receive */

	#define BACKLOG 10     /* how many pending connections queue will hold */

	#define RETURNED_ERROR -1

void fillClients(){
char usernameSaved[15];
int pin;
int clientNum;
//Variables
toAuthenticate = fopen("Authentication.txt", "r");
int cnt = 0;
char x;
 char str[100];
//Clearing first line
  do {
    x = fgetc(toAuthenticate);
  } while (x != '\n');
  //SCANNING AUTH.txt STORING ALL DATA IN STRUCTS
  while (fgets(str, 100, toAuthenticate)) {
		cnt++;
    sscanf(str, "%s %d %d", usernameSaved, &pin, &clientNum);
    if (feof(toAuthenticate))
      break;
    //Attempting to use my array of structs to loop and store all the pins of every user
    clients[cnt].pinNumber = pin;
    //storing every clients client number
    clients[cnt].clientNumber = clientNum;
    //storing client usernames
    for (int i = 0; i < 15; i++) {
      clients[cnt].username[i] = usernameSaved[i];
    }
    printf("clients user %s , clients pin %i\n", clients[cnt].username, clients[cnt].pinNumber);
  }
}

void fillClientsInfo(){
  char str[100];
  char c;
  int chk;
  int cnt2 = 0;
char detailsname[15];
char detailssurname[15];
int detailsaccount1;
int detailsaccount2;
int detailsaccount3;
int detailsclientnum;
clientDetails = fopen("Client_Details.txt", "r");
do{
    c = fgetc(clientDetails);
  } while (c != '\n');

  while (fgets(str, 100, clientDetails)) {

    /* writing content to stdout */
    //puts(str);
    chk = sscanf(str, "%s %s %d %d, %d, %d", detailsname, detailssurname, & detailsclientnum, & detailsaccount1, & detailsaccount2, & detailsaccount3);
	if(chk < 4 ){
		break;
		}
    if (chk == 4) {
      for (int i = 0; i < 15; i++) {
        clientsinfo[cnt2].name[i] = detailsname[i];
        clientsinfo[cnt2].surname[i] = detailssurname[i];
      }
      clientsinfo[cnt2].clientnum = detailsclientnum;
      clientsinfo[cnt2].account1 = detailsaccount1;
      clientsinfo[cnt2].account2 = 0;
      clientsinfo[cnt2].account3 = 0;
    }
    if (chk == 5) {
      for (int i = 0; i < 15; i++) {
        clientsinfo[cnt2].name[i] = detailsname[i];
        clientsinfo[cnt2].surname[i] = detailssurname[i];
      }
      clientsinfo[cnt2].clientnum = detailsclientnum;
      clientsinfo[cnt2].account1 = detailsaccount1;
	if(detailsaccount2 % 13 == 0){
	clientsinfo[cnt2].account3 = detailsaccount2;
	clientsinfo[cnt2].account2 = 0;
	}
	if(detailsaccount2 % 12 == 0){
	clientsinfo[cnt2].account2 = detailsaccount2;
	clientsinfo[cnt2].account3 = 0;
	}

    }
    if (chk == 6) {
      for (int i = 0; i < 15; i++) {
        clientsinfo[cnt2].name[i] = detailsname[i];
        clientsinfo[cnt2].surname[i] = detailssurname[i];
      }
      clientsinfo[cnt2].clientnum = detailsclientnum;
		if(detailsaccount2 % 13 == 0){
	clientsinfo[cnt2].account3 = detailsaccount2;
	clientsinfo[cnt2].account2 = detailsaccount3;
	}
	if(detailsaccount2 % 12 == 0){
	clientsinfo[cnt2].account2 = detailsaccount2;
	clientsinfo[cnt2].account3 = detailsaccount3;
	}
      clientsinfo[cnt2].account1 = detailsaccount1;
      clientsinfo[cnt2].account2 = detailsaccount2;
      clientsinfo[cnt2].account3 = detailsaccount3;
    }
    if (clientsinfo[cnt2].clientnum == clientsinfo[cnt2 - 1].clientnum) {
      break;
    }
		printf("\n\n%s %s %d %d %d %d\n\n", detailsname, detailssurname, detailsclientnum, detailsaccount1, detailsaccount2, detailsaccount3);
    cnt2++;
  }
}

void fillAccounts(){
int cnt3 = 0;
char x;
int accountNumber;
float openingBalance;
float closingBalance;
accountsFile = fopen("Accounts.txt", "r");
	char str3[100];
	//skipping first line of accounts file
do{
    x = fgetc(accountsFile);
 }while (x != '\n');

	while (fgets(str3, 100, accountsFile)) {
	sscanf(str3,"%i %f %f" , &accountNumber, &openingBalance, &closingBalance);
	accounts[cnt3].accNum = accountNumber;
	accounts[cnt3].openBal = openingBalance;
	accounts[cnt3].closeBal = closingBalance;
    if (accounts[cnt3].accNum == accounts[cnt3-1].accNum){
      break;}
	cnt3++;
	}
for(int i = 0; i < 24; i++){
printf("\n acc = %i open = %f close = %f \n" , accounts[i].accNum, accounts[i].openBal, accounts[i].closeBal);

}
}

char * Receive_Array_char_Data(int socket_identifier, int size) {
  int number_of_bytes, i = 0;
  char names;

  char * resultss = malloc(sizeof(char) * size);
  for (i = 0; i < size; i++) {
    if ((number_of_bytes = recv(socket_identifier, &names, sizeof(char), 0)) == RETURNED_ERROR) {
      perror("recv");
      exit(EXIT_FAILURE);
    }
    resultss[i] = names;
  }
  return resultss;
}

void sendStrings(int socket_id, char * theArray, int length) {
  int i = 0;
  char characters;
  for (i = 0; i < length; i++) {
    characters = theArray[i];
    send(socket_id, & characters, sizeof(char), 0);
  }

  for (i = 0; i < 1; i++) {
    characters = strlen(theArray) + '0';
    send(socket_id, & characters, sizeof(char), 0);
  }
}
void Send_Array_Data(int socket_id, char* array) {

	int i=0;
	/* Create an array of squares of first 30 whole numbers */
	int simpleArray[ARRAY_SIZE] = {0};
	for (i = 0; i < ARRAY_SIZE; i++) {
		simpleArray[i] = i * i;
	}

	uint16_t statistics;
	for (i = 0; i < ARRAY_SIZE; i++) {
		statistics = htons(simpleArray[i]);
		send(socket_id, &statistics, sizeof(uint16_t), 0);
	}
}


void *client_handler(void *socket_desc)
{
bool login = false;
bool correctAc = false;
char saveAcNum[20];
int saveTracker;
char onlineCnum[14];
char onlineAc1[14];
char onlineAc2[14];
char onlineAc3[14];
char onlineName[14];
char onlineSurname[14];
int usersClientNum;
char onlineOpen[14];
char onlineClose[14];
char onlineOpen2[14];
char onlineClose2[14];
char onlineClose3[14];

    //Get the socket descriptor
	puts("thread handling");
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000], user[15];
	char* resultss = Receive_Array_char_Data(sock, 30);
	for(int i = 0 ; i< 30; i++){
	printf("array[%d] %c\n",i, resultss[i]);
	}
	char userEnteredName[15];
      char arrayLim = resultss[28];
      char arrayLim2 = resultss[29];
      int intval = arrayLim - '0';
      int intval2 = arrayLim2 - '0';
      char pinEntered[intval2];
	int match = 0;

      //STORING PIN SENT TO SERVER FROM CLIENT
      for (int i = 0; i < intval2; i++) {
        pinEntered[i] = resultss[i + 15];
      }
      //STORING USERNAME SENT TO SERVER FROM CLIENT
      for (int i = 0; i < intval; i++) {
        userEnteredName[i] = resultss[i];
      }
      //CONVERT PIN FROM CHAR ARRAY TO INT VALUE
      int sentPin;
      sscanf(pinEntered, "%d", & sentPin);
      printf("Pin Send :   %i\n\n ", sentPin);

      //CHECKING WHATS GOING ON
      printf("%c\n\n", arrayLim);
      printf("%c\n\n", arrayLim2);
      printf("%s\n\n", pinEntered);
      printf("%i\n\n", intval);
	int tracker = 0;


	 for (int i = 1; i < 11; i++) {
        match = 0;
        for (int k = 0; k < intval; k++) {
          printf("%c  %c", clients[i].username[k], userEnteredName[k]);

          //CONFIRMING USERNAME MATCHES
          if (clients[i].username[k] == userEnteredName[k] && clients[i].pinNumber == sentPin) {
            match = match + 1;
          }
        }
	   if (match == intval) { //Bunch of character arrays to send to client
					login = true;
          printf("MATCH\n");

          //Users client number
          usersClientNum = clients[i].clientNumber;
          for (int i = 0; i < 10; i++) {
            //matching users client number against client details struct
            //converting acc nums to char arrays
            if (usersClientNum == clientsinfo[i].clientnum) {
              sprintf(onlineCnum, "%d", usersClientNum);
              sprintf(onlineAc1, "%d", clientsinfo[i].account1);
              sprintf(onlineAc2, "%d", clientsinfo[i].account2);
              sprintf(onlineAc3, "%d", clientsinfo[i].account3);
              //name & surname char arrays
              for (int k = 0; k < 15; k++) {
                onlineName[k] = clientsinfo[i].name[k];
                onlineSurname[k] = clientsinfo[i].surname[k];
              }
						for(int j = 0; j < 24; j++){
						if(clientsinfo[tracker].account1 == accounts[j].accNum){
						saveTracker = tracker;
						clientsinfo[tracker].figs.open = accounts[j].openBal;
						clientsinfo[tracker].figs.close = accounts[j].closeBal;
						printf("\n\nClient Number = %i \n Account Number %i \n Opening Balance = %f \n Closing Balance = %f \n",clientsinfo[tracker].clientnum,clientsinfo[tracker].account1, clientsinfo[tracker].figs.open, clientsinfo[tracker].figs.close);
}
						if(clientsinfo[tracker].account2 == accounts[j].accNum){
						clientsinfo[tracker].figs2.open = accounts[j].openBal;
						clientsinfo[tracker].figs2.close = accounts[j].closeBal;

}
}

						sprintf(onlineOpen, "%.2lf", clientsinfo[tracker].figs.open);
						sprintf(onlineClose, "%.2lf", clientsinfo[tracker].figs.close);
						sprintf(onlineOpen2, "%.2lf", clientsinfo[tracker].figs2.open);
						sprintf(onlineClose2, "%.2lf", clientsinfo[tracker].figs2.close);
						sprintf(onlineClose3, "%.2lf", clientsinfo[tracker].figs3.close);
            }

tracker++;
          }
					int ffs = strlen(onlineClose);
					onlineClose[ffs] = '\0';
					printf("\n%d", ffs);
          for (int p = 0; p < 15; p++) {
            //printf("%c\n", onlineAc1[p]);
						printf("%c", onlineClose[p]);
          }
          printf("\nStored client num = %d", usersClientNum);
          sendStrings(sock, onlineName, 14);
          sendStrings(sock, onlineSurname, 14);
          sendStrings(sock, onlineCnum, 14);
          sendStrings(sock, onlineAc1, 14);
          sendStrings(sock, onlineAc2, 14);
          sendStrings(sock, onlineAc3, 14);
	  sendStrings(sock, onlineOpen, 14);
	  sendStrings(sock, onlineClose,14);

        }
        //Using store client number find user details
        //variables for current user



        printf("\n Username : %s\n Pin : %d\n Client Num : %d \n\n", clients[i].username, clients[i].pinNumber, clients[i].clientNumber);

      } //  END FOR LOOP
  //IF LOGIN = FALSE , CLOSE CONNECTION SEND MESSAGE
      if (login == false) {
        char incorrect[] = "ERROR Username and/or pin was INCORRECT";
        send(sock, incorrect, strlen(incorrect) +1, 0);
        close(sock);
      }




    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
	char firstWord[1000], secondWord[1000],buf[1000];
	correctAc = false;
        //Send the message back to client
	sscanf(client_message, "%s %[^\r]\n",firstWord,secondWord);
	printf("Client input %s, First word %s, Second word %s\n", client_message, firstWord, secondWord);


	if(strcmp(firstWord, "BALANCE") == 0 && strcmp(secondWord, "SAVINGS") == 0) {
	// loop to update client balance from struct
	//for(int i = 0 ; i < 10; i++){
	//if(clientsinfo[i].account1 == atoi(onlineAc1)){
	sprintf(onlineClose, "%.2lf", clientsinfo[saveTracker].figs.close );
//}
//}
	sprintf(buf, "\n\n %s %s Your balance for account %s is  $%s", onlineName, onlineSurname, onlineAc1 , onlineClose);

	 write(sock , buf , strlen(buf));////////////////////////////

}

	if(strcmp(firstWord, "BALANCE") == 0 && strcmp(secondWord, "LOAN") == 0) {
	// loop to update client balance from struct
	//for(int i = 0 ; i < 10; i++){
	//if(clientsinfo[i].account2 == atoi(onlineAc2)){
	sprintf(onlineClose2, "%.2lf", clientsinfo[saveTracker].figs2.close );
//}
//}
	sprintf(buf, "\n\n %s %s Your balance for account %s is  $%s", onlineName, onlineSurname, onlineAc2 , onlineClose2);
	 write(sock , buf , strlen(buf));////////////////////////////

}

	if(strcmp(firstWord, "BALANCE") == 0 && strcmp(secondWord, "CREDIT") == 0) {
	// loop to update client balance from struct
	//for(int i = 0 ; i < 10; i++){
	//if(clientsinfo[i].account3 == atoi(onlineAc3)){
	sprintf(onlineClose3, "%.2lf", clientsinfo[saveTracker].figs3.close );
//}
//}
	sprintf(buf, "\n\n %s %s Your balance for account %s is  $%s", onlineName, onlineSurname, onlineAc3 , onlineClose3);
	 write(sock , buf , strlen(buf));////////////////////////////

}

	if(strcmp(firstWord, "WITHDRAWSAV") == 0) {
	char* store;
	float curr = strtod((secondWord),&store);
	float calc = clientsinfo[saveTracker].figs.close -= curr;
	if(calc >= 0){
	sprintf(onlineClose, "%.2lf", clientsinfo[saveTracker].figs.close);
	sprintf(buf, "\n\n %s %s Your balance for account %s is now $%s", onlineName, onlineSurname, onlineAc1 , onlineClose);
	append_transaction(0, atoi(onlineAc1), curr, "Withdrawal");
	write(sock , buf , strlen(buf)+1);////////////////////////////
	}else{
		calc = clientsinfo[saveTracker].figs.close += curr;
		write(sock, "Insufficient Funds - Unable To Process Request", strlen("Insufficient Funds - Unable To Process Request")+1);
	}

}
	if(strcmp(firstWord, "WITHDRAWCREDIT") == 0) {
	char* store;
	float curr = strtod((secondWord),&store);
	float calc = clientsinfo[saveTracker].figs3.close -= curr;
	if(calc >= clientsinfo[saveTracker].figs3.open - 5000){
	sprintf(onlineClose3, "%.2lf", clientsinfo[saveTracker].figs3.close);
	sprintf(buf, "\n\n %s %s Your balance for account %s is now $%s", onlineName, onlineSurname, onlineAc3 , onlineClose3);
	append_transaction(0, atoi(onlineAc3), curr, "Withdrawal");
	write(sock , buf , strlen(buf)+1);////////////////////////////
	}else{
		calc = clientsinfo[saveTracker].figs3.close += curr;
		write(sock, "Insufficient Funds - Unable To Process Request", strlen("Insufficient Funds - Unable To Process Request")+1);
	}

}


if(strcmp(firstWord, "DEPOSITSAV") == 0) {
	char* store;
	float curr = strtod((secondWord),&store);
	if(curr <= 1000){
	float calc = clientsinfo[saveTracker].figs.close += curr;
	sprintf(onlineClose, "%.2lf", clientsinfo[saveTracker].figs.close);
	sprintf(buf, "\n\nDeposit Completed: Closing Balance : %s" , onlineClose);
	append_transaction(atoi(onlineAc1), 0, curr, "Deposit");
	write(sock , buf , strlen(buf)+1);////////////////////////////
	}else{
	write(sock, "You cannot deposit more than $1000.00 in a single transaction!", strlen("You cannot deposit more than $1000.00 in a single transaction!")+1);
}
}
if(strcmp(firstWord, "DEPOSITLOAN") == 0) {
	char* store;
	float curr = strtod((secondWord),&store);
	if(curr <= 1000){
	float calc = clientsinfo[saveTracker].figs2.close += curr;
	sprintf(onlineClose2, "%.2lf", clientsinfo[saveTracker].figs2.close);
	sprintf(buf, "\n\nDeposit Completed: Closing Balance : %s" , onlineClose2);
	append_transaction(atoi(onlineAc2), 0, curr, "Deposit");
	 write(sock , buf , strlen(buf)+1);////////////////////////////
	}else{
	write(sock, "You cannot deposit more than $1000.00 in a single transaction!", strlen("You cannot deposit more than $1000.00 in a single transaction!")+1);
}
}
if(strcmp(firstWord, "DEPOSITCREDIT") == 0) {
	char* store;
	float curr = strtod((secondWord),&store);
	if(curr <= 1000){
	float calc = clientsinfo[saveTracker].figs3.close += curr;
	sprintf(onlineClose3, "%.2lf", clientsinfo[saveTracker].figs3.close);
	sprintf(buf, "\n\nDeposit Completed: Closing Balance : %s" , onlineClose3);
	append_transaction(atoi(onlineAc3), 0, curr, "Deposit");
	 write(sock , buf , strlen(buf)+1);////////////////////////////
	}else{
	write(sock, "You cannot deposit more than $1000.00 in a single transaction!", strlen("You cannot deposit more than $1000.00 in a single transaction!")+1);
}
}

//SINTERNALC Savings -> Credit Transfer
if(strcmp(firstWord, "SINTERNALC") == 0){
	//store the amount as a float curr
	char* store;
	float curr = strtod((secondWord),&store);
	float calc = clientsinfo[saveTracker].figs.close -= curr;
	if(calc >= 0){
	clientsinfo[saveTracker].figs3.close += curr;
	sprintf(onlineClose3, "%.2lf", clientsinfo[saveTracker].figs3.close);
	sprintf(buf, "\n\nINTERNAL TRANSFER\n\n\nDeducted %.2lf From: Account - %s Closing Balance - %.2lf\nTransfer %.2lf Dest: Account - %s - Closing Balance - %s" , curr, onlineAc1, clientsinfo[saveTracker].figs.close, curr, onlineAc3, onlineClose3);
	write(sock, buf, strlen(buf)+1);
	append_transaction(atoi(onlineAc3),atoi(onlineAc1), curr, "Transfer");

}else{
		calc = clientsinfo[saveTracker].figs.close += curr;
		write(sock, "Insufficient Funds - Unable To Process Request", strlen("Insufficient Funds - Unable To Process Request")+1);
	}
	}
//CINTERNALS Credit -> Savings Transfer
	if(strcmp(firstWord, "CINTERNALS") == 0) {
	char* store;
	float curr = strtod((secondWord),&store);
	float calc = clientsinfo[saveTracker].figs3.close -= curr;
	//If the transfer amount doesn't put the credit account below the maximum daily limit of -5000
	if(clientsinfo[saveTracker].figs3.close >= -5000){
	clientsinfo[saveTracker].figs.close += curr;
	sprintf(buf, "\n\nINTERNAL TRANSFER\n\n\nDeducted %.2lf From: Account - %d Closing Balance - %.2lf\nTransfer %.2lf Dest: Account - %d - Closing Balance - %.2lf" , curr, clientsinfo[saveTracker].account3, clientsinfo[saveTracker].figs3.close, curr, clientsinfo[saveTracker].account1, clientsinfo[saveTracker].figs.close);
	append_transaction(atoi(onlineAc1), atoi(onlineAc3), curr, "Transfer");
	write(sock , buf , strlen(buf)+1);////////////////////////////
	}else{
		calc = clientsinfo[saveTracker].figs3.close += curr;
		write(sock, "Insufficient Funds - Unable To Process Request", strlen("Insufficient Funds - Unable To Process Request")+1);
	}

}

//CINTERNALL Credit -> LOAN Transfer
	if(strcmp(firstWord, "CINTERNALL") == 0) {
	char* store;
	float curr = strtod((secondWord),&store);
	float calc = clientsinfo[saveTracker].figs3.close -= curr;
	//If the transfer amount doesn't put the credit account below the maximum daily limit of -5000
	if(clientsinfo[saveTracker].figs3.close >= -5000){
	clientsinfo[saveTracker].figs2.close += curr;
	sprintf(buf, "\n\nINTERNAL TRANSFER\n\n\nDeducted %.2lf From: Account - %d Closing Balance - %.2lf\nTransfer %.2lf Dest: Account - %d - Closing Balance - %.2lf" , curr, clientsinfo[saveTracker].account3, clientsinfo[saveTracker].figs3.close, curr, clientsinfo[saveTracker].account2, clientsinfo[saveTracker].figs2.close);
	append_transaction(atoi(onlineAc2), atoi(onlineAc3), curr, "Transfer");
	write(sock , buf , strlen(buf)+1);////////////////////////////
	}else{
		calc = clientsinfo[saveTracker].figs3.close += curr;
		write(sock, "Insufficient Funds - Unable To Process Request", strlen("Insufficient Funds - Unable To Process Request")+1);
	}

}



//SINTERNALC Savings -> Credit Transfer
if(strcmp(firstWord, "SINTERNALL") == 0){
	//store the amount as a float curr
	char* store;
	float curr = strtod((secondWord),&store);
	float calc = clientsinfo[saveTracker].figs.close -= curr;
	if(calc >= 0){
	clientsinfo[saveTracker].figs2.close += curr;
	sprintf(onlineClose2, "%.2lf", clientsinfo[saveTracker].figs2.close);
	sprintf(buf, "\n\nINTERNAL TRANSFER\n\n\nDeducted %.2lf From: Account - %s Closing Balance - %.2lf\nTransfer %.2lf Dest: Account - %s - Closing Balance - %s" , curr, onlineAc1, clientsinfo[saveTracker].figs.close, curr, onlineAc3, onlineClose2);
	write(sock, buf, strlen(buf)+1);
	append_transaction(atoi(onlineAc2),atoi(onlineAc1), curr, "Transfer");

}else{
		calc = clientsinfo[saveTracker].figs.close += curr;
		write(sock, "Insufficient Funds - Unable To Process Request", strlen("Insufficient Funds - Unable To Process Request")+1);
	}
	}


if(strcmp(firstWord, "EXTERNAL") == 0) {
	int trigger = 0;
	for(int i = 0; i < 24; i++){
	if(accounts[i].accNum == atoi(secondWord) && atoi(secondWord) > 0 && atoi(secondWord) != clientsinfo[saveTracker].account1 && atoi(secondWord) != clientsinfo[saveTracker].account2 && atoi(secondWord) != clientsinfo[saveTracker].account3){
		sprintf(buf, "Enter the Amount to Transfer (E/e to exit) - $");
	sprintf(saveAcNum,"%s", secondWord);
	write(sock , buf , strlen(buf)+1);////////////////////////////
	trigger = 1;
	}
	}
	if(trigger == 0 && strcmp(secondWord, "e") != 0 && strcmp(secondWord, "E") != 0){
	write(sock, "Invalid Account Number - Please try again (E/e to exit) -", strlen("Invalid Account Number - Please try again (E/e to exit) -")+1);

	}else{
	write(sock, "", strlen("")+1);}
	//trigger = 0;
}

if(strcmp(firstWord, "TRANSACTIONS") == 0 && transactions == NULL){
write(sock, "\n\n\nThere is currently no transaction history", strlen("There is currently no transaction history\n\n\n"));

}


if(strcmp(firstWord, "TRANSACTIONS") == 0 && transactions != NULL) {
	int transCount = 0;
	for(int k = 0; k <= sizeof(transactions); k++){
	if(atoi(secondWord) == transactions[k].fromAccount || atoi(secondWord) == transactions[k].toAccount){
	transCount++;
}
}
	sprintf(buf, "\nTotal Number of Transactions: %d \n " , transCount);
	write(sock, buf, strlen(buf));
	if(atoi(secondWord) % 11 == 0){
	sprintf(buf, "Opening Balance Account %d : $%.2lf \n", clientsinfo[saveTracker].account1, clientsinfo[saveTracker].figs.open);
}
if(atoi(secondWord) % 12 == 0){
	sprintf(buf, "Opening Balance Account %d : $%.2lf \n", clientsinfo[saveTracker].account2, clientsinfo[saveTracker].figs2.open);
}
if(atoi(secondWord) % 13 == 0){
	sprintf(buf, "Opening Balance Account %d : $%.2lf \n", clientsinfo[saveTracker].account3, clientsinfo[saveTracker].figs3.open);
}
	write(sock, buf, strlen(buf));
	write(sock,"\nTransaction \t Type \t       Amount \n", strlen("Transaction \t Type \t       Amount \n"));
	int count = 0;
	int count1 = 0;
	int count2 = 0;
	for(int i = 0; i <= sizeof(transactions); i++){
	if(atoi(secondWord) == transactions[i].fromAccount && atoi(secondWord) % 11 == 0) {
	count++;
	sprintf(buf, "\n   %d     \t%s     \t$-%.2lf " , count,transactions[i].transType, transactions[i].amount);
	write(sock, buf, strlen(buf));	
}
	if(atoi(secondWord) == transactions[i].toAccount && atoi(secondWord) % 11 == 0){
	count++;
	sprintf(buf, "\n   %d     \t%s     \t$%.2lf " ,  count ,transactions[i].transType, transactions[i].amount);
	write(sock, buf, strlen(buf));
}
	if(atoi(secondWord) == transactions[i].fromAccount && atoi(secondWord) % 12 == 0) {
	count1++;
	sprintf(buf, "\n   %d     \t%s     \t$-%.2lf " , count1,transactions[i].transType, transactions[i].amount);
	write(sock, buf, strlen(buf));	
}
	if(atoi(secondWord) == transactions[i].toAccount && atoi(secondWord) % 12 == 0){
	count1++;
	sprintf(buf, "\n   %d     \t%s     \t$%.2lf " ,  count1 ,transactions[i].transType, transactions[i].amount);
	write(sock, buf, strlen(buf));
}
	if(atoi(secondWord) == transactions[i].fromAccount && atoi(secondWord) % 13 == 0) {
	count2++;
	sprintf(buf, "\n   %d     \t%s     \t$-%.2lf " , count2,transactions[i].transType, transactions[i].amount);
	write(sock, buf, strlen(buf));	
}
	if(atoi(secondWord) == transactions[i].toAccount && atoi(secondWord) % 13 == 0){
	count2++;
	sprintf(buf, "\n   %d     \t%s     \t$%.2lf " ,  count2 ,transactions[i].transType, transactions[i].amount);
	write(sock, buf, strlen(buf));
}
}
if(atoi(secondWord) % 11 == 0){
sprintf(buf, "\n\n Closing Balance:  \t$%.2lf ", clientsinfo[saveTracker].figs.close);
}
if(atoi(secondWord) % 12 == 0){
sprintf(buf, "\n\n Closing Balance:  \t$%.2lf ", clientsinfo[saveTracker].figs2.close);
}
if(atoi(secondWord) % 13 == 0){
sprintf(buf, "\n\n Closing Balance:  \t$%.2lf ", clientsinfo[saveTracker].figs3.close);
}

write(sock, buf, strlen(buf)+1);
}


if(strcmp(firstWord, "AMOUNTD") == 0){
for(int i = 0; i<10; i++){
if(clientsinfo[i].account1 > 1 && clientsinfo[i].account1 == atoi(saveAcNum) && clientsinfo[i].account1 != clientsinfo[saveTracker].account1 && clientsinfo[i].account1 != clientsinfo[saveTracker].account2 && clientsinfo[i].account1 != clientsinfo[saveTracker].account3){
correctAc = true;
printf("SAVED ACCOUNT NUMBER %s" , saveAcNum);
char* store;
float curr = strtod((secondWord),&store);
clientsinfo[i].figs.close += curr;
// APPEND TRANSACTION
append_transaction(atoi(saveAcNum), atoi(onlineAc1), curr, "Transfer");
printf("to %d from %d amount %f type Transfer", atoi(saveAcNum), atoi(onlineAc1), curr);
printf("WOO HOO TRANSFER BABY");
break;
}
if(clientsinfo[i].account2 > 1 && clientsinfo[i].account2 == atoi(saveAcNum) && clientsinfo[i].account2 != clientsinfo[saveTracker].account1 && clientsinfo[i].account2 != clientsinfo[saveTracker].account2 && clientsinfo[i].account2 != clientsinfo[saveTracker].account3){
correctAc = true;
char* store;
float curr = strtod((secondWord),&store);
clientsinfo[i].figs2.close += curr;
// APPEND TRANSACTION
append_transaction(atoi(saveAcNum), atoi(onlineAc1), curr, "Transfer");
printf("to %d from %d amount %f type Transfer", atoi(saveAcNum), atoi(onlineAc1), curr);
printf("%s", secondWord);
printf("WOO HOO TRANSFER BABY");
break;
}
if(clientsinfo[i].account3 > 1 && clientsinfo[i].account3 == atoi(saveAcNum) && clientsinfo[i].account3 != clientsinfo[saveTracker].account1 && clientsinfo[i].account3 != clientsinfo[saveTracker].account2 && clientsinfo[i].account3 != clientsinfo[saveTracker].account2){
correctAc = true;
char* store;
float curr = strtod((secondWord),&store);
clientsinfo[i].figs3.close += curr;
// APPEND TRANSACTION
append_transaction(atoi(saveAcNum), atoi(onlineAc1), curr, "Transfer");
printf("to %d from %d amount %f type Transfer", atoi(saveAcNum), atoi(onlineAc1), curr);
printf("%s", secondWord);
printf("WOO HOO TRANSFER BABY");
break;
}

}
if(correctAc == false){
write(sock, "No such account number", strlen("No such account number")+1);
}

if(correctAc == true){
//remove money - ok to update onlineClose
char* store;
float curr = strtod((secondWord),&store);
clientsinfo[saveTracker].figs.close-=curr;
sprintf(onlineClose, "%.2lf", clientsinfo[saveTracker].figs.close );
sprintf(buf, "\n\n You sent $%s to account number %s",secondWord, saveAcNum);
write(sock , buf , strlen(buf)+1);////////////////////////////
}
}




if(strcmp(firstWord, "AMOUNTC") == 0){
for(int i = 0; i<10; i++){
if(clientsinfo[i].account1 > 1 && clientsinfo[i].account1 == atoi(saveAcNum) && clientsinfo[i].account1 != clientsinfo[saveTracker].account1 && clientsinfo[i].account1 != clientsinfo[saveTracker].account2 && clientsinfo[i].account1 != clientsinfo[saveTracker].account3){
correctAc = true;
char* store;
float curr = strtod((secondWord),&store);
clientsinfo[i].figs.close += curr;
// APPEND TRANSACTION
append_transaction(atoi(saveAcNum), atoi(onlineAc3), curr, "Transfer");
printf("to %d from %d amount %f type Transfer", atoi(saveAcNum), atoi(onlineAc1), curr);
printf("%s", secondWord);
printf("WOO HOO TRANSFER BABY");
break;
}
if(clientsinfo[i].account2 > 1 && clientsinfo[i].account2 == atoi(saveAcNum) && clientsinfo[i].account2 != clientsinfo[saveTracker].account1 && clientsinfo[i].account2 != clientsinfo[saveTracker].account2 && clientsinfo[i].account2 != clientsinfo[saveTracker].account3){
correctAc = true;
char* store;
float curr = strtod((secondWord),&store);
clientsinfo[i].figs2.close += curr;
// APPEND TRANSACTION
append_transaction(atoi(saveAcNum), atoi(onlineAc3), curr, "Transfer");
printf("to %d from %d amount %f type Transfer", atoi(saveAcNum), atoi(onlineAc1), curr);
printf("%s", secondWord);
printf("WOO HOO TRANSFER BABY");
break;
}
if(clientsinfo[i].account3 > 1 && clientsinfo[i].account3 == atoi(saveAcNum) && clientsinfo[i].account3 != clientsinfo[saveTracker].account1 && clientsinfo[i].account3 != clientsinfo[saveTracker].account2 && clientsinfo[i].account3 != clientsinfo[saveTracker].account2){
correctAc = true;
char* store;
float curr = strtod((secondWord),&store);
clientsinfo[i].figs3.close += curr;
// APPEND TRANSACTION
append_transaction(atoi(saveAcNum), atoi(onlineAc3), curr, "Transfer");
printf("to %d from %d amount %f type Transfer", atoi(saveAcNum), atoi(onlineAc1), curr);
printf("%s", secondWord);
printf("WOO HOO TRANSFER BABY");
break;
}

}
if(correctAc == false){
write(sock, "No such account number", strlen("No such account number")+1);
}

if(correctAc == true){
//remove money - ok to update onlineClose
char* store;
float curr = strtod((secondWord),&store);
clientsinfo[saveTracker].figs3.close-=curr;
sprintf(onlineClose3, "%.2lf", clientsinfo[saveTracker].figs3.close );

sprintf(buf, "\n\n You sent $%s to account number %s",secondWord, saveAcNum);
//append_transaction(0, atoi(onlineAc3), curr, "Withdraw");
write(sock , buf , strlen(buf)+1);////////////////////////////
}
}


else{
	 write(sock , "" , strlen("")+1);
}
       // write(sock , client_message , strlen(client_message)+1); }
	//memset(client_message,"\n",sizeof(client_message));
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;

}
int main(int argc, char *argv[]) {

	/* Thread and thread attributes */
	pthread_t client_thread;
	pthread_attr_t attr;


	int sockfd, new_fd, *newestSock;  /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr;    /* my address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;
	int i=0;

	/* Get port number for server to listen on */
	if (argc != 2) {
		fprintf(stderr,"usage: client port_number\n");
		exit(1);
	}

	/* generate the socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* generate the end point */
	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(atoi(argv[1]));     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
		/* bzero(&(my_addr.sin_zero), 8);   ZJL*/     /* zero the rest of the struct */

	/* bind the socket to the end point */
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
	== -1) {
		perror("bind");
		exit(1);
	}

	/* start listnening */
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	printf("server starts listnening ...\n");

	//Fills the clients structre , username , pin , clientNum
	fillClients();
	//Fills the clients Info structure
	fillClientsInfo();
	//Fills the accounts structure
	fillAccounts();
	int counter = 0;
	/* repeat: accept, send, close the connection */
	/* for every accepted connection, use a sepetate process or thread to serve it */
	while(1) {  /* main accept() loop */
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
		&sin_size)) == -1) {
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", \
			inet_ntoa(their_addr.sin_addr));
		newestSock = malloc(1);
		*newestSock = new_fd;

		//Create a thread to accept client

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&client_thread, &attr, client_handler,(void*) newestSock);
		//pthread_join(client_thread,NULL);

		//if (send(new_fd, "All of array data sent by server\n", 40 , 0) == -1)
		//		perror("send");
	if(new_fd < 0){
	return 1;
	}

	}
	return 0;

}

