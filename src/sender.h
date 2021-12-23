//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __MINIPROJECT_SENDER_H_
#define __MINIPROJECT_SENDER_H_

#include <omnetpp.h>
#include <bits/stdc++.h>
#include "MessageM_m.h"
using namespace omnetpp;
using namespace std;

/**
 * TODO - Generated class
 */
class Sender : public cSimpleModule
{
  cMessage* timeoutChecker;
  string errorString="";
  int sequenceNumber = 0;
  double numberOfTransmissions = 0;
  double numberOfDataTransmissions = 0;
  deque<string> plainMesseages;
  void readFile(string fileName);
  void extractErrorBytes(MessageM_Base* message);
  void byteStuffing(MessageM_Base* message);
  void addHeader(MessageM_Base * message, int id, int type, double sending_time);
  void applyModeification(MessageM_Base* message);
  MessageM_Base * prepareMessage(string message,int id);
  void addTrailer(MessageM_Base * message);
  void applyErrorDelayAndSend(MessageM_Base * message);
  void logEvent(MessageM_Base* message,double time,string type);
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

#endif
