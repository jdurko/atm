
#include <stdlib.h>
#include <stdio.h>
#include "CNP_Protocol.h"
#include <map>
#include <utility>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define BUF_SIZE	1024
// #define	SERVER_IP	"129.120.151.98" // cse05    129.120.151.99
//#define SERVER_IP   "129.120.151.94" // cse05
#define SERVER_IP   "129.120.151.99"
#define SERVER_PORT	8000

int main(int argc, char *argv[]) {
int sock_send;
    struct sockaddr_in	addr_send;
    int	i,text;
    int	send_len,bytes_sent;
    char  buf[BUF_SIZE];
    const char delim='%';
	const char *selected;

    // Client Connect Message protocol
    cnp::CONNECT_REQUEST  connectServReq;

/* create socket for sending data */
    sock_send=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_send < 0) {
        printf("socket() failed\n");
        exit(0);
    }
/* create socket address structure to connect to */
    memset(&addr_send, 0, sizeof (addr_send)); /* zero out structure */
    addr_send.sin_family = AF_INET; /* address family */
    addr_send.sin_addr.s_addr = inet_addr(SERVER_IP);
    addr_send.sin_port = htons((unsigned short)SERVER_PORT);

/* connect to the server socket */
    i=connect(sock_send, (struct sockaddr *) &addr_send, sizeof (addr_send));
    if (i < 0) {
        printf("connect() failed\n");
	close(sock_send);
        exit(0);
    }
	//send a connection request
	bytes_sent = send(sock_send, &connectServReq, connectServReq.get_Size(), 0);
	cnp::CONNECT_RESPONSE connectres;
	if(recv(sock_send,&connectres, connectres.get_Size(), 0)<0){
        //ERROR_CONNECT_RESPONSE
    }
   // printf(" response %lu\n", connectres.get_ResponseResult());
    //cnp::CONNECT_REQUEST connectreq;
    //selected= strtok(buf,delim);
   // while( selected != NULL )
   //{
   //   printf( " %s\n", selected );

     // connectreq(strtok(NULL, buf));
  // }
  bool check=true;
  while(check==true){ //keep communicating wuth server
printf("-----\n");// TODO CONNECT_RESPONSE header

		printf("1 -> CREATE AN ACCOUNT\n");
		printf("2 -> LOGIN INTO YOUR ACCOUNT\n");
        scanf("%d", &text);
		switch (text){
		case 1:{  // TODO CREATE_ACCOUNT_REQUEST
			//cnp::CREATE_ACCOUNT_REQUEST accountreq;
			//Receive a reply from server
			cnp::CREATE_ACCOUNT_RESPONSE accountRes;
			//TODO GET ClinetID
			cnp::WORD wClientID=connectres.get_ClientID();
            char * szFirstName=new char[9];
			printf("Enter First Name\n");
			scanf("%s",  szFirstName) ; // TODO First Name
            char * szLastName=new char[12];
			printf("Enter Last Name\n");
			scanf("%s",  szLastName); // TODO Last Name
            char * szEmailAddress=new char[15];
			printf("Enter Email\n");
			scanf("%s",  szEmailAddress); // TODO Email Address
			cnp::WORD wPin;
			printf("Enter PIN\n");
			scanf("%hu", &wPin); // TODO PIN NUMBER
			cnp::DWORD dwSSN;
			printf("Enter #SSN\n");
			scanf("%lu", &dwSSN); // TODO SOCIAL SECURITY
			cnp::DWORD dwDLN;
			printf("Enter #Driver Licence\n");
			scanf("%lu", &dwDLN); // TODO Driver Licence
			cnp::CREATE_ACCOUNT_REQUEST accountreq(wClientID,szFirstName,szLastName,szEmailAddress,wPin,dwSSN,dwDLN,0);
			cnp::DWORD msgtype1=accountreq.get_MsgType();
			if (send(sock_send, &msgtype1, sizeof(msgtype1), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (send(sock_send, &accountreq, accountreq.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (recv(sock_send, &accountRes, accountRes.get_Size(), 0)<0){
				//TODO error CNP_MESSAGE
			}
			//printf("Response SUCCESS:\n", accountRes.get_ResponseResult());
			//check=false;
			if (accountRes.get_ResponseResult()==cnp::CER_SUCCESS){
			check=false;
			printf("Response SUCCESS:%lu\n", accountRes.get_ResponseResult());
			connectres.m_Hdr.m_wClientID=accountRes.m_Hdr.get_ClientID();
			}else{
			printf("Response ERROR:%lu\n", accountRes.get_ResponseResult());
			}
		}

			break;
		case 2:{ // TODO LOGON REQUEST
		    //TODO getClientID
		    cnp::WORD wClientID2=connectres.get_ClientID();
			//cnp::LOGON_REQUEST logonreq;
			//Receive a reply from server
			cnp::LOGON_RESPONSE logonres;
            char * szFirstName2=new char[9];
			printf("Enter First Name\n");
			scanf("%s",  szFirstName2); // TODO First Name
			cnp::WORD wPin2;
			printf("Enter PIN number\n");
			scanf("%hu",  &wPin2); //TODO PIN
			cnp::LOGON_REQUEST logonreq(wClientID2,szFirstName2,wPin2,0);
			cnp::DWORD msg2=logonreq.get_MsgType();
			if (send(sock_send, &msg2, sizeof(msg2), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (send(sock_send, &logonreq, logonreq.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (recv(sock_send, &logonres,logonres.get_Size(), 0)<0){
				//TODO error CNP_MESSAGE
			}

			if (logonres.get_ResponseResult()==cnp::CER_SUCCESS){
			check=false;
			printf("Response SUCCESS:%lu\n", logonres.get_ResponseResult());
			connectres.m_Hdr.m_wClientID=logonres.m_Hdr.get_ClientID();
			}else{
			printf("Response ERROR:%lu\n", logonres.get_ResponseResult());
			}
		}
			break;
        default:
            break;
		}
  }
	// keep communicationg with the server

	while (1){

		printf("-----\n");// TODO CONNECT_RESPONSE header
		//printf("1 -> CREATE AN ACCOUNT\n");
		//printf("2 -> LOGIN INTO YOUR ACCOUNT\n");
		printf("3 -> LOG OFF\n");
		printf("4 -> REQUEST A DEPOSIT\n");
		printf("5 -> WITHDRAW FROM YOUR ACCOUNT\n");
		printf("6 -> PURCHASE STAMPS\n");
		printf("7 -> REQUEST A BALANCE\n");
		printf("8 -> REQUEST A TRANSACTION\n");
		printf("---------------------\n");//TODO CONNECT_RESPONSE body
		scanf("%d", &text); //answer by replying
	//	printf("---TEXT %d \n",text);
		switch (text){
	/*	case 1:{  // TODO CREATE_ACCOUNT_REQUEST
			//cnp::CREATE_ACCOUNT_REQUEST accountreq;
			//Receive a reply from server
			cnp::CREATE_ACCOUNT_RESPONSE accountRes;
			//TODO GET ClinetID
			cnp::WORD wClientID=connectres.get_ClientID();
            char * szFirstName=new char[9];
			printf("Enter First Name\n");
			scanf("%s",  szFirstName) ; // TODO First Name
            char * szLastName=new char[12];
			printf("Enter Last Name\n");
			scanf("%s",  szLastName); // TODO Last Name
            char * szEmailAddress=new char[15];
			printf("Enter Email\n");
			scanf("%s",  szEmailAddress); // TODO Email Address
			cnp::WORD wPin;
			printf("Enter PIN\n");
			scanf("%hu", &wPin); // TODO PIN NUMBER
			cnp::DWORD dwSSN;
			printf("Enter #SSN\n");
			scanf("%lu", &dwSSN); // TODO SOCIAL SECURITY
			cnp::DWORD dwDLN;
			printf("Enter #Driver Licence\n");
			scanf("%lu", &dwDLN); // TODO Driver Licence
			cnp::CREATE_ACCOUNT_REQUEST accountreq(wClientID,szFirstName,szLastName,szEmailAddress,wPin,dwSSN,dwDLN,0);
			cnp::DWORD msgtype1=accountreq.get_MsgType();
			if (send(sock_send, &msgtype1, sizeof(msgtype1), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (send(sock_send, &accountreq, accountreq.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (recv(sock_send, &accountRes, accountRes.get_Size(), 0)<0){
				//TODO error CNP_MESSAGE
			}
			printf("Response:\n", accountRes.get_ResponseResult());
		}

			break;
		case 2:{ // TODO LOGON REQUEST
		    //TODO getClientID
		    cnp::WORD wClientID2=connectres.get_ClientID();
			//cnp::LOGON_REQUEST logonreq;
			//Receive a reply from server
			cnp::LOGON_RESPONSE logonres;
            char * szFirstName2=new char[9];
			printf("Enter First Name");
			scanf("%s",  szFirstName2); // TODO First Name
			cnp::WORD wPin2;
			printf("Enter PIN number");
			scanf("%hu",  &wPin2); //TODO PIN
			cnp::LOGON_REQUEST logonreq(wClientID2,szFirstName2,wPin2,0);
			cnp::DWORD msg2=logonreq.get_MsgType();
			if (send(sock_send, &msg2, sizeof(msg2), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (send(sock_send, &logonreq, logonreq.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (recv(sock_send, &logonres,logonres.get_Size(), 0)<0){
				//TODO error CNP_MESSAGE
			}
			printf("Response:", logonres.get_ResponseResult());
		}
			break;
			*/
		case 3:{ //TODO LOGOFF REQUEST
			//cnp::LOGOFF_REQUEST logoffreq;
			//Receive a reply from server
			cnp::LOGOFF_RESPONSE logoffres;
			//GetClientID
			cnp::WORD wClientID3=connectres.m_Hdr.m_wClientID;
			cnp::LOGOFF_REQUEST logoffreq(wClientID3,0);
			cnp::DWORD msg3=logoffreq.get_MsgType();
			if (send(sock_send, &msg3, sizeof(msg3), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (send(sock_send, &logoffreq, logoffreq.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (recv(sock_send, &logoffres, logoffres.get_Size(), 0)<0){
				//TODO error CNP_MESSAGE
			}
			//printf("Response:", logoffres.get_ResponseResult());
			if (logoffres.get_ResponseResult()==cnp::CER_SUCCESS){
			printf("Response SUCCESS:%lu\n", logoffres.get_ResponseResult());
			bool check1=true;
  while(check1==true){
printf("-----\n");// TODO CONNECT_RESPONSE header

		printf("1 -> CREATE AN ACCOUNT\n");
		printf("2 -> LOGIN INTO YOUR ACCOUNT\n");
        scanf("%d", &text);
		switch (text){
		case 1:{  // TODO CREATE_ACCOUNT_REQUEST
			//cnp::CREATE_ACCOUNT_REQUEST accountreq;
			//Receive a reply from server
			cnp::CREATE_ACCOUNT_RESPONSE accountRes;
			//TODO GET ClinetID
			cnp::WORD wClientID=connectres.m_Hdr.m_wClientID;
            char * szFirstName=new char[9];
			printf("Enter First Name\n");
			scanf("%s",  szFirstName) ; // TODO First Name
            char * szLastName=new char[12];
			printf("Enter Last Name\n");
			scanf("%s",  szLastName); // TODO Last Name
            char * szEmailAddress=new char[15];
			printf("Enter Email\n");
			scanf("%s",  szEmailAddress); // TODO Email Address
			cnp::WORD wPin;
			printf("Enter PIN\n");
			scanf("%hu", &wPin); // TODO PIN NUMBER
			cnp::DWORD dwSSN;
			printf("Enter #SSN\n");
			scanf("%lu", &dwSSN); // TODO SOCIAL SECURITY
			cnp::DWORD dwDLN;
			printf("Enter #Driver Licence\n");
			scanf("%lu", &dwDLN); // TODO Driver Licence
			cnp::CREATE_ACCOUNT_REQUEST accountreq(wClientID,szFirstName,szLastName,szEmailAddress,wPin,dwSSN,dwDLN,0);
			cnp::DWORD msgtype1=accountreq.get_MsgType();
			if (send(sock_send, &msgtype1, sizeof(msgtype1), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (send(sock_send, &accountreq, accountreq.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (recv(sock_send, &accountRes, accountRes.get_Size(), 0)<0){
				//TODO error CNP_MESSAGE
			}
			//printf("Response SUCCESS:\n", accountRes.get_ResponseResult());
			//check=false;
			if (accountRes.get_ResponseResult()==cnp::CER_SUCCESS){
			check1=false;
			printf("Response SUCCESS:%lu\n", accountRes.get_ResponseResult());
			}else{
			printf("Response ERROR:%lu\n", accountRes.get_ResponseResult());
			}
		}

			break;
		case 2:{ // TODO LOGON REQUEST
		    //TODO getClientID
		    cnp::WORD wClientID2=connectres.get_ClientID();
			//cnp::LOGON_REQUEST logonreq;
			//Receive a reply from server
			cnp::LOGON_RESPONSE logonres;
            char * szFirstName2=new char[9];
			printf("Enter First Name\n");
			scanf("%s",  szFirstName2); // TODO First Name
			cnp::WORD wPin2;
			printf("Enter PIN number\n");
			scanf("%hu",  &wPin2); //TODO PIN
			cnp::LOGON_REQUEST logonreq(wClientID2,szFirstName2,wPin2,0);
			cnp::DWORD msg2=logonreq.get_MsgType();
			if (send(sock_send, &msg2, sizeof(msg2), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (send(sock_send, &logonreq, logonreq.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (recv(sock_send, &logonres,logonres.get_Size(), 0)<0){
				//TODO error CNP_MESSAGE
			}

			if (logonres.get_ResponseResult()==cnp::CER_SUCCESS){
			check1=false;
			printf("Response SUCCESS:%lu\n", logonres.get_ResponseResult());
			}else{
			printf("Response ERROR:%lu\n", logonres.get_ResponseResult());
			}
		}
			break;
        default:
            break;
		}
  }
			}else{
			printf("Response ERROR:%lu\n", logoffres.get_ResponseResult());
			}
		}
			break;
		case 4:{ //TODO DEPOSIT REQUEST
		    //GetClientID
		    cnp::WORD wClientID4=connectres.get_ClientID();
			//cnp::DEPOSIT_REQUEST depositreq;
			//Receive a reply from server
			cnp::DEPOSIT_RESPONSE depositres;
			cnp::DWORD dwAmount;
			printf("Enter Amount\n");
			scanf("%lu",  &dwAmount);
			cnp::DEPOSIT_TYPE wType;
			printf("Enter cash or  check\n");
            char choice[5];
			scanf("%s", choice);
			if(choice=="cash"){
                    wType=cnp::DT_CASH;
			}else{
			    wType=cnp::DT_CHECK;
			}
			//scanf("%s", &wType);
			//if(wType==)
			cnp::DEPOSIT_REQUEST depositreq(wClientID4,dwAmount,wType,0 );
			cnp::DWORD msg4=depositreq.get_MsgType();
			if (send(sock_send, &msg4, sizeof(msg4), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (send(sock_send, &depositreq, depositreq.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}

			if (recv(sock_send, &depositres, depositres.get_Size(), 0)<0){
				//TODO error CNP_MESSAGE
			}
			printf("Response:", depositres.get_ResponseResult());
			if (depositres.get_ResponseResult()==cnp::CER_SUCCESS){
			printf("Response SUCCESS:%lu\n", depositres.get_ResponseResult());
			}else{
			printf("Response ERROR:%lu\n", depositres.get_ResponseResult());
			}
		}
			break;
		case 5:{ //TODO WITHDRAW REQUEST
		    //GETClientID
			//cnp::WITHDRAWAL_REQUEST withdrawalreq;
			//Receive a reply from server
			cnp::WITHDRAWAL_RESPONSE withdrawalres;
			cnp::WORD wClientID5=connectres.get_ClientID();
			cnp::DWORD dwAmount2;
			printf("Enter withdraw amount\n");
			scanf("%lu", &dwAmount2);
			cnp::WITHDRAWAL_REQUEST withdrawalreq(wClientID5,dwAmount2,0);
			cnp::DWORD msg5=withdrawalreq.get_MsgType();
			if (send(sock_send, &msg5, sizeof(msg5), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (send(sock_send, &withdrawalreq, withdrawalreq.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (recv(sock_send, &withdrawalres, withdrawalres.get_Size(), 0)<0){
				//TODO error CNP_MESSAGE
			}
			printf("Response:", withdrawalres.get_ResponseResult());
			if (withdrawalres.get_ResponseResult()==cnp::CER_SUCCESS){
			printf("Response SUCCESS:%lu\n", withdrawalres.get_ResponseResult());
			}else{
			printf("Response ERROR:%lu\n", withdrawalres.get_ResponseResult());
			}
		}
			break;
		case 6:{//TODO STAMP PURCHASE REQUEST
		    //GetClientID
		    cnp::WORD wClientID6=connectres.get_ClientID();
			//cnp::STAMP_PURCHASE_REQUEST stampreq;
			//Receive a reply from server
			cnp::STAMP_PURCHASE_RESPONSE stampres;
			cnp::DWORD dwAmount3;
			printf("Enter number of stamps\n");
			scanf("%lu", &dwAmount3);
			cnp::STAMP_PURCHASE_REQUEST stampreq(wClientID6,dwAmount3,0);
			cnp::DWORD msg6=stampreq.get_MsgType();
			if (send(sock_send, &msg6, sizeof(msg6), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (send(sock_send, &stampreq, stampreq.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (recv(sock_send, &stampres, stampres.get_Size(), 0)<0){
				//TODO error CNP_MESSAGE
			}
			printf("Response:", stampres.get_ResponseResult());
		}
			break;
		case 7:{//TODO BALANCE REQUEST
		    //getClientID
		   // cnp::BALANCE_QUERY_REQUEST balancereq;
			//Receive a reply from server
			cnp::BALANCE_QUERY_RESPONSE balanceres;
		    printf(" Balance request processing...\n");
		    cnp::WORD wClientID7= connectres.get_ClientID();//getclientID for sending
		     cnp::BALANCE_QUERY_REQUEST balancereq(wClientID7,0);
		     cnp::DWORD msg7=balancereq.get_MsgType();
		     if (send(sock_send, &msg7, sizeof(msg7), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
            if (send(sock_send, &balancereq, balancereq.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			if (recv(sock_send, &balanceres,balanceres.get_Size(), 0)<0){
				//TODO error CNP_MESSAGE
			}
			printf("Response:", balanceres.get_ResponseResult());
		}
            break;
        case 8:{//TODO TRANSACTION REQUEST
            //getClientID
            cnp::WORD wClientID8=connectres.get_ClientID();
           // cnp::TRANSACTION_QUERY_REQUEST transactionreq;
			//Receive a reply from server
			cnp::TRANSACTION_QUERY_RESPONSE transactionres;
            cnp::DWORD dwStartID2;
            printf("Start ID\n");
            scanf("%lu",&dwStartID2);
            cnp::WORD wTransactionCount;
            printf("number of transactions\n");
            scanf("%hu",&wTransactionCount);
            cnp::TRANSACTION_QUERY_REQUEST transactionreq(wClientID8,dwStartID2,wTransactionCount,0);
            cnp::DWORD msg8=transactionreq.get_MsgType();
              if (send(sock_send, &msg8, sizeof(msg8), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
             if (send(sock_send, &transactionreq, transactionreq.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
			 if (recv(sock_send, &transactionres, sizeof(transactionres), 0)<0){
				 //TODO error CNP_MESSAGE
			 }
			 printf("Response:", transactionres.get_ResponseResult());
        }
            break;
        default:
            break;

		}
		//bytes_sent = send(sock_send, buf, strlen(buf), 0);
		//if (send(sock_send, buf, send_len, 0) < 0){
			//TODO error  CNP_MESSAGE
		//}
	}


}
