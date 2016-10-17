#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>


char username[30];
char client_pin[15];
char input[15];
char ans[15];
char ans2[15];
char withdraw[15];
char deposit[15];
char outServer[64];
char* inServer;
int transCount;
int closing;
bool login = false;
char* temp;
bool selection = false;
char buf[1000];
bool account1 = false;
bool account2 = false;
bool account3 = false;

bool exitMenu(char* a1, char* a2, char* msg, char* buf){

	if(strcmp(a1,"e") == 0 || strcmp(a1,"E") == 0 || strcmp(a2,"e") == 0 || strcmp(a2,"E") == 0 || strcmp(msg, "e") == 0|| strcmp(msg, "E") == 0 || strcmp(buf,"e") ==0 || strcmp(buf, "E") == 0){
	selection = true;
//Reset Variables
a1[0] = 'Z';
a2[0] = 'Z';
msg[0] = 'Z';
buf[0] = 'Z';

return true;
}
return false;
}

	char* extTranSav(char* buf, char* msg){
		//ask the user how much they wish to send to the account
		//printf("Enter the Amount to Transfer (E/e to exit) - $");
		//intake the amount , store in buf
		scanf("%s", msg);
		sprintf(buf, "AMOUNTD %s", msg);
		return buf;
	}
		char* extTranCred(char* buf, char* msg){
		//ask the user how much they wish to send to the account
		//printf("Enter the Amount to Transfer (E/e to exit) - $");
		//intake the amount , store in buf
		scanf("%s", msg);
		sprintf(buf, "AMOUNTC %s", msg);
		return buf;
	}

char* tmenuOne( char* ac2, char* ac3, char* answer, char* answer2, char* msg, char* buf, int sock){
	//If there is a loan accounnt
	if(atoi(ac2) % 12 == 0 && atoi(ac2) > 0){
	//send server savings->internal->loan
	printf("Enter the Amount to Transfer (E/e  to exit) - ");
	scanf("%s", msg);
	sprintf(buf, "SINTERNALL %s", msg);
	//write(sock, buf, strlen(buf)+1);
	}
//If there is no loan account but there is a credit account
if(atoi(ac2) == 0 && atoi(ac3) % 13 == 0 && atoi(ac3) > 0 ){
	printf("Enter the Amount to Transfer (E/e  to exit) - ");
	scanf("%s", msg);
	sprintf(buf, "SINTERNALC %s", msg);
	//write(sock, buf, strlen(buf)+1);
}
//If there is no loan and no credit - external only
if(atoi(ac2) == 0 && atoi(ac3) == 0){
	printf("Enter Destination Account Number (E/e  to exit) - ");
	scanf("%s", msg);
	sprintf(buf, "EXTERNAL %s", msg);
	//write(sock, buf, strlen(buf)+1);
}

return buf;
}

char* tmenuOneC( char* ac2, char* ac1, char* answer, char* answer2, char* msg, char* buf, int sock){
	//If they have a credit, there is a savings DISP as first option
	if(atoi(ac1) % 11 == 0 && atoi(ac1) > 0){
	//send server credit->internal->savings
	printf("Enter the Amount to Transfer (E/e  to exit) - ");
	scanf("%s", msg);
	sprintf(buf, "CINTERNALS %s", msg);
	//write(sock, buf, strlen(buf)+1);
	}

return buf;
}

char* tmenuTwoC( char* ac2, char* ac1, char* answer, char* answer2, char* msg, char* buf, int sock){
	//If there is a loan account it will be second after savings
	if(atoi(ac2) % 12 == 0 && atoi(ac2) > 0) {
	printf("Enter the Amount to Transfer (E/e to exit) - ");
	scanf("%s", msg);
	//send server credit->internal->loan
	sprintf(buf, "CINTERNALL %s", msg);
	}
	//If there is no loan account then option 2 will be external after savings
	if(atoi(ac2) == 0) {
	printf("Enter Destination Account Number (E/e  to exit) - ");
	scanf("%s", msg);
	//send server credit->external
	sprintf(buf, "EXTERNAL %s", msg);
	}

return buf;
}

char* tmenuTwo( char* ac2, char* ac3, char* answer, char* answer2, char* msg, char* buf, int sock){
	//If there is a loan accounnt then the second option will be credit
	if(atoi(ac2) % 12 == 0 && atoi(ac2) > 0){
	//send server savings->internal->credit
	printf("Enter the Amount to Transfer (E/e  to exit) - ");
	scanf("%s", msg);
	sprintf(buf, "SINTERNALC %s", msg);
	//write(sock, buf, strlen(buf)+1);
	}
//If there is no loan account but there is a credit account the second option will be external
if(atoi(ac2) == 0 && atoi(ac3) % 13 == 0 && atoi(ac3) > 0 ){
	printf("Enter Destination Account Number (E/e  to exit) - ");
	scanf("%s", msg);
	sprintf(buf, "EXTERNAL %s", msg);
	//write(sock, buf, strlen(buf)+1);
}
return buf;
}

