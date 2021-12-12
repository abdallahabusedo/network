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

void Sender::makeSend(MessageM_Base * msg){
    MessageM_Base * dupMsg = msg->dup();
    if(!msg->getLoss()){
        EV<<"not loss\n";
         if(msg->getDuple()){
             if(msg->getDelay()){
                 EV<<"delay and duplicate\n";
//                 updateTime(par("delay_time").intValue()*1.0);
                 sendDelayed(msg, par("delay_time").intValue()*1.0, "out");
//                 updateTime(0.01+par("delay_time").intValue()*1.0);
                 sendDelayed(dupMsg, 0.01+par("delay_time").intValue()*1.0, "out");
             }
             else{
                 EV<<"duplicate \n";
                 send(msg, "out");
//                 updateTime(0.01);
                 sendDelayed(dupMsg, 0.01, "out");
             }
         }
         else{
             if(msg->getDelay()){
                 EV<<"delay \n";
//                 updateTime(par("delay_time").intValue()*1.0);
                 sendDelayed(msg,par("delay_time").intValue()*1.0, "out");
             }
             else{
                 EV<<"normal \n";
                 send(msg,"out");
             }
         }
    }
}

void Sender::byteStuffing (MessageM_Base* message){
    string temp = message->getPayload();
    for (int i  = 0; i  < temp.size(); i++ ) {
       if (temp[i]== '$'){
           temp.insert(i, "/");
            i++;
        }
    }
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

void Sender::initialize()
{
    this->readFile(par("input_file"));
    scheduleAt(par("start_transmission_time").intValue(),new cMessage("Start Transition"));

    // TODO - Generated method body
//    this->currentTime = par("start_transmission_time").intValue()*1.0;
//    std::ofstream outputFile;
//    string outputFileName = par("output_file");
//    outputFile.open(outputFileName,std::ios_base::app);
//    for (int i = 0; i < messeages.size(); ++i) {
//        cMessage * msg = this->operations(this->messeages[i], i);
//        EV << "message number " << i << " ,current Time :" << this->currentTime<<endl;
//        outputFile<<"- Sender sends message with type=0, id="<<i<<" and content="<<msg->getName()<<" at "<<simTime()<<" with "<<this->errorString<<"\n";
//        makeSend(msg);
//        reInit();
//        EV << "----------------------" <<endl ;
//    }
}

void Sender::handleMessage(cMessage *msg)
{

    if(msg->isSelfMessage()){
     if(!strcmp(msg->getName(), "Start Transition")){
         for (int i = 0; i < this->messeages.size(); ++i){
             MessageM_Base * toSend = new MessageM_Base();
             toSend->setPayload(this->messeages[i].c_str());
             this->extractErrorBytes(toSend);
             string payload = toSend->getPayload();
             payload = payload.substr(5,this->messeages[i].size());
             toSend->setPayload(payload.c_str());
             this->byteStuffing(toSend);
             if(toSend->getMode()){
                 this->modeification(toSend);
             }
             this->addHeader(toSend, i, 0, simTime().dbl());
             this->parityBit(toSend);
             this->makeSend(toSend);
         }
     }else{
         // timeout
     }
    }
    else{
        // ACK/NACK
    }
}
