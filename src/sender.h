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
using namespace omnetpp;
using namespace std;

/**
 * TODO - Generated class
 */
class Sender : public cSimpleModule
{
  string errorByte;
  string messageBody;
  int currentTime;
  vector<string> messeages;
  void readFile(string fileName);
  cMessage * byteStuffing(string message);
  cMessage * addHeader(string message, int id, int type,int sending_time);
  cMessage * modeification(string message);

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