char* tmenuThree(char* ac2, char* ac3, char* msg, char* buf){
	//If there is a loan acc and a credit acc, then there will be external
	if(atoi(ac2) > 0 && atoi(ac3) > 0) {
		printf("Enter Destination Account Number (E/e  to exit) - ");
	scanf("%s", msg);
	sprintf(buf, "EXTERNAL %s", msg);
	//write(sock, buf, strlen(buf)+1);
}
return buf;


}

void introMenu(char* name, char* surname, char *clientNumber){
printf("\n\nWelcome to the ATM System");
printf("\n\nYou are currently logged in as %s %s \n", name, surname);
printf("Client Number - %s" , clientNumber);
}

char* selectionMenu(){
printf("\n\nPlease enter a selection");
printf("\n<1> Account Balance");
printf("\n<2> Withdrawal");
printf("\n<3> Deposit");
printf("\n<4> Transfer");
printf("\n<5> Transaction Listing");
printf("\n<6> EXIT");
printf("\n\nSelect option 1-6 -->");
//printf("\nOpening Balance : %s", clientOpen);
//printf("\nClosing Balance : %s", clientClose);
scanf("%s",input);
return input;
}



char *Receive_Array_char_Data(int socket_identifier, int size) {
    int number_of_bytes, i=0;
    char names;

	char *resultss = malloc(sizeof(char)*size);
	for (i=0; i < size; i++) {
		if ((number_of_bytes=recv(socket_identifier, &names, sizeof(char), 0))
		         == -1) {
			perror("recv");
			exit(EXIT_FAILURE);
		}
		resultss[i] = names;
	}
	return resultss;
}


	#define MAXDATASIZE 100 /* max number of bytes we can get at once */

	#define ARRAY_SIZE 30
void interface(){
  printf("===========================================================\n\n");
  printf("Welcome to the Online ATM System \n\n");
  printf("===========================================================\n\n");
  printf("\n\n\nYou are required to logn with your registered Username and PIN\n");
	printf("\nPlease enter your Username --> ");
  username[15] = scanf("%s", username);
  printf("\nPlease enter your pin --> ");
  client_pin[15] = scanf("\n%s", client_pin);

}


