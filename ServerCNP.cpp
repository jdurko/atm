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
#define LISTEN_PORT 8000
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
			cnp::WORD wClientID1 = accountreq.get_ClientID();
			const char * szFirstName1 = accountreq.get_FirstName();// TODO First Name
			const char * szLastName1 = accountreq.get_LastName();// TODO Last Name
			const char * szEmailAddress = accountreq.get_EmailAddress(); // TODO Email Address
			cnp::WORD wPin1 = accountreq.get_PIN(); // TODO PIN NUMBER
			cnp::DWORD dwSSN = accountreq.get_SSNumber();// TODO SOCIAL SECURITY
			cnp::DWORD dwDLN = accountreq.get_DLNumber(); // TODO Driver Licence
printf("FirstName %d \n", *szFirstName1);
printf("ClientID %d \n",wClientID1);
printf("SSN %lu \n",dwSSN);
			//TODO CREATE IN FILE DATABASE
            CustomerRecord custRec1(threadCount,szFirstName1 , szLastName1, szEmailAddress, wPin1, dwSSN,dwDLN);//create record
  //         printf("CUST RECORD %s  \n", custRec1.m_szFirstName);
            char  * str1=new char [32];
            strcat (str1, szFirstName1);
            //const char pin[7]=""+wPin;
    //        printf("Concatenate1 %s \n",str1);
           // strcat (str,(const char *)wPin);
          //  printf("Concatenate2 %s \n",str);
  //         printf("RECORD %s \n",custRec1.m_szFirstName);
            g_mapCustomerRecords.insert(std::pair<DWORD, CustomerRecord> (*str1, custRec1) );//insert

   //          printf("MAP %s \n", g_mapCustomerRecords.find(*str1)->second.m_szFirstName);
     //       open up a file stream and write them out
//    *note*, I would open the file in binary, unless you love parsing text.

std::streambuf *psbuf, *backup;
   std::ofstream filestr;
  filestr.open ("CustomerRecord.txt");
//backup = std::cout.rdbuf();     // back up cout's streambuf
 psbuf = filestr.rdbuf();        // get file's streambuf
  std::cout.rdbuf(psbuf);         // assign streambuf to cout
//  iterate over the map
 //printf("ok \n");

    for (std::map<DWORD, CustomerRecord>::iterator it = g_mapCustomerRecords.begin(); it != g_mapCustomerRecords.end(); ++it)
    {
       std::cout << str1 << " => " << it->second.m_dwRecordID << ","<<it->second.m_szFirstName<<","<<it->second.m_szLastName<<","<<it->second.m_szEmailAddress<<","<<it->second.m_wPIN<<","<<it->second.dw_SSN<<","<<it->second.m_dwDDL<<'\n';

    }

// close the file stream & clear our in-memory collection of records

    filestr.close();
 //   printf("ok 1 \n");
    g_mapCustomerRecords.empty();
//printf("ok 2 \n");

			cnp::DWORD result=cnp::CER_SUCCESS;//result
			cnp::DWORD dwSequenceNumber=accountreq.get_Sequence();//
			wClientID1=threadCount;
			cnp::CREATE_ACCOUNT_RESPONSE accountres(result, wClientID1, dwSequenceNumber, 0);
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

			cnp::WORD wClientID2 = logonreq.get_ClientID();
			const char * szFirstName2 = logonreq.get_FirstName(); // TODO First Name
			cnp::WORD wPin = logonreq.get_PIN();//TODO PIN
			//SELECT CLIENT IN DATABASE
//  open up the file and start reading them back in
 CustomerRecord custRecord;
        //custRecord.m_szFirstName=*szFirstName;
        //custRecord.m_wPIN=wPin;
        char * str=new char[32];
            strcat (str, szFirstName2);
  //           printf( " pointer %d\n", *str );
    std::ifstream ifs ("CustomerRecord.txt", std::ifstream::binary);

  // get pointer to associated buffer object
  std::filebuf* pbuf = ifs.rdbuf();

  // get file size using buffer's members
  std::size_t size = pbuf->pubseekoff (0,ifs.end,ifs.in);
  pbuf->pubseekpos (0,ifs.in);

  // allocate memory to contain file data
  char* buffer=new char[size];

  // get file data
  pbuf->sgetn (buffer,size);

  ifs.close();
 // const char s[2] = "-";
// printf( " STR %s\n", str );
 cnp::DWORD result2=cnp::CER_ERROR;//
char *token1;
//CustomerRecord rectemp();
//char * se=new char[32];
   /* get the first token */
   token1 = strtok(buffer, " => ");

   /* walk through other tokens */
   while( token1 != NULL )
   {
  //    printf( " TOKEN %s\n", token1 );
  //    printf( " TOKEN1 %d\n", *token1 );
       // *se=*token1;
    if (token1!=NULL){
     // if ((int )*str==(int )*token1)
     while (strcmp (str,token1) == 0){
        result2=cnp::CER_SUCCESS;
        break;
     }

   }
      token1 = strtok(NULL, " => ");
  //     printf( " BEFORE %s\n", token1 );
      if (token1!=NULL){
     // if ((int )* str==(int )*token1)
     while (strcmp (str,token1) == 0){
        result2=cnp::CER_SUCCESS;
       break;
     }


   }
   }

