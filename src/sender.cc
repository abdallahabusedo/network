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
Define_Module(Sender);
using namespace std;

void Sender::extractErrorBytes(string message){
    string temp = message .substr(0,4);
    if (temp[0] == '1' ){
        this->mode = true;
    }
    if (temp[1]== '1'){
        this->loss = true;
    }
    if (temp[2] == '1'){
        this->duple = true;
    }
    if (temp[3] == '1'){
        this->delay = true;
    }
}

void Sender::readFile(string fileName){
    ifstream inputFile(fileName);
    string line ;
    while(getline(inputFile,line)){
        this->messeages.push_back(line);
    }
    inputFile.close();
}

string Sender::byteStuffing (string message){
    for (int i  = 0; i  < message.size(); i++ ) {
       if (message[i]== '$'){
           message.insert(i, "/");
            i++;
        }
    }
    return message;
}

string Sender::addHeader(string message, int id, int type,int sending_time){
    // need to fix as receiver can extract header in all cases
    message.insert(0,",");
    message.insert(0,to_string(type));
    message.insert(0,",");
    message.insert(0,to_string(sending_time));
    message.insert(0,",");
    message.insert(0,to_string(id));
    return message;
}

string Sender::modeification(string message){
    int count = message.size();
    int ind = rand()%(count+1);
    bitset<8> b(message[ind]);
    int ind2 = rand()%(9);
    b[ind2-1].flip();
    message[ind]= char(b.to_ullong());
    return message;
}

int Sender::parityBit(string message){
    bitset<8> b(message[0]);
    for (int i = 1; i < message.size(); i++) {
        bitset<8> a(message[i]);
        b = b ^ a;
    }
    int sum = 0;
    for (int i = 0; i < 8; ++i) {
        sum+=b[i];
    }
    if(sum%2==0)
        return 0;
    else
        return 1;
}

cMessage * Sender::operations(string message ,int id ){
    this->extractErrorBytes(message);
    string payload = message.substr(5,message.size());
    string byteStuffed = this->byteStuffing(payload);
    string messageToSend = byteStuffed;
    if(this->mode){
      messageToSend = this->modeification(byteStuffed);
      EV<<"modeif done\n";
    }
    messageToSend = this->addHeader(messageToSend, id, 0, par("start_transmission_time").intValue());
    int i = parityBit(messeages[0]);
    messageToSend.append(to_string(i));

    return new cMessage(messageToSend.c_str());

}

void Sender::makeSend(cMessage* msg){
    cMessage *dupMsg = msg->dup();
    if(!this->loss){
        EV<<"not loss\n";
         if(this->duple){
             if(this->delay){
                 EV<<"d d\n";
                 sendDelayed(msg, par("delay_time").intValue(), "out");
                 sendDelayed(dupMsg, 0.01+par("delay_time").intValue(), "out");
             }
             else{
                 EV<<"dup \n";
                 send(msg, "out");
                 sendDelayed(dupMsg, simTime() + 0.01, "out");
             }
         }
         else{
             if(this->delay){
                 EV<<"del \n";
                 sendDelayed(msg, par("delay_time").intValue(), "out");
             }
             else{
                 EV<<"norm\n";
                 send(msg,"out");
             }
         }
    }
}

void Sender::initialize()
{
    // TODO - Generated method body
    this->currentTime = par("start_transmission_time").intValue();
    this->readFile(par("input_file"));
    cMessage * msg = this->operations(this->messeages[0], 0);
    scheduleAt(simTime(), msg);
}


void Sender::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(msg->isSelfMessage()){
        EV << "Received message from myself after delayed"<< endl;
        makeSend(msg);
    } else {
        // ACK and NACK
    }
}
