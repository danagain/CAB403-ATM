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

char* whatAccounts(char* ac1, char* ac2, char* ac3){

printf("\n\nSelect account type");
//ACCOUNT 1
if(atoi(ac1) > 0 && atoi(ac1) % 11 == 0){
printf("\n<1> Savings Account");
}
if(atoi(ac1) > 0 && atoi(ac1) % 12 == 0){
printf("\n<1> Loan Account");
}
if(atoi(ac1) > 0 && atoi(ac1) % 13 == 0){
printf("\n<1> Credit Card Account");
}
//ACCOUNT 2
if(atoi(ac2) > 0 && atoi(ac2) % 11 == 0){
printf("\n<2> Savings Account");
}
if(atoi(ac2) > 0 && atoi(ac2) % 12 == 0){
printf("\n<2> Loan Account");
}
if(atoi(ac2) > 0 && atoi(ac2) % 13 == 0){
printf("\n<2> Credit Card Account");
}
//ACCOUNT 3
if(atoi(ac3) > 0 && atoi(ac3) % 11 == 0){
printf("\n<3> Savings Account");
}
if(atoi(ac3) > 0 && atoi(ac3) % 12 == 0){
printf("\n<3> Loan Account");
}
if(atoi(ac3) > 0 && atoi(ac3) % 13 == 0){
printf("\n<3> Credit Card Account");
}
printf("\nEnter your selection (E/e to exit) - ");
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
for(int i = 0; i < fromBuf[14]; i++){
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
	selectionMenu();
}
//        printf("Enter message : ");
       // fgets(message ,1000,stdin);
	//puts(message);
	while(atoi(&input[0]) == 1) { 
	whatAccounts(clientAc1, clientAc2, clientAc3);
	if(atoi(&ans[0]) == 1){

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
}

while(atoi(&input[0]) == 2) {
whatAccounts(clientAc1, clientAc2, clientAc3);
	if(atoi(&ans[0]) == 1){
	char* withdrawal;
	printf(" Savings Balance Selected - > Send savings Withdrawal variable to server \n");
	printf("How much would you like to withdraw : ");
	//fgets(message,1000,stdin);
	scanf("%s" ,message);
	//fgets(message,1000,stdin);
	//sscanf(message, "%s" , withdrawal);
	sprintf(buf, "WITHDRAWSAV %s", message);
	write(sockfd , buf , strlen(buf));
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
whatAccounts(clientAc1, clientAc2, clientAc3);
	if(atoi(&ans[0]) == 1){
	char* deposit;
	printf(" Savings Balance Selected - > Send savings Credit variable to server \n");
	printf("How much would you like to withdraw : ");
	//fgets(message,1000,stdin);
	scanf("%s" ,message);
	//fgets(message,1000,stdin);
	//sscanf(message, "%s" , withdrawal);
	sprintf(buf, "CREDITSAV %s", message);
	write(sockfd , buf , strlen(buf));
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

while(atoi(&input[0]) == 4) {
whatAccounts(clientAc1, clientAc2, clientAc3);
	if(atoi(&ans[0]) == 1){
	char* deposit;
	printf(" Savings Balance Selected - > Send savings Transfer variable to server \n");
	printf("Please enter the account number you wish to transfer too : ");
	//fgets(message,1000,stdin);
	scanf("%s" ,message);
	//fgets(message,1000,stdin);
	//sscanf(message, "%s" , withdrawal);
	sprintf(buf, "TRANSFERN %s", message);
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
	scanf("%s" ,trans);
	sprintf(buf, "AMOUNT %s", trans);
	write(sockfd,buf,strlen(buf)+1);
        if( recv(sockfd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
       }   
	puts(server_reply);
	selection = true;
break;

}
}

 
       // Send some data
      //  if( send(sockfd , message , strlen(message)+1 , 0) < 0)
        //{
          //  puts("Send failed");
            //return 1;
        //}
         

         

        //Receive a reply from the server
       // if( recv(sockfd , server_reply , 2000 , 0) < 0)
        //{
         //   puts("recv failed");
           // break;
        //}
	

         
       // puts("Server reply :");
       // puts(server_reply);

	//if(atoi(&ans[0]) == 1){
	//selectionMenu();
//}


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