//printf( "AFTER TOKEN1 %s\n", token1 );
//printf( "AFTER STR %s\n", str );
free(str);
free(token1);

           // strcat (str,(const char *)wPin);

      //  ifs.read(reinterpret_cast<char*>(& custRecord), sizeof(CustomerRecord));
      //  std::map<DWORD, CustomerRecord>::iterator it;
      //  if (ifs)

           // g_mapCustomerRecords.insert( std::make_pair(custRecord.m_dwRecordID, custRecord) );
        //     it = g_mapCustomerRecords.find(*str);
        //if (it != g_mapCustomerRecords.end() ){
        //    printf("Not Found \n");
        //    break;
       // }

          //  printf("LastName %s \n", it->second.m_szLastName);
        //it->second.m_dwDDL = 99999;

  //  }
            wClientID2=wPin;
			cnp::DWORD dwSequenceNumber2=logonreq.get_Sequence();//;
			cnp::LOGON_RESPONSE logonres(result2, wClientID2, dwSequenceNumber2, 0);
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
			cnp::DWORD result3=cnp::CER_SUCCESS;
			cnp::DWORD dwSequenceNumber3=logoffreq.get_Sequence();
			cnp::LOGOFF_RESPONSE logoffres(result3, wClientID, dwSequenceNumber3, 0);
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
			cnp::WORD wClientID4 = depositreq.get_ClientID();
			cnp::DWORD dwAmount = depositreq.get_Amount();
			cnp::WORD wType = depositreq.get_DepositType();

			std::streambuf *psbuf, *backup;
   std::ofstream filestr;
  filestr.open ("BalanceRecord.txt");
//backup = std::cout.rdbuf();     // back up cout's streambuf
 psbuf = filestr.rdbuf();        // get file's streambuf
  std::cout.rdbuf(psbuf);         // assign streambuf to cout
//  iterate over the map
 //printf("ok \n");
       std::cout << wClientID4 << " => " << dwAmount<< ","<<wType<<'\n';

// close the file stream & clear our in-memory collection of records

    filestr.close();
    printf("ok 1 \n");
			cnp::DWORD result4=cnp::CER_SUCCESS;//
			cnp::DWORD dwSequenceNumber4=depositreq.get_Sequence();//
			cnp::DEPOSIT_RESPONSE depositres(result4, wClientID4, dwSequenceNumber4, 0);
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
			cnp::WORD wClientID5 = withdrawalreq.get_ClientID();
			cnp::DWORD dwAmount = withdrawalreq.get_Amount();
			   char * str=new char[32];
            strcat (str, (const char *)&wClientID5);
           //  printf( " pointer %d\n", *str );
    std::ifstream ifs ("CustomerRecord.txt", std::ifstream::binary);

  // get pointer to associated buffer object
  std::filebuf* pbuf = ifs.rdbuf();

  // get file size using buffer's members
  std::size_t size = pbuf->pubseekoff (0,ifs.end,ifs.in);
  pbuf->pubseekpos (0,ifs.in);

  // allocate memory to contain file data
  char* buffer=new char[size];

  // get file data
  pbuf->sgetn (buffer,size);

  ifs.close();
 // const char s[2] = "-";
 printf( " STR %s\n", str );
 cnp::DWORD result5=cnp::CER_ERROR;//
char *token1;
//CustomerRecord rectemp();
//char * se=new char[32];
   /* get the first token */
   token1 = strtok(buffer, " => ");

   /* walk through other tokens */
   while( token1 != NULL )
   {
      printf( " TOKEN %s\n", token1 );
      printf( " TOKEN1 %d\n", *token1 );
       // *se=*token1;
    if (token1!=NULL){
     // if ((int )*str==(int )*token1)
     while (strcmp (str,token1) == 0){
        result5=cnp::CER_SUCCESS;
        break;
     }

   }
      token1 = strtok(NULL, " => ");
       printf( " BEFORE %s\n", token1 );
      if (token1!=NULL){
     // if ((int )* str==(int )*token1)
     while (strcmp (str,token1) == 0){
        result5=cnp::CER_SUCCESS;
       break;
     }


   }
   }

//printf( "AFTER TOKEN1 %s\n", token1 );
//printf( "AFTER STR %s\n", str );
free(str);
free(token1);

			// result5=cnp::CER_SUCCESS;
			cnp::DWORD dwSequenceNumber5=withdrawalreq.get_Sequence();;
			cnp::WITHDRAWAL_RESPONSE withdrawalres(result5, wClientID5, dwSequenceNumber5, 0);
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
			cnp::WORD wClientID6 = stampreq.get_ClientID();
			cnp::DWORD dwAmount = stampreq.get_Amount();
			cnp::DWORD dwresult6=cnp::CER_SUCCESS;
			cnp::DWORD dwSequenceNumber6;
			cnp::STAMP_PURCHASE_RESPONSE stampres(dwresult6, wClientID6, dwSequenceNumber6, 0);
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
