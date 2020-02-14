//====================================================================================================================//
// File:          can_demo.cpp                                                                                        //
// Description:   CAN bus demo                                                                                        //
//                                                                                                                    //
// Copyright (C) MicroControl GmbH & Co. KG                                                                           //
// 53844 Troisdorf - Germany                                                                                          //
// www.microcontrol.net                                                                                               //
//                                                                                                                    //
//--------------------------------------------------------------------------------------------------------------------//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the   //
// following conditions are met:                                                                                      //
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions, the following   //
//    disclaimer and the referenced file 'LICENSE'.                                                                   //
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the       //
//    following disclaimer in the documentation and/or other materials provided with the distribution.                //
// 3. Neither the name of MicroControl nor the names of its contributors may be used to endorse or promote products   //
//    derived from this software without specific prior written permission.                                           //
//                                                                                                                    //
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance     //
// with the License. You may obtain a copy of the License at                                                          //
//                                                                                                                    //
//    http://www.apache.org/licenses/LICENSE-2.0                                                                      //
//                                                                                                                    //
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed   //
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for  //
// the specific language governing permissions and limitations under the License.                                     //
//                                                                                                                    //
//====================================================================================================================//



/*--------------------------------------------------------------------------------------------------------------------*\
** Include files                                                                                                      **
**                                                                                                                    **
\*--------------------------------------------------------------------------------------------------------------------*/


#include "can_demo.hpp"


#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <QtCore/QTimer>


#ifndef  VERSION_MAJOR
#define  VERSION_MAJOR                       1
#endif

#ifndef  VERSION_MINOR
#define  VERSION_MINOR                       0
#endif

#ifndef  VERSION_BUILD
#define  VERSION_BUILD                       0
#endif



//--------------------------------------------------------------------------------------------------------------------//
// main()                                                                                                             //
//                                                                                                                    //
//--------------------------------------------------------------------------------------------------------------------//
int main(int argc, char *argv[])
{
   QCoreApplication clAppT(argc, argv);
   QCoreApplication::setApplicationName("can-demo");
   
   //---------------------------------------------------------------------------------------------------
   // get application version (defined in .pro file)
   //
   QString clVersionT;
   clVersionT += QString("%1.").arg(VERSION_MAJOR);
   clVersionT += QString("%1.").arg(VERSION_MINOR, 2, 10, QLatin1Char('0'));
   clVersionT += QString("%1,").arg(VERSION_BUILD, 2, 10, QLatin1Char('0'));
   clVersionT += " build on ";
   clVersionT += __DATE__;
   QCoreApplication::setApplicationVersion(clVersionT);


   //---------------------------------------------------------------------------------------------------
   // create the main class
   //
   CanDemo clMainT;

   
   //---------------------------------------------------------------------------------------------------
   // connect the signals
   //
   QObject::connect(&clMainT, SIGNAL(finished()), &clAppT, SLOT(quit()));
   
   //---------------------------------------------------------------------------------------------------
   // This code will start the messaging engine in QT and in 10 sec it will start the execution in the
   // clMainT.quit() routine.
   //
   QTimer::singleShot(10000, &clMainT, SLOT(quit()));
    fprintf(stdout, "Demo will quit in 10 seconds .. \n");
   clAppT.exec();
}


//--------------------------------------------------------------------------------------------------------------------//
// CanDemo()                                                                                                         //
// constructor                                                                                                        //
//--------------------------------------------------------------------------------------------------------------------//
CanDemo::CanDemo(QObject *parent) :
    QObject(parent)
{
   
    //---------------------------------------------------------------------------------------------------
    // connect signals for socket operations
    //
    QObject::connect(&clCanSocketP, &QCanSocket::connected,         this, &CanDemo::socketConnected);

    QObject::connect(&clCanSocketP, &QCanSocket::disconnected,      this, &CanDemo::socketDisconnected);
   
   //QObject::connect(&clCanSocketP, &QCanSocket::error, this, &CanDemo::socketError);
   
    QObject::connect(&clCanSocketP, &QCanSocket::framesReceived,    this, &CanDemo::socketFramesReceived);


   
    //---------------------------------------------------------------------------------------------------
    // connect to CAN interface
    //
    clCanSocketP.connectNetwork(eCAN_CHANNEL_1);
}



