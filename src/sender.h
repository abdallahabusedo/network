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
  double currentTime;
  string errorString="";
  bool mode = false, loss= false , duple = false, delay = false;
  vector<string> messeages;
  void readFile(string fileName);
  void extractErrorBytes(string message);
  void extractMessage(string message);
  string byteStuffing(string message);
  string addHeader(string message, int id, int type,double sending_time);
  string modeification(string message);
  cMessage * operations(string message,int id);
  int parityBit(string message);
  void makeSend(cMessage* msg);
  void updateTime(double delay);
  void reInit();
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
