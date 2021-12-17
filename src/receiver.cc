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

#include <bits/stdc++.h>
#include "receiver.h"
#include "MessageM_m.h"
using namespace omnetpp;
using namespace std;

Define_Module(Receiver);

bool Receiver::getParityByte(MessageM_Base * message){
    string payload = message->getPayload();
    bitset<8> b(payload[0]);
    for (int i = 1; i < payload.size(); i++) {
        bitset<8> a(payload[i]);
        b ^= a;
    }
    return b == message->getTrailer();
}

void Receiver::logEvent(MessageM_Base* message,double time,string type) {
    std::ofstream out;
    out.open(par("output_file").stringValue(),std::ios_base::app);
    if(type == "receive"){
        out<<"- Receiver received message with id="<<message->getId()
                  <<" and content "<<message->getPayload()
                  <<" at "<<time<<endl;
    } else if(type == "ack"){
        out<<"- Receiver sent ACK at " <<time<< endl;
    } else if(type == "nack"){
        out<<"- Receiver sent NACK at " <<time<< endl;
    } else if(type == "discard"){
        out<<"- Receiver received and discarded unexpected message at "<<time<<endl;
    }
    out.close();
}

void Receiver::initialize(){
    this->expectedMessageId = 0;
}

void Receiver::handleMessage(cMessage *msg) {
    MessageM_Base *mmsg = check_and_cast<MessageM_Base *>(msg);
    if(mmsg->getId() == this->expectedMessageId){
        this->logEvent(mmsg, simTime().dbl(), "receive");
        if(this->getParityByte(mmsg)){
            // ACK
            MessageM_Base* ack = new MessageM_Base();
            ack->setId(1-this->expectedMessageId);
            ack->setType(1);
            this->logEvent(nullptr, simTime().dbl(), "ack");
            this->expectedMessageId = 1-this->expectedMessageId;
            sendDelayed(ack, this->sendingDelay, "out");
        } else {
            // NACK
            MessageM_Base* nack = new MessageM_Base();
            nack->setId(this->expectedMessageId);
            nack->setType(2);
            this->logEvent(nullptr, simTime().dbl(), "nack");
            sendDelayed(nack, this->sendingDelay, "out");
        }
    } else {
        this->logEvent(nullptr, simTime().dbl(), "discard");
    }
}
