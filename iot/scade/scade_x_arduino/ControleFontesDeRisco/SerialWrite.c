#ifndef _KCG_IMPORTED_FUNCTIONS_H_
#define _KCG_IMPORTED_FUNCTIONS_H_
#include "kcg_types.h"

#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <wchar.h>

int inicializado = 0;
HANDLE hComm;

void inicializar(){

    // Handle to the Serial port
	LPCSTR  ComPortName = "COM4";  // Name of the Serial port(May Change) to be opened,
	BOOL  Status;                          // Status of the various operations
    // Bytes read by ReadFile()
	int i = 0;

	hComm = CreateFile( ComPortName,                  // Name of the Port to be Opened
                        GENERIC_READ | GENERIC_WRITE, // Read/Write Access
						0,                            // No Sharing, ports cant be shared
						NULL,                         // No Security
					    OPEN_EXISTING,                // Open existing port only
                         FILE_ATTRIBUTE_NORMAL,                            // Non Overlapped I/O
                        NULL);                        // Null for Comm Devices

	if (hComm == INVALID_HANDLE_VALUE)
		printf("\n    Error! - Port %s can't be opened\n", ComPortName);
	else
		printf("Port opened\n");//std::wcout <<"\n    Port " << ComPortName << " Opened\n ";

	/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/

	DCB dcbSerialParams = { 0 };                         // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);      //retrieves  the current settings

	if (Status == FALSE)
		printf("\n    Error! in GetCommState()");

	dcbSerialParams.BaudRate = CBR_9600;      // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;        // Setting Parity = None

	Status = SetCommState(hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB

	if (Status == FALSE)
		{
			printf("\n    Error! in Setting DCB Structure");
		}
	else //If Successfull display the contents of the DCB Structure
		{
			printf("\n\n    Setting DCB Structure Successfull\n");
			printf("\n       Baudrate = %d", dcbSerialParams.BaudRate);
			printf("\n       ByteSize = %d", dcbSerialParams.ByteSize);
			printf("\n       StopBits = %d", dcbSerialParams.StopBits);
			printf("\n       Parity   = %d", dcbSerialParams.Parity);
		}

	/*------------------------------------ Setting Timeouts --------------------------------------------------*/

	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout         = 50;
	timeouts.ReadTotalTimeoutConstant    = 50;
	timeouts.ReadTotalTimeoutMultiplier  = 10;
	timeouts.WriteTotalTimeoutConstant   = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hComm, &timeouts) == FALSE)
		printf("\n\n    Error! in Setting Time Outs");
	else
		printf("\n\n    Setting Serial Port Timeouts Successfull");

	/*------------------------------------ Setting Receive Mask ----------------------------------------------*/

	Status = SetCommMask(hComm, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

	if (Status == FALSE)
		printf("\n\n    Error! in Setting CommMask");
	else
		printf("\n\n    Setting CommMask successfull");
}

void writeCharacter(int sinal){
    DWORD dwEventMask;                     // Event mask to trigger
    char  TempChar;                        // Temperory Character
    //WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received
	//printf("\n\n    Characters Received");
	DWORD NoBytesRead;
	    // Send specified text (remaining command line arguments)
    char bytes_to_send[1];
    //bytes_to_send[0] = 31;
    bytes_to_send[0] = (byte) sinal;
    DWORD bytes_written, total_bytes_written = 0;
    printf("Sending bytes...");

    if(!WriteFile(hComm, bytes_to_send, 1, &bytes_written, NULL))
    {
        fprintf(stderr, "Error\n");
        CloseHandle(hComm);
        return;
    }

    printf("%d bytes written\n", bytes_written);



}

void writeSignal(int sinal){
    if(inicializado == 0){
        inicializar();
        inicializado = 1;
    }

    writeCharacter(sinal);

    return;
}

int readSerial()
{
    if(inicializado == 0){
        inicializar();
        inicializado = 1;
    }

   /*------------------------------------ Setting WaitComm() Event   ----------------------------------------*/


	/*-------------------------- Program will Wait here till a Character is received ------------------------*/


            DWORD dwEventMask;                     // Event mask to trigger
           // WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received
			printf("\n\n    Characters Received");
			char  TempChar;                        // Temperory Character
	//char  SerialBuffer[100000];               // Buffer Containing Rxed Data
	DWORD NoBytesRead;

            //Status =
            ReadFile(hComm, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);


			if(NoBytesRead>0){
				printf("%d", TempChar);
			}

			return (int) TempChar;

			/*------------Printing the RXed String to Console----------------------*/
}//End of Main()

#ifndef SerialWrite
/* SerialWrite */
 kcg_int SerialWrite(/* SerialWrite::Input1 */ kcg_int Input1){
    writeSignal((int) Input1);

    return Input1;
 }
#endif /* SerialWrite */
#endif
