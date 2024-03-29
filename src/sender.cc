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
    string errorBits = message->getPayload();
    this->errorString = "";
    errorBits = errorBits.substr(0, 4);
    if (errorBits[0] == '1' ){
        message->setMode(1);
        this->errorString.append("Modification ");
    }
    if (errorBits[1]== '1'){
        message->setLoss(1);
        this->errorString.append("loss ");
    }
    if (errorBits[2] == '1'){
        message->setDuple(1);
        this->errorString.append("Duplication ");
    }
    if (errorBits[3] == '1'){
        message->setDelay(1);
        this->errorString.append("delay ");
    }
}

void Sender::addHeader(MessageM_Base * message, int id, int type, double sending_time){
    message->setId(id);
    message->setType(type);
    message->setSendingTime(sending_time);
}

void Sender::applyModeification(MessageM_Base* message){
    string payload = message->getPayload();
    int count = payload.size();
    int ind = rand()%(count);
    bitset<8> b(payload[ind]);
    int ind2 = rand()%(8);
    b[ind2].flip();
    payload[ind]= char(b.to_ullong());
    message->setPayload(payload.c_str());
}

void Sender::addTrailer(MessageM_Base * message){
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
        out<<"- Sender received NACK due to err or dup at time " <<time<< endl;
    } else if(type == "timeout"){
        out<<"- Sender got timeout at "<<time<<endl;
    } else if(type == "loss"){
        this->numberOfTransmissions++,this->numberOfDataTransmissions++;
        out<<"- Sender did not send message with id="<<message->getId()
           <<" and content "<<message->getPayload()<<" at time "<<time<<" due to loss"<<endl;
    } else if(type == "resend"){
        this->numberOfTransmissions++,this->numberOfDataTransmissions++;
        out<<"- Sender re-sends message with id="<<message->getId()
           <<" and content "<<message->getPayload()
           <<" at "<<time<<endl;
    } else if(type == "stats"){
        out<<"- .............................."<<endl
        <<"- total transmission time="<<simTime()-par("start_transmission_time").doubleValue()<<endl
        <<"- total number of transmissions="<<this->numberOfTransmissions<<endl
        <<"- the network throughput="<<(this->numberOfDataTransmissions/(simTime()-par("start_transmission_time").doubleValue()))<<endl;
    }
    out.close();
}

void Sender::applyErrorDelayAndSend(MessageM_Base * message){
    if(message->getMode()){
         this->applyModeification(message);
    }
    MessageM_Base * duplicatedMessage = message->dup();
    if(!message->getLoss()){
         if(message->getDuple()){
             if(message->getDelay()){
                 this->logEvent(message,simTime().dbl()+par("delay_time").doubleValue(),"send");
                 sendDelayed(message, par("delay_time").doubleValue(), "out");
                 this->logEvent(duplicatedMessage,simTime().dbl()+0.01+par("delay_time").doubleValue(),"send");
                 sendDelayed(duplicatedMessage, 0.01+par("delay_time").doubleValue(), "out");
             }
             else{
                 this->logEvent(message,simTime().dbl(), "send");
                 send(message, "out");
                 this->logEvent(duplicatedMessage,simTime().dbl()+0.01, "send");
                 sendDelayed(duplicatedMessage, 0.01, "out");
             }
         }
         else{
             if(message->getDelay()){
                 this->logEvent(message,simTime().dbl()+par("delay_time").doubleValue(),"send");
                 sendDelayed(message,par("delay_time").doubleValue(), "out");
             }
             else{
                 this->logEvent(message,simTime().dbl(), "send");
                 send(message,"out");
             }
         }
    } else {
        this->logEvent(message,simTime().dbl(), "loss");
    }
}

void Sender::byteStuffing (MessageM_Base* message) {
    string stuffedPayload = message->getPayload();
    stuffedPayload.insert(0, "$");
    for (int i  = 1; i  < stuffedPayload.size(); i++ ) {
       if (stuffedPayload[i] == '$' || stuffedPayload[i] == '/') {
           stuffedPayload.insert(i, "/");
            i++;
       }
    }
    stuffedPayload.append("$");
    message->setPayload(stuffedPayload.c_str());
}

void Sender::readFile(string fileName){
    ifstream inputFile(fileName);
    string line ;
    while(getline(inputFile,line)){
        this->plainMesseages.push_back(line);
    }
    inputFile.close();
}

MessageM_Base * Sender::prepareMessage(string message, int id){
    MessageM_Base * preparedMessage = new MessageM_Base();
    preparedMessage->setPayload(message.c_str());
    this->extractErrorBytes(preparedMessage);
    string payload = preparedMessage->getPayload();
    payload = payload.substr(5,message.size());
    preparedMessage->setPayload(payload.c_str());
    this->byteStuffing(preparedMessage);
    this->addHeader(preparedMessage, id, 0, simTime().dbl());
    this->addTrailer(preparedMessage);
    return preparedMessage;
}

void Sender::initialize()
{
    this->readFile(par("input_file"));
    this->timeoutChecker = new cMessage("have timeout");
    this->sequenceNumber = 0;
    this->numberOfTransmissions = 0;
    this->numberOfDataTransmissions = 0;
    scheduleAt(par("start_transmission_time"),new cMessage("transmit message"));
}

void Sender::handleMessage(cMessage *msg) {
    if(this->plainMesseages.empty()) return;
    string current = this->plainMesseages[0];
    MessageM_Base * nextMessageToSend = prepareMessage(current,this->sequenceNumber);

    if(msg->isSelfMessage()){
        if(!strcmp(msg->getName(), "transmit message")){
             this->applyErrorDelayAndSend(nextMessageToSend);
             scheduleAt(simTime().dbl()+par("timeout_interval").doubleValue(), this->timeoutChecker);
        } else if(!strcmp(msg->getName(), "have timeout")) {
             // timeout
             this->logEvent(nullptr,simTime().dbl(),"timeout");
             send(nextMessageToSend, "out");
             this->logEvent(nextMessageToSend,simTime().dbl(),"resend");
             scheduleAt(simTime().dbl()+par("timeout_interval").doubleValue(), this->timeoutChecker);
        }
    } else {
        MessageM_Base *receiverResponse = check_and_cast<MessageM_Base *>(msg);
        if(receiverResponse->getType()==1){
            // ACK
            cancelEvent(this->timeoutChecker);
            this->logEvent(nullptr,simTime().dbl(),"ack");
            this->plainMesseages.pop_front();
            this->sequenceNumber = 1 - this->sequenceNumber;
            scheduleAt(simTime().dbl(), new cMessage("transmit message"));
        } else {
            // NACK
            this->logEvent(nullptr,simTime().dbl(),"nack");
        }
    }
}

void Sender::finish(){
    this->logEvent(nullptr, 0, "stats");
}
