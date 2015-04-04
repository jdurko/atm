//#include "stdafx.h"
#include <stdlib.h>
#include "DataMap.h"
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
#define BACKLOG	10
#define BUF_SIZE	1024
#define LISTEN_PORT	8000
int threadCount = BACKLOG;
void *client_handler(void *arg);
int main(int argc, char *argv[]) {
     int status, *sock_tmp;
    pthread_t a_thread;
    void *thread_result;
    cnp::DWORD dwSequenceNumber;
    struct sockaddr_in addr_mine;
    struct sockaddr_in addr_remote;
    int sock_listen;
    int sock_aClient;
    int addr_size;
    int reuseaddr = 1;


    sock_listen = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_listen < 0) {
        perror("socket() failed");
        exit(0);
    }

    memset(&addr_mine, 0, sizeof (addr_mine));
    addr_mine.sin_family = AF_INET;
    addr_mine.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_mine.sin_port = htons((unsigned short)LISTEN_PORT);

    status = bind(sock_listen, (struct sockaddr *) &addr_mine,
    	sizeof (addr_mine));
    if (status < 0) {
        perror("bind() failed");
        close(sock_listen);
        exit(1);
    }

    status = listen(sock_listen, 5);
    if (status < 0) {
        perror("listen() failed");
        close(sock_listen);
        exit(1);
    }

    addr_size = sizeof(struct sockaddr_in);
    printf("waiting for a client\n");
   //Thread
  while(1) {
   if (threadCount < 1) {
    		sleep(1);
    	}

    	sock_aClient = accept(sock_listen, (struct sockaddr *) &addr_remote,
            (socklen_t*) &addr_size);
    	if (sock_aClient == -1){
    		close(sock_listen);
        	exit(1);
    	}

    	printf("Got a connection from %s on port %d\n",
                    inet_ntoa(addr_remote.sin_addr),
                    htons(addr_remote.sin_port));
    	sock_tmp = (int*)malloc(1);
    	*sock_tmp = sock_aClient;
    	printf("thread count = %d\n", threadCount);
    	threadCount--;
 		status = pthread_create(&a_thread, NULL, client_handler,
            (void *) sock_tmp);
 		if (status != 0) {
 			perror("Thread creation failed");
 			printf("In client_handler ERROR \n");
 			close(sock_listen);
 			close(sock_aClient);
 			free(sock_tmp);
        	exit(1);
 		}
  }
 return 0;


}
void *client_handler(void *sock_desc) {
	int msg_size;
	char buf[BUF_SIZE];
	int text;
    char  delim='%';
    char *selected;
	int sock = *(int*)sock_desc;
	cnp::CONNECT_REQUEST connectreq;
	cnp::DWORD mdhead;
	//cnp::CONNECT_REQUEST connectreq=new cnp::CONNECT_REQUEST(0);
    if(recv(sock,&connectreq, connectreq.get_Size(), 0)<0){
        //ERROR_CONNECT_RESPONSE
    }
    //selected= strtok(buf,delim);
    //while( selected != NULL )
   //{
    //  printf( " %s\n", selected );

   //   connectreq(strtok(NULL, buf));

   //connectreq=buf;
   // cnp::DWORD result= "WELCOME TO ATM: DURKOM: PLEASE READ CAREFULLY AND SELECT FROM 1 TO 8 THE NUMBER CORRESPONDING TO  YOUR CHOICE"//INFO TO SEND
    cnp::DWORD result=0x02;
    cnp::WORD wClientID=connectreq.get_ClientID();
    cnp::WORD wMajorVersion=connectreq.get_ClientMajorVersion();
    cnp::WORD wMinorVersion=connectreq.get_ClientMinorVersion();
    cnp::DWORD dwValidationKey=connectreq.get_ClientValidationKey();
    cnp::DWORD dwSequenceNumber=connectreq.get_Sequence();
    cnp::DWORD dcontext=connectreq.get_Context();
    cnp::CONNECT_RESPONSE connectres(result,wClientID,wMajorVersion,wMinorVersion,dwSequenceNumber,dcontext);
    if (send(sock,&connectres, connectres.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
        }


	printf("In client_handler\n");
	while (1) {
        //Receive a reply from server
        if(recv(sock, &mdhead, sizeof(mdhead), 0)<0){
            //ERRROR MESSAGE
        }
//printf("thread count = %d\n", threadCount);
printf("Print mdhead %lu \n",mdhead);
printf("Print MT_CREATE %d \n",cnp::MT_CREATE_ACCOUNT_REQUEST_ID);

        // selected= strtok(buf,&delim);
         if(mdhead==cnp::MT_CREATE_ACCOUNT_REQUEST_ID){
            text=1;
         }
         if(mdhead==cnp::MT_LOGON_REQUEST_ID){
            text=2;
         }
         if(mdhead==cnp::MT_LOGOFF_REQUEST_ID){
            text=3;
         }
         if(mdhead==cnp::MT_DEPOSIT_REQUEST_ID){
            text=4;
         }
         if(mdhead==cnp::MT_WITHDRAWAL_REQUEST_ID){
            text=5;
         }
         if(mdhead==cnp::MT_PURCHASE_STAMPS_REQUEST_ID){
            text=6;
         }
         if(mdhead==cnp::MT_BALANCE_QUERY_REQUEST_ID){
            text=7;
         }
         if(mdhead==cnp::MT_TRANSACTION_QUERY_REQUEST_ID){
            text=8;
         }

   // while( selected != NULL )
   //{
    //  printf( " %s\n", selected );

   //   connectreq(strtok(NULL, s));
  // }


		//cnp::CONNECT_REQUEST ServReq;
		//if (recv(sock_recv, ServReq, strlen(ServReq), 0)<0){
		//TODO error CNP_MESSAGE
		//}
		printf("Header\n");// TODO CONNECT_RESPONSE header
		printf("");//TODO CONNECT_RESPONSE body
		//scanf("%s", text); //answer by replying
printf("TEXT %d \n",text);
		switch (text)
		{
		case 1:{  // TODO CREATE_ACCOUNT_RESPONSE
			//cnp::CREATE_ACCOUNT_RESPONSE accountres;
			//cnp::CREATE_ACCOUNT_REQUEST accountreq;
			cnp::CREATE_ACCOUNT_REQUEST accountreq;
			if (recv(sock, &accountreq, accountreq.get_Size(), 0) < 0){
				//ERROR_CONNECT_RESPONSE
			}
			// accountreq::m_Hdr=mdhead.m_Hdr;

			// accountreq::m_Request=mdhead.m_Response;
			//TODO GET ClinetID
			cnp::WORD wClientID = accountreq.get_ClientID();
			const char * szFirstName = accountreq.get_FirstName();// TODO First Name
			const char * szLastName = accountreq.get_LastName();// TODO Last Name
			const char * szEmailAddress = accountreq.get_EmailAddress(); // TODO Email Address
			cnp::WORD wPin = accountreq.get_PIN(); // TODO PIN NUMBER
			cnp::DWORD dwSSN = accountreq.get_SSNumber();// TODO SOCIAL SECURITY
			cnp::DWORD dwDLN = accountreq.get_DLNumber(); // TODO Driver Licence
printf("FirstName %d \n", *szFirstName);
printf("ClientID %d \n",wClientID);
printf("SSN %lu \n",dwSSN);
			//TODO CREATE IN FILE DATABASE
            CustomerRecord custRec1(threadCount,szFirstName , szLastName, szEmailAddress, wPin, dwSSN,dwDLN);//create record
           printf("CUST RECORD %s  \n", custRec1.m_szFirstName);
            char  str[32];
            strcat (str, szFirstName);
            //const char pin[7]=""+wPin;
            printf("Concatenate1 %s \n",str);
           // strcat (str,(const char *)wPin);
          //  printf("Concatenate2 %s \n",str);
            g_mapCustomerRecords.insert(std::pair<DWORD, CustomerRecord> (*str, custRec1) );//insert
     //       open up a file stream and write them out
//    *note*, I would open the file in binary, unless you love parsing text.

    std::ofstream of("CustomerRecords.dat", std::ios_base::binary );

//  iterate over the map

    for (std::map<DWORD, CustomerRecord>::iterator it = g_mapCustomerRecords.begin(); it != g_mapCustomerRecords.end(); ++it)
    {
        of.write(reinterpret_cast<const char*>(& it->second), sizeof(CustomerRecord) );
    }

// close the file stream & clear our in-memory collection of records

    of.close();
    g_mapCustomerRecords.empty();

			cnp::DWORD result;//result
			cnp::DWORD dwSequenceNumber;//
			wClientID=threadCount;
			cnp::CREATE_ACCOUNT_RESPONSE accountres(result, wClientID, dwSequenceNumber, 0);
			if (send(sock, &accountres, accountres.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
		}

			break;
		case 2:{ // TODO LOGON REQUEST
			cnp::LOGON_REQUEST logonreq;
			//cnp::LOGON_RESPONSE logonres;
			// cnp::LOGON_REQUEST logonreq=mdhead;
			//TODO getClientID
			if (recv(sock, &logonreq, logonreq.get_Size(), 0) < 0){
				//ERROR_CONNECT_RESPONSE
			}

			cnp::WORD wClientID = logonreq.get_ClientID();
			const char * szFirstName = logonreq.get_FirstName(); // TODO First Name
			cnp::WORD wPin = logonreq.get_PIN();//TODO PIN
			//SELECT CLIENT IN DATABASE
//  open up the file and start reading them back in

    std::ifstream ifs("CustomerRecords.dat", std::ios_base::binary );

    while (ifs)
    {
        CustomerRecord custRecord;
        //custRecord.m_szFirstName=*szFirstName;
        //custRecord.m_wPIN=wPin;
        char  str[80];
            strcat (str, szFirstName);
           // strcat (str,(const char *)wPin);
        ifs.read(reinterpret_cast<char*>(& custRecord), sizeof(CustomerRecord));
        std::map<DWORD, CustomerRecord>::iterator it;
        if (ifs)
           // g_mapCustomerRecords.insert( std::make_pair(custRecord.m_dwRecordID, custRecord) );
             it = g_mapCustomerRecords.find(*str);
        if (it != g_mapCustomerRecords.end() ){
            printf("Not Found \n");
            break;
        }

            printf("LastName %s \n", it->second.m_szLastName);
        //it->second.m_dwDDL = 99999;

    }
			cnp::DWORD result;//
			cnp::DWORD dwSequenceNumber;
			cnp::LOGON_RESPONSE logonres(result, wClientID, dwSequenceNumber, 0);
			if (send(sock, &logonres, logonres.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
		}
			break;
		case 3:{ //TODO LOGOFF REQUEST
			//cnp::LOGOFF_RESPONSE logoffres;
			cnp::LOGOFF_REQUEST logoffreq;
			if (recv(sock, &logoffreq, logoffreq.get_Size(), 0) < 0){
				//ERROR_CONNECT_RESPONSE
			}
			//GetClientID
			cnp::WORD wClientID = logoffreq.get_ClientID();
			//TERMINATE CONNECTION
			cnp::DWORD result;
			cnp::DWORD dwSequenceNumber;
			cnp::LOGOFF_RESPONSE logoffres(result, wClientID, dwSequenceNumber, 0);
			if (send(sock, &logoffres, logoffres.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
		}
			break;
		case 4:{ //TODO DEPOSIT REQUEST
			cnp::DEPOSIT_REQUEST depositreq;
			//cnp::DEPOSIT_RESPONSE depositres;
			//cnp::DEPOSIT_REQUEST depositreq(strtok(NULL,buf));
			if (recv(sock, &depositreq, depositreq.get_Size(), 0) < 0){
				//ERROR_CONNECT_RESPONSE
			}
			//GetClientID
			cnp::WORD wClientID = depositreq.get_ClientID();
			cnp::DWORD dwAmount = depositreq.get_Amount();
			cnp::WORD wType = depositreq.get_DepositType();
			cnp::DWORD result;//
			cnp::DWORD dwSequenceNumber;//
			cnp::DEPOSIT_RESPONSE depositres(result, wClientID, dwSequenceNumber, 0);
			if (send(sock, &depositres, depositres.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
		}
			break;
		case 5:{ //TODO WITHDRAW REQUEST
			//GETClientID
			cnp::WITHDRAWAL_REQUEST withdrawalreq;
			//cnp::WITHDRAWAL_RESPONSE withdrawalres;
			if (recv(sock, &withdrawalreq, withdrawalreq.get_Size(), 0) < 0){
				//ERROR_CONNECT_RESPONSE
			}
			//cnp::WITHDRAWAL_REQUEST withdrawalreq(strtok(NULL,buf));
			cnp::WORD wClientID = withdrawalreq.get_ClientID();
			cnp::DWORD dwAmount = withdrawalreq.get_Amount();
			cnp::DWORD result;
			cnp::DWORD dwSequenceNumber;
			cnp::WITHDRAWAL_RESPONSE withdrawalres(result, wClientID, dwSequenceNumber, 0);
			if (send(sock, &withdrawalres, withdrawalres.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
		}
			break;
		case 6:{//TODO STAMP PURCHASE REQUEST
			//GetClientID
			cnp::STAMP_PURCHASE_REQUEST stampreq;
			//cnp::STAMP_PURCHASE_RESPONSE stampres;
			//cnp::STAMP_PURCHASE_REQUEST stampreq(strtok(NULL,buf));
			if (recv(sock, &stampreq, stampreq.get_Size(), 0) < 0){
				//ERROR_CONNECT_RESPONSE
			}
			cnp::WORD wClientID = stampreq.get_ClientID();
			cnp::DWORD dwAmount = stampreq.get_Amount();
			cnp::DWORD dwresult;
			cnp::DWORD dwSequenceNumber;
			cnp::STAMP_PURCHASE_RESPONSE stampres(dwresult, wClientID, dwSequenceNumber, 0);
			if (send(sock, &stampres, stampres.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
		}
			break;
		case 7:{//TODO BALANCE REQUEST
			//getClientID
			cnp::BALANCE_QUERY_REQUEST balancereq;
			//cnp::BALANCE_QUERY_RESPONSE balanceres;
			// cnp::BALANCE_QUERY_REQUEST balancereq(strtok(NULL,buf));
			if (recv(sock, &balancereq, balancereq.get_Size(), 0) < 0){
				//ERROR_CONNECT_RESPONSE
			}
			cnp::WORD wClientID = balancereq.get_ClientID();   //getclientID for sending
			cnp::DWORD dwresult;
			cnp::DWORD balance;//show balance
			cnp::DWORD context = balancereq.get_Context();
			cnp::DWORD sequence=balancereq.get_Sequence();
			cnp::BALANCE_QUERY_RESPONSE balanceres(dwresult, wClientID, balance, context, sequence);
			if (send(sock, &balanceres, balanceres.get_Size(), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
		}
            break;
		case 8:{//TODO TRANSACTION REQUEST
			//getClientID
			cnp::TRANSACTION_QUERY_REQUEST transactionreq;
			//cnp::TRANSACTION_QUERY_RESPONSE transactionres;
			//cnp::TRANSACTION_QUERY_REQUEST transactionreq(strtok(NULL,buf));
			if (recv(sock, &transactionreq, transactionreq.get_Size(), 0) < 0){
				//ERROR_CONNECT_RESPONSE
			}
			cnp::DWORD dwStartID = transactionreq.get_StartID();
			cnp::WORD wTransactionCount = transactionreq.get_TransactionCount();
			cnp::DWORD result;
			cnp::DWORD sequence;
			cnp::TRANSACTION_QUERY_RESPONSE transactionres(result, wClientID, wTransactionCount, sequence, 0);
			if (send(sock, &transactionres, sizeof(cnp::TRANSACTION_QUERY_RESPONSE), 0) < 0){
				//TODO error  CNP_ACCONT_MESSAGE
			}
		}
		break;
		default:
            break;

		}
		//bytes_sent = send(sock_send, buf, strlen(buf), 0);
		//if (send(sock_send, buf, send_len, 0) < 0){
			//TODO error  CNP_MESSAGE
		//}
	//}
      //  buf[msg_size] = 0;
       // printf("Received: %s\n", buf);
       // if (strcmp(buf, "shutdown") == 0)
        //    break;

    }

    close(sock);
	free(sock_desc);
	threadCount++;
    // pthread_exit("Thank you for the CPU time");
}
