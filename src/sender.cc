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

#include "sender.h"
#include <bits/stdc++.h>
#include<stdio.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <math.h>
#include "MessageM_m.h"
Define_Module(Sender);
using namespace std;

void Sender::extractErrorBytes(MessageM_Base * message){
    string temp = message->getPayload();
    this->errorString = "";
    temp = temp.substr(0, 4);
    if (temp[0] == '1' ){
        message->setMode(1);
        this->errorString.append("Modification ");
    }
    if (temp[1]== '1'){
        message->setLoss(1);
        this->errorString.append("loss ");
    }
    if (temp[2] == '1'){
        message->setDuple(1);
        this->errorString.append("Duplication ");
    }
    if (temp[3] == '1'){
        message->setDelay(1);
        this->errorString.append("delay ");
    }
}

void Sender::addHeader(MessageM_Base * message, int id, int type, double sending_time){
    // need to fix as receiver can extract header in all cases
    message->setId(id);
    message->setType(type);
    message->setSendingTime(sending_time);
}

void Sender::modeification(MessageM_Base* message){
    string payload = message->getPayload();
    int count = payload.size();
    int ind = rand()%(count+1);
    bitset<8> b(payload[ind]);
    int ind2 = rand()%(9);
    b[ind2-1].flip();
    payload[ind]= char(b.to_ullong());
    message->setPayload(payload.c_str());
}

void Sender::parityBit(MessageM_Base * message){
    string payload = message->getPayload();
    bitset<8> b(payload[0]);
    for (int i = 1; i < payload.size(); i++) {
        bitset<8> a(payload[i]);
        b ^= a;
    }
    message->setTrailer(b);
}

void Sender::logEvent(MessageM_Base* message,double time,string type) {
    std::ofstream out;
    out.open(par("output_file").stringValue(),std::ios_base::app);
    if(type == "send"){
        this->numberOfTransmissions++,this->numberOfDataTransmissions++;
        out<<"- Sender sent message with id="<<message->getId()
           <<" and content "<<message->getPayload()
           <<" at "<<time<<" it has "<<this->errorString<<endl;
    } else if(type == "ack"){
        this->numberOfTransmissions++;
        out<<"- Sender received ACK at time " <<time<< endl;
    } else if(type == "nack"){
        this->numberOfTransmissions++;
        out<<"- Sender received NACK at time " <<time<< endl;
    } else if(type == "timeout"){
        out<<"- Sender got timeout"<<endl;
    } else if(type == "resend"){
        this->numberOfDataTransmissions++;
        out<<"- Sender re-sends correct message with id="<<message->getId()
           <<" and content "<<message->getPayload()
           <<" at "<<time<<endl;
    } else if(type == "stats"){
        out<<"- .............................."<<endl
        <<"- total transmission time="<<simTime()<<endl
        <<"- total number of transmissions="<<this->numberOfTransmissions<<endl
        <<"- the network throughput="<<(this->numberOfDataTransmissions/simTime().dbl())<<endl;
    }
    out.close();
}

void Sender::makeSend(MessageM_Base * msg){
    if(msg->getMode()){
         this->modeification(msg);
    }
    EV<<msg->getPayload()<<endl;
    MessageM_Base * dupMsg = msg->dup();
    if(!msg->getLoss()){
         if(msg->getDuple()){
             if(msg->getDelay()){
                 this->logEvent(msg,simTime().dbl()+par("delay_time").doubleValue(),"send");
                 sendDelayed(msg, par("delay_time").doubleValue(), "out");
                 this->logEvent(msg,simTime().dbl()+0.01+par("delay_time").doubleValue(),"send");
                 sendDelayed(dupMsg, 0.01+par("delay_time").doubleValue(), "out");
             }
             else{
                 this->logEvent(msg,simTime().dbl(), "send");
                 send(msg, "out");
                 this->logEvent(dupMsg,simTime().dbl()+0.01, "send");
                 sendDelayed(dupMsg, 0.01, "out");
             }
         }
         else{
             if(msg->getDelay()){
                 this->logEvent(msg,simTime().dbl()+par("delay_time").doubleValue(),"send");
                 sendDelayed(msg,par("delay_time").doubleValue(), "out");
             }
             else{
                 this->logEvent(msg,simTime().dbl(), "send");
                 send(msg,"out");
             }
         }
    }
}

void Sender::byteStuffing (MessageM_Base* message) {
    string temp = message->getPayload();
    temp.insert(0, "$");
    for (int i  = 1; i  < temp.size(); i++ ) {
       if (temp[i] == '$' || temp[i] == '/') {
           temp.insert(i, "/");
            i++;
       }
    }
    temp.append("$");
    message->setPayload(temp.c_str());
}

void Sender::readFile(string fileName){
    ifstream inputFile(fileName);
    string line ;
    while(getline(inputFile,line)){
        this->messeages.push_back(line);
    }
    inputFile.close();
}

MessageM_Base * Sender::operations(string message, int id){
    MessageM_Base * toSend = new MessageM_Base();
    toSend->setPayload(message.c_str());
    this->extractErrorBytes(toSend);
    string payload = toSend->getPayload();
    payload = payload.substr(5,message.size());
    toSend->setPayload(payload.c_str());
    this->byteStuffing(toSend);
    this->addHeader(toSend, id, 0, simTime().dbl());
    this->parityBit(toSend);
    return toSend;
}

void Sender::initialize()
{
    this->readFile(par("input_file"));
    this->timeoutChecker = new cMessage("have timeout");
    this->incrementalId = 0;
    this->numberOfTransmissions = 0;
    this->numberOfDataTransmissions = 0;
    scheduleAt(par("start_transmission_time"),new cMessage("transmit message"));
}

void Sender::handleMessage(cMessage *msg) {
    if(this->messeages.empty()) return;
    string current = this->messeages[0];
    MessageM_Base * toSend = operations(current,this->incrementalId);

    if(msg->isSelfMessage()){
        if(!strcmp(msg->getName(), "transmit message")){
             this->makeSend(toSend);
             scheduleAt(simTime().dbl()+par("timeout_interval").doubleValue(), this->timeoutChecker);
        } else if(!strcmp(msg->getName(), "have timeout")) {
             // timeout
             cancelEvent(this->timeoutChecker);
             this->logEvent(nullptr,simTime().dbl(),"timeout");
             send(toSend, "out");
             this->logEvent(toSend,simTime().dbl(),"resend");
             scheduleAt(simTime().dbl()+par("timeout_interval").doubleValue(), this->timeoutChecker);
        }
    } else {
        MessageM_Base *mmsg = check_and_cast<MessageM_Base *>(msg);
        cancelEvent(this->timeoutChecker);
        if(mmsg->getType()==1){
            // ACK
            this->logEvent(nullptr,simTime().dbl(),"ack");
            this->messeages.pop_front();
            this->incrementalId = 1 - this->incrementalId;
            scheduleAt(simTime().dbl(), new cMessage("transmit message"));
        } else {
            // NACK
            this->logEvent(nullptr,simTime().dbl(),"nack");
            send(toSend, "out");
            this->logEvent(toSend,simTime().dbl(),"resend");
            scheduleAt(simTime().dbl()+par("timeout_interval").doubleValue(), this->timeoutChecker);
        }
    }
}

void Sender::finish(){
    this->logEvent(nullptr, 0, "stats");
}