//--------------------------------------------------------------------------------------------------------------------//
// CanDemo::quit()                                                                                                   //
// call this routine to quit the application                                                                          //
//--------------------------------------------------------------------------------------------------------------------//
void CanDemo::quit()
{
   clCanSocketP.disconnectNetwork();

   emit finished();
}



//--------------------------------------------------------------------------------------------------------------------//
// QCanDump::sendFrame()                                                                                              //
//                                                                                                                    //
//--------------------------------------------------------------------------------------------------------------------//
void CanDemo::sendFrame(void)
{

    //---------------------------------------------------------------------------------------------------
    // send Standard CAN frame - ID = 0x100, DLC = 2, Data = 0x11 0x22
    //
    fprintf(stdout, "Send Standard CAN frame  \n");
    clCanFrameP.setFrameFormat(QCanFrame::eFORMAT_CAN_STD);
    clCanFrameP.setIdentifier(0x100);
    clCanFrameP.setDlc(2);
    clCanFrameP.setData(0, 0x11);
    clCanFrameP.setData(1, 0x22);

    clCanSocketP.write(clCanFrameP);

    //---------------------------------------------------------------------------------------------------
    // send Extended CAN frame - ID = 0x1AC03B21, DLC = 6, Data = 0xE1 0xE2 0xE3  0xE4 0xE5 0xE6
    //
    fprintf(stdout, "Send Extended CAN frame  \n");
    clCanFrameP.setFrameFormat(QCanFrame::eFORMAT_CAN_EXT);
    clCanFrameP.setIdentifier(0x1AC03B21);
    clCanFrameP.setDlc(6);
    clCanFrameP.setData(0, 0xE1);
    clCanFrameP.setData(1, 0xE2);
    clCanFrameP.setData(2, 0xE3);
    clCanFrameP.setData(3, 0xE4);
    clCanFrameP.setData(4, 0xE5);
    clCanFrameP.setData(5, 0xE6);
    clCanSocketP.write(clCanFrameP);
}


//--------------------------------------------------------------------------------------------------------------------//
// CanDemo::socketConnected()                                                                                        //
//                                                                                                                    //
//--------------------------------------------------------------------------------------------------------------------//
void CanDemo::socketConnected()
{
   fprintf(stdout, "Connected to network can1 \n");
   sendFrame();
}


//--------------------------------------------------------------------------------------------------------------------//
// CanDemo::socketDisconnected()                                                                                     //
//                                                                                                                    //
//--------------------------------------------------------------------------------------------------------------------//
void CanDemo::socketDisconnected()
{
   qDebug() << "Disconnected from CAN ";
   
}

//--------------------------------------------------------------------------------------------------------------------//
// CanDemo::socketError()                                                                                            //
// Show error message and quit                                                                                        //
//--------------------------------------------------------------------------------------------------------------------//
/*
void CanDemo::socketError(QAbstractSocket::SocketError teSocketErrorV)
{
   Q_UNUSED(teSocketErrorV);  // parameter not used 
   
   //----------------------------------------------------------------
   // show error message in case the connection to the network fails
   //
   fprintf(stderr, "%s %s\n", 
           qPrintable(tr("Failed to connect to CAN interface:")),
           qPrintable(clCanSocketP.errorString()));
   quit();
}
*/

//--------------------------------------------------------------------------------------------------------------------//
// CanDemo::socketError()                                                                                            //
// Show error message and quit                                                                                        //
//--------------------------------------------------------------------------------------------------------------------//
void CanDemo::socketFramesReceived(uint32_t ulFrameCntV)
{
     QCanFrame      clCanFrameT;
   QString        clCanStringT;

   while (ulFrameCntV)
   {
      if (clCanSocketP.read(clCanFrameT) == true)
      {
         clCanStringT = clCanFrameT.toString();
         fprintf(stdout, "%s\n", qPrintable(clCanStringT));
      }
      ulFrameCntV--;

   }
}