void invalidAccount(char* reply, char* msg, char* buf, int sock){
		while(strcmp("Invalid Account Number - Please try again (E/e to exit) -", reply) == 0 ) { 
	scanf("%s", msg);
	sprintf(buf, "EXTERNAL %s", msg);
	write(sock, buf, strlen(buf) + 1 );
	//Receive a reply from the server
        if( recv(sock , reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	//Display message back from server
	puts(reply);
	}
	
	//If client has no extra accounts, First option will be EXTERNAL
	//If server replies asking for an amount to transfer to external account num
	if(strcmp("Enter the Amount to Transfer (E/e to exit) - $", reply) == 0 ) {
	//save amount to send to buf
	extTranSav(buf, msg);
	//Send the userinput to server
	write(sock, buf, strlen(buf) + 1);
	//Receive a reply from the server
        if( recv(sock , reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	//Display message back from server
	puts(reply);
	}
	//while the server is sending back an incorrect client num input
}

void invalidAccountCred(char* reply, char* msg, char* buf, int sock){
		while(strcmp("Invalid Account Number - Please try again (E/e to exit) -", reply) == 0 ) { 
	scanf("%s", msg);
	sprintf(buf, "EXTERNAL %s", msg);
	write(sock, buf, strlen(buf) + 1 );
	//Receive a reply from the server
        if( recv(sock , reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	//Display message back from server
	puts(reply);
	}
	
	//If client has no extra accounts, First option will be EXTERNAL
	//If server replies asking for an amount to transfer to external account num
	if(strcmp("Enter the Amount to Transfer (E/e to exit) - $", reply) == 0 ) {
	//save amount to send to buf
	extTranCred(buf, msg);
	//Send the userinput to server
	write(sock, buf, strlen(buf) + 1);
	//Receive a reply from the server
        if( recv(sock , reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	//Display message back from server
	puts(reply);
	}
	//while the server is sending back an incorrect client num input
}

char* transTo(char* ac1, char* ac2, char* ac3){
if(atoi(input) == 4 && atoi(ans) == 1){
printf("\n\nSelect account type");
if(atoi(ac2) > 0 && atoi(ac3) > 0){
printf("\n<1> Loan");
printf("\n<2> Credit");
printf("\n<3> External");
}
if(atoi(ac2) == 0 && atoi(ac3) > 0){
printf("\n<1> Credit");
printf("\n<2> External");
}
if(atoi(ac3) == 0 && atoi(ac2) > 0){
printf("\n<1> Loan");
printf("\n<2> External");
}
if(atoi(ac3) == 0 && atoi(ac2) == 0){
printf("\n<1> External");
}



}

if(atoi(input) == 4 && atoi(ans) == 2){
printf("\n\nSelect account type");
if(atoi(ac1) > 0 && atoi(ac2) > 0){
printf("\n<1> Savings");
printf("\n<2> Loan");
printf("\n<3> External");
}
if(atoi(ac2) == 0 && atoi(ac1) > 0){
printf("\n<1> Savings");
printf("\n<2> External");
}


}

printf("\n\nSelect Account To Transfer To (E/e to exit):");
scanf("%s",ans2);

return ans2;
}

char* whatAccounts(char* ac1, char* ac2, char* ac3){

if(atoi(input) == 1 || atoi(input) == 3 || atoi(input) == 5){
printf("\n\nSelect account type");

//If there is only a savings account
if(atoi(ac2) == 0 && atoi(ac3) == 0){
printf("\n<1> Savings Account");
account1 = true;
}

//if there is a saving account and only a loan
if(atoi(ac2) > 0 && atoi(ac3) ==0 ){
printf("\n<1> Savings Account");
printf("\n<2> Loan Account");
account1 = true;
account2 = true;
}
//if there is a saving account and only a credit
if(atoi(ac3) > 0 && atoi(ac2) ==0 ){
printf("\n<1> Savings Account");
printf("\n<2> Credit Account");
account1 = true;
account3 = true;
}
//if there is a savings and + 2 extra acc
if(atoi(ac3) > 0 && atoi(ac2) > 0 ) {
printf("\n<1> Savings Account");
printf("\n<2> Loan Account");
printf("\n<3> Credit Account");
account1 = true;
account2 = true;
account3 = true;
} 
printf("\nPlease make your selection (E/e to exit) -");
}


//WITHDRAW OPTION - NO LOAN ACCOUNT
if(atoi(input) == 2 || atoi(input) == 4){
printf("\n\nSelect account type");
//ACCOUNT 1
if(atoi(ac1) > 0 && atoi(ac1) % 11 == 0){
printf("\n<1> Savings Account");
account1 = true;
}

if(atoi(ac1) > 0 && atoi(ac1) % 13 == 0){
printf("\n<1> Credit Card Account");
account3 = true;
}
//ACCOUNT 2
if(atoi(ac2) > 0 && atoi(ac2) % 11 == 0){
printf("\n<2> Savings Account");
account1 = true;
}

if(atoi(ac2) > 0 && atoi(ac2) % 13 == 0){
printf("\n<2> Credit Card Account");
account3 = true;
}
//ACCOUNT 3
if(atoi(ac3) > 0 && atoi(ac3) % 11 == 0){
printf("\n<2> Savings Account");
account1 = true;
}

if(atoi(ac3) > 0 && atoi(ac3) % 13 == 0){
printf("\n<2> Credit Card Account");
account3 = true;
}
if(atoi(input) == 1 || atoi(input) == 2 || atoi(input) == 3){
printf("\nEnter your selection (E/e to exit) - ");
}
if(atoi(input) == 4){
printf("\nSelect Account To Transfer From (E/e to exit) -");
}
}


scanf("%s",ans);
return ans;
}



void clrscr()
{
    system("@cls||clear");
}

void Send_Username(int socket_id, char *myArray, char *myArray2) {
	int i=0;
	char names;
	for (i = 0; i < 15; i++) {
		names = myArray[i];
		send(socket_id, &names, sizeof(char), 0);
	}
	for (i = 0; i < 13; i++) {
		names = myArray2[i];
		send(socket_id, &names, sizeof(char), 0);
	}
	//username length for match purposes
	for (i = 0; i < 1; i++) {
		names = strlen(myArray) + '0';
		send(socket_id, &names, sizeof(char), 0);
	}
		//username length for match purposes
	for (i = 0; i < 1; i++) {
		names = strlen(myArray2) + '0';
		send(socket_id, &names, sizeof(char), 0);
	}
}

void Send_Trans(int socket_id, char *myArray) {
	int i=0;
	char names;
	for (i = 0; i < transCount; i++) {
		names = myArray[i];
		send(socket_id, &names, sizeof(char), 0);
	}

}


int main(int argc, char *argv[]) {
	int sockfd, numbytes, i=0;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */

	if (argc != 3) {
		fprintf(stderr,"usage: client_hostname port_number\n");
		exit(1);
	}

	if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
		herror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}


	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(atoi(argv[2]));    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */


	if (connect(sockfd, (struct sockaddr *)&their_addr, \
	sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
	clrscr();
	interface(); // create user interface of an ATM , take username and
//			pin number and send to server for authentication


	//Send_Username(sockfd, username, client_pin);
	//char* buff = Receive_Array_char_Data(sockfd, 1024);
	//if(buff[0] == 'E' && buff[1] == 'R' && buff[2] == 'R'){
	//printf("Incorrect Username and/or Password");
	//close(sockfd);
	//return 0;
	//}

char message[1000] , server_reply[2000], fromBuf[120], trans[1000];


Send_Username(sockfd,username, client_pin);
recv(sockfd, fromBuf, 120, 0);

char clientName[14];
char clientSurname[14];
char clientNum[14];
char clientAc1[14];
char clientAc2[14];
char clientAc3[14];
char clientOpen[14];
char clientClose[14];


//Client Name
for(int i = 0; i < fromBuf[15]; i++){
clientName[i] = fromBuf[i];
}
//Client Surname
for(int i = 0; i < fromBuf[15+15]; i++){
clientSurname[i] = fromBuf[i+15];
}
//client Number
for(int i = 0; i < fromBuf[15+15+15]; i++){
clientNum[i] = fromBuf[i+30];
}
//Account number 1
for(int i = 0; i < fromBuf[15+15+15+15]; i++){
clientAc1[i] = fromBuf[i+45];
}
//Account number 2
for(int i = 0; i < fromBuf[15+15+15+15+15]; i++){
clientAc2[i] = fromBuf[i+60];
}
//Account number 3
for(int i = 0; i < fromBuf[15+15+15+15+15+15]; i++){
clientAc3[i] = fromBuf[i+75];
}
//Clients Opening Balance
for(int i = 0; i < fromBuf[15+15+15+15+15+15+15]; i++){
clientOpen[i] = fromBuf[i+90];
}
//Clients Opening Balance
for(int i = 0; i < fromBuf[15+15+15+15+15+15+15+15]; i++){
clientClose[i] = fromBuf[i+105];
}

closing = atoi(clientClose);

	if(strcmp(fromBuf,"ERROR Username and/or pin was INCORRECT") == 0){
	    close(sockfd);
	printf("Goodbye");
	return 0;
	exit(1);
}

login = true;
clrscr();
 while(1)
    {
	if(selection == true){
	//Reset Variables
ans[0] = 'Z';
ans2[0] = 'Z';
message[0] = 'Z';
buf[0] = 'Z';
input[0] = 'Z';
	selectionMenu();
}


//balance while loop
	while(atoi(&input[0]) == 1) {
					//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}

//while the selection input is not valid
while(1){	
	whatAccounts(clientAc1, clientAc2, clientAc3);
	//If savings is selected - continue
	if(atoi(&ans[0]) == 1){
	break;
	}
	//If credit is selected and the account exists - continue
	if(atoi(&ans[0]) == 2 && (account2 == true || account3 == true)){
	break;
	}
	if(atoi(&ans[0]) == 3 && account2 == true && account3 == true){
	break;
	}	
		//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	selectionMenu();
	break;
}	else{
	printf("\nInvalid Option - Select Again");
	}
	}

	if(atoi(&ans[0]) == 1 && account1 == true){

printf(" Savings Balance Selected - > Send savings Balance variable to server \n");
write(sockfd , "BALANCE SAVINGS" , strlen("BALANCE SAVINGS")+1);
        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }

       // puts("Server reply :");
	printf("===================================================================\n\n\n");
        puts(server_reply);
printf("\n\n\n===================================================================\n\n\n");
	selection = true;
break;
}
if(atoi(&ans[0]) == 2 && account2 == false && account3 == true ){

printf(" Credit Balance Selected - > Send credit Balance variable to server \n");
write(sockfd , "BALANCE CREDIT" , strlen("BALANCE CREDIT")+1);
        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }

       // puts("Server reply :");
	printf("===================================================================\n\n\n");
        puts(server_reply);
printf("\n\n\n===================================================================\n\n\n");
	selection = true;
break;
}


if(atoi(&ans[0]) == 2 && account2 == true ){

printf(" Credit Balance Selected - > Send credit Balance variable to server \n");
write(sockfd , "BALANCE LOAN" , strlen("BALANCE LOAN")+1);
        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }

       // puts("Server reply :");
	printf("===================================================================\n\n\n");
        puts(server_reply);
printf("\n\n\n===================================================================\n\n\n");
	selection = true;
break;
}

if(atoi(&ans[0]) == 3 && account3 == true){

printf(" Credit Balance Selected - > Send credit Balance variable to server \n");
write(sockfd , "BALANCE CREDIT" , strlen("BALANCE CREDIT")+1);
        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }

       // puts("Server reply :");
	printf("===================================================================\n\n\n");
        puts(server_reply);
printf("\n\n\n===================================================================\n\n\n");
	selection = true;
break;
}



}


//withdraw while loop
while(atoi(&input[0]) == 2) {
					//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}
//while the selection input is not valid
while(1){
	whatAccounts(clientAc1, clientAc2, clientAc3);
	//If savings is selected - continue
	if(atoi(&ans[0]) == 1){
	break;
	}
	//If credit is selected and the account exists - continue
	if(atoi(&ans[0]) == 2 && account3 == true){
	break;
	}else{
	printf("\nInvalid Option - Select Again");
	}
		//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
		selectionMenu();
	break;
}

}

	if(atoi(&ans[0]) == 1){
	char* withdrawal;
	printf(" Savings Balance Selected - > Send savings Withdrawal variable to server \n");
	printf("\n\nHow much would you like to WITHDRAW: ");
	scanf("%s" ,message);
	sprintf(buf, "WITHDRAWSAV %s", message);
	write(sockfd , buf , strlen(buf)+1);
	        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
       // puts("Server reply :");
	printf("===================================================================\n\n\n");
        puts(server_reply);
printf("\n\n\n===================================================================\n\n\n");
	selection = true;
break;

}
	if(atoi(&ans[0]) == 2){
	char* withdrawal;
	printf(" Credit Balance Selected - > Send savings Withdrawal variable to server \n");
	printf("\n\nHow much would you like to WITHDRAW: ");
	scanf("%s" ,message);
	sprintf(buf, "WITHDRAWCREDIT %s", message);
	write(sockfd , buf , strlen(buf)+1);
	        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
       // puts("Server reply :");
	printf("===================================================================\n\n\n");
        puts(server_reply);
	printf("\n\n\n===================================================================\n\n\n");
	selection = true;
break;

}
}



while(atoi(&input[0]) == 3) {
printf("\nThe Maximum Daily Limit is $1000.00\n");
//handle the selection input
while(1){	
	whatAccounts(clientAc1, clientAc2, clientAc3);
	//If savings is selected - continue
	if(atoi(&ans[0]) == 1){
	break;
	}
	//If credit is selected and the account exists - continue
	if(atoi(&ans[0]) == 2 && (account2 == true || account3 == true)){
	break;
	}
	if(atoi(&ans[0]) == 3 && account2 == true && account3 == true){
	break;
	}	
		//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	selectionMenu();
	break;
}	else{
	printf("\nInvalid Option - Select Again");
	}
	}


	if(atoi(&ans[0]) == 1){
	char* deposit;
	printf(" Savings Balance Selected - > Send savings Credit variable to server \n");
	printf("\n\nHow much would you like to DEPOSIT: ");
	//fgets(message,1000,stdin);
	scanf("%s" ,message);
	//fgets(message,1000,stdin);
	//sscanf(message, "%s" , withdrawal);
	sprintf(buf, "DEPOSITSAV %s", message);
	write(sockfd , buf , strlen(buf)+1);
	        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
       // puts("Server reply :");
	printf("===================================================================\n\n\n");
        puts(server_reply);
	while(strcmp(server_reply, "You cannot deposit more than $1000.00 in a single transaction!") == 0){
			printf("\nEnter the amount to deposit (E/e) to exit : $");
			scanf("%s" ,message);
	//fgets(message,1000,stdin);
	//sscanf(message, "%s" , withdrawal);
	sprintf(buf, "DEPOSITSAV %s", message);
	write(sockfd , buf , strlen(buf)+1);
	        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
	printf("\n\n\n===================================================================\n\n\n");
	puts(server_reply);
	}
printf("\n\n\n===================================================================\n\n\n");
	selection = true;
break;

}

if(atoi(&ans[0]) == 2 && account2 == false && account3 == true){
	char* deposit;
	printf(" Loan Balance Selected - > Send Loan variable to server \n");
	printf("\n\nHow much would you like to DEPOSIT: ");
	//fgets(message,1000,stdin);
	scanf("%s" ,message);
	//fgets(message,1000,stdin);
	//sscanf(message, "%s" , withdrawal);
	sprintf(buf, "DEPOSITCREDIT %s", message);
	write(sockfd , buf , strlen(buf)+1);
	        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
       // puts("Server reply :");
	printf("===================================================================\n\n\n");
        puts(server_reply);
while(strcmp(server_reply, "You cannot deposit more than $1000.00 in a single transaction!") == 0){
			printf("\nEnter the amount to deposit (E/e) to exit : $");
			scanf("%s" ,message);
	//fgets(message,1000,stdin);
	//sscanf(message, "%s" , withdrawal);
	sprintf(buf, "DEPOSITCREDIT %s", message);
	write(sockfd , buf , strlen(buf)+1);
	        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
	printf("\n\n\n===================================================================\n\n\n");
	puts(server_reply);
	}
printf("\n\n\n===================================================================\n\n\n");
	selection = true;
break;

}

if(atoi(&ans[0]) == 2 && account2 == true){
	char* deposit;
	printf(" Loan Balance Selected - > Send Loan variable to server \n");
	printf("\n\nHow much would you like to DEPOSIT: ");
	//fgets(message,1000,stdin);
	scanf("%s" ,message);
	//fgets(message,1000,stdin);
	//sscanf(message, "%s" , withdrawal);
	sprintf(buf, "DEPOSITLOAN %s", message);
	write(sockfd , buf , strlen(buf)+1);
	        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
       // puts("Server reply :");
	printf("===================================================================\n\n\n");
        puts(server_reply);
while(strcmp(server_reply, "You cannot deposit more than $1000.00 in a single transaction!") == 0){
			printf("\nEnter the amount to deposit (E/e) to exit : $");
			scanf("%s" ,message);
	//fgets(message,1000,stdin);
	//sscanf(message, "%s" , withdrawal);
	sprintf(buf, "DEPOSITLOAN %s", message);
	write(sockfd , buf , strlen(buf)+1);
	        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
	printf("\n\n\n===================================================================\n\n\n");
	puts(server_reply);
	}
printf("\n\n\n===================================================================\n\n\n");
	selection = true;
break;

}

if(atoi(&ans[0]) == 3 && account3 == true){
	char* deposit;
	printf(" Credit Balance Selected - > Send  Credit variable to server \n");
	printf("\n\nHow much would you like to DEPOSIT: ");
	//fgets(message,1000,stdin);
	scanf("%s" ,message);
	//fgets(message,1000,stdin);
	//sscanf(message, "%s" , withdrawal);
	sprintf(buf, "DEPOSITCREDIT %s", message);
	write(sockfd , buf , strlen(buf)+1);
	        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
       // puts("Server reply :");
	printf("===================================================================\n\n\n");
        puts(server_reply);
printf("\n\n\n===================================================================\n\n\n");
	selection = true;
break;

}
}




//TRANSFERS////////////////////
///////////////////////////////
//WHILE WE ARE IN TRANSFER MENU
while(atoi(&input[0]) == 4) {
					//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	selection = true;
	break;
}
//while the selection input is not valid
while(1){
	whatAccounts(clientAc1, clientAc2, clientAc3);
	//If savings is selected - continue
	if(atoi(&ans[0]) == 1){
	break;
	}
	//If credit is selected and the account exists - continue
	if(atoi(&ans[0]) == 2 && account3 == true){
	break;
	}else{
	printf("\nInvalid Option - Select Again");
	}
		//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
		//selectionMenu();
	selection = false;
	break;
}
}

	//If the savings account is selected
	if(atoi(&ans[0]) == 1 ){
	//Bring up the menu allowing user to select a transfer option
	transTo(clientAc1, clientAc2, clientAc3);
	
	//If the selection is INVALID Ask again
	while(1){
	//If savings is selected - continue
	if(atoi(&ans2[0]) == 1){
	break;
	}
	//If option2 is selected and client has at least 1 extra account - continue
	if(atoi(&ans2[0]) == 2 && (atoi(clientAc3) > 0 ||atoi(clientAc2) > 0)){
	break;
	}
	//If option3 is selected and client has at least 2 extra account - continue
	if(atoi(&ans2[0]) == 3 && atoi(clientAc2) > 0 && atoi(clientAc3) > 0){
	break;
	}else{
	printf("\nInvalid Option - Select Again");
	//Bring up the menu allowing user to select a transfer option
	transTo(clientAc1, clientAc2, clientAc3);
	}
	//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}
}
	//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}
	//If the menu item 1 is selected
	if(atoi(&ans2[0]) == 1){
	tmenuOne(clientAc2, clientAc3, ans, ans2, message, buf, sockfd);
	
	//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}
	//Send the userinput to server
	write(sockfd, buf, strlen(buf) + 1);
	//Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	//Display message back from server
	puts(server_reply);
	//While the response is invalid	
	while(strcmp("Insufficient Funds - Unable To Process Request", server_reply) == 0){
	tmenuOne( clientAc2, clientAc3, ans, ans2, message, buf, sockfd);
	//Send the userinput to server
	write(sockfd, buf, strlen(buf) + 1);
	//Receive a reply from the server
	        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	//Display message back from server
	puts(server_reply);
		//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}//end invalid response
}
	invalidAccount(server_reply, message, buf, sockfd);
		
}//End If Option item 1

	//If the user selects option 2
	if(atoi(&ans2[0]) == 2){
	tmenuTwo(clientAc2, clientAc3, ans, ans2, message, buf, sockfd);
	
	//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}
	//Send the userinput to server
	write(sockfd, buf, strlen(buf) + 1);
	//Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	//Display message back from server
	puts(server_reply);
	//While the response is invalid	
	while(strcmp("Insufficient Funds - Unable To Process Request", server_reply) == 0){
	tmenuTwo( clientAc2, clientAc3, ans, ans2, message, buf, sockfd);
	//Send the userinput to server
	write(sockfd, buf, strlen(buf) + 1);
	//Receive a reply from the server
	        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	//Display message back from server
	puts(server_reply);
		//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}

}
	invalidAccount(server_reply, message, buf, sockfd);
	

}//End of menu selection 2

//If Menu option three is selected
if(atoi(&ans2[0]) == 3){
//check there is 2 other accounts
tmenuThree(clientAc2, clientAc3, message, buf);
//Send the userinput to server
	write(sockfd, buf, strlen(buf) + 1);
	//Receive a reply from the server
	        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	//Display message back from server
	puts(server_reply);
		//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}
	//if account number is invalid
	invalidAccount(server_reply, message, buf, sockfd);
	//If server replies asking for an amount to transfer to external account num
	if(strcmp("Enter the Amount to Transfer (E/e to exit) - $", server_reply) == 0 ) {
	//save amount to send to buf
	extTranSav(buf, message);
	//Send the userinput to server
	write(sockfd, buf, strlen(buf) + 1);
	//Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	//Display message back from server
	puts(server_reply);
	}

}

}//End If Menu Item 1 ( SAVINGS )

//////////////////////////////////////////////////// TO DO - EVERYTHING FOR MENU OPTION 2 CREDIT /////////////////////////////////


//If the credit option for transfer is selected and they have a credit account
if(atoi(&ans[0]) == 2 && account3 == true)
{
	//Bring up the menu allowing user to select a transfer option
	transTo(clientAc1, clientAc2, clientAc3);
	//If the selection is INVALID Ask again
	while(1){
	//If savings is selected - continue
	if(atoi(&ans2[0]) == 1){
	break;
	}
	//If option2 is selected and client has at least 1 extra account - continue
	if(atoi(&ans2[0]) == 2 && (atoi(clientAc3) > 0 ||atoi(clientAc2) > 0)){
	break;
	}
	//If option3 is selected and client has at least 2 extra account - continue
	if(atoi(&ans2[0]) == 3 && atoi(clientAc2) > 0 && atoi(clientAc3) > 0){
	break;
	}else{
	printf("\nInvalid Option - Select Again");
	//Bring up the menu allowing user to select a transfer option
	transTo(clientAc1, clientAc2, clientAc3);
	}
	//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}
}
	//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}
	//If the first account option is selected
	if(atoi(&ans2[0]) == 1)
	{
			//If e is pressed go back a screen
		if(exitMenu(ans,ans2,message,buf) == true){
			break;
		}
		//Execute method for first option
		tmenuOneC( clientAc2, clientAc1, ans, ans2, message, buf, sockfd);
		//send appropriate action to server
		write(sockfd, buf, strlen(buf)+1);
		//Receive a reply from the server
	        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
		//Display message back from server
		puts(server_reply);	
	}
	//If the server comes back with insifficent funds
	//While the response is invalid	
	while(strcmp("Insufficient Funds - Unable To Process Request", server_reply) == 0){
	tmenuOneC( clientAc2, clientAc1, ans, ans2, message, buf, sockfd);
	//Send the userinput to server
	write(sockfd, buf, strlen(buf) + 1);
	//Receive a reply from the server
	  if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
	//Display message back from server
	puts(server_reply);
		//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}

}// end option 1

	//If option 2 inside CREDIT transfer is selected 
	if(atoi(&ans2[0]) == 2 && atoi(&ans[0]) == 2){
	//while in option 2 if e is pressed go back to menu
		if(exitMenu(ans,ans2,message,buf) == true){
			break;
		}
	//Execute method for Option 2 - Transfer from Credit
	tmenuTwoC(clientAc2, clientAc1, ans, ans2, message, buf, sockfd);
	//write outcome to server
	write(sockfd, buf, strlen(buf)+1);
	//Receive back from server
	if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
	//Display message back from server
	puts(server_reply);

	//If the server comes back with insifficent funds
	//While the response is invalid	
	while(strcmp("Insufficient Funds - Unable To Process Request", server_reply) == 0){
	tmenuTwoC( clientAc2, clientAc1, ans, ans2, message, buf, sockfd);
	//Send the userinput to server
	write(sockfd, buf, strlen(buf) + 1);
	//Receive a reply from the server
	  if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
	//Display message back from server
	puts(server_reply);
		//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}
}
	//If there is no loan account the option 2 method will have sent the server
	//A external transfer request
		//if account number is invalid
	invalidAccountCred(server_reply, message, buf, sockfd);
	//If server replies asking for an amount to transfer to external account num
	if(strcmp("Enter the Amount to Transfer (E/e to exit) - $", server_reply) == 0 ) {
	//save amount to send to buf
	extTranCred(buf, message);
	//Send the userinput to server
	write(sockfd, buf, strlen(buf) + 1);
	//Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	//Display message back from server
	puts(server_reply);
	}
}//END option 2

	//IF there is an option 3 then the client has +2 accounts
	if(atoi(&ans2[0]) == 3 && atoi(clientAc2) > 0 && atoi(clientAc3) > 0){
	printf("Enter Destination Account Number (E/e  to exit) - ");
	scanf("%s", message);
	sprintf(buf, "EXTERNAL %s", message);
	//Send the userinput to server
	write(sockfd, buf, strlen(buf) + 1);
	//Receive a reply from the server
	        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	//Display message back from server
	puts(server_reply);
		//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}
	//if account number is invalid
	invalidAccountCred(server_reply, message, buf, sockfd);
	//If server replies asking for an amount to transfer to external account num
	if(strcmp("Enter the Amount to Transfer (E/e to exit) - $", server_reply) == 0 ) {
	//save amount to send to buf
	extTranCred(buf, message);
	//Send the userinput to server
	write(sockfd, buf, strlen(buf) + 1);
	//Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	//Display message back from server
	puts(server_reply);
	}

	}//END option 3

}//END Credit transfer

//Transfer Over , Take me to main menu
selection = true;
break;
}//end while











//TRANSACTIONS CODE
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
while(atoi(&input[0]) == 5) {
whatAccounts(clientAc1, clientAc2, clientAc3);
//while the selection input is not valid
while(1){	
	//If savings is selected - continue
	if(atoi(&ans[0]) == 1){
	break;
	}
	//If credit is selected and the account exists - continue
	if(atoi(&ans[0]) == 2 && (account2 == true || account3 == true)){
	break;
	}
	if(atoi(&ans[0]) == 3 && account2 == true && account3 == true){
	break;
	}	
		//If e is pressed go back a screen
	if(exitMenu(ans,ans2,message,buf) == true){
	break;
}	else{
	printf("\nInvalid Option - Select Again");
	whatAccounts(clientAc1, clientAc2, clientAc3);
	}
	}

	if(strcmp(ans,"e") == 0 || strcmp(ans,"E") == 0){
	selection = true;
	break;

}
	if(atoi(&ans[0]) == 1){
	char* deposit;
	printf(" Savings Transactions Selected - > Send savings trans variable to server \n");
	sprintf(buf, "TRANSACTIONS %s", clientAc1);
	write(sockfd , buf , strlen(buf));
	        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	printf("===================================================================\n");

        puts(server_reply);
printf("===================================================================\n");
	selection = true;
break;

}

if(atoi(&ans[0]) == 2 && account2 == false && account3 == true){
	char* deposit;
	printf(" Savings Transactions Selected - > Send savings trans variable to server \n");
	sprintf(buf, "TRANSACTIONS %s", clientAc3);
	write(sockfd , buf , strlen(buf));
	        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	printf("===================================================================\n");

        puts(server_reply);
printf("===================================================================\n");
	selection = true;
break;


}

if(atoi(&ans[0]) == 2 && account2 == true){
	char* deposit;
	printf(" Savings Transactions Selected - > Send savings trans variable to server \n");
	sprintf(buf, "TRANSACTIONS %s", clientAc2);
	write(sockfd , buf , strlen(buf));
	        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	printf("===================================================================\n");

        puts(server_reply);
printf("\n\n\n===================================================================\n");
	selection = true;
break;


}

if(atoi(&ans[0]) == 3 && account3 == true && account2 == true){
	char* deposit;
	printf(" Savings Transactions Selected - > Send savings trans variable to server \n");
	sprintf(buf, "TRANSACTIONS %s", clientAc3);
	write(sockfd , buf , strlen(buf));
	        //Receive a reply from the server
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
	printf("===================================================================\n");
	printf("Transaction \t Type \t       Amount \n");
        puts(server_reply);
printf("\n\n\n===================================================================\n");
	selection = true;
break;



}





}

	if(login == true){
	clrscr();
	introMenu(clientName, clientSurname, clientNum);
	printf("\n");
	selectionMenu();
}
	login = false;
	//memset(&message[0], 0, sizeof(message));
    }

    close(sockfd);
    return 0;
}

