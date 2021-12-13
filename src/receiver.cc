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

void Receiver::initialize()
{
    // TODO - Generated method body
    this->expectedMessageId = 0;
}

void Receiver::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    // cast to MessageM
    std::ofstream outputFile;
     string outputFileName = par("output_file");
     outputFile.open(outputFileName,std::ios_base::app);
    MessageM_Base *mmsg = check_and_cast<MessageM_Base *>(msg);
    if(mmsg->getId() == this->expectedMessageId){
        outputFile<<"- Receiver received message with type="<<mmsg->getType()
                  <<", id="<<mmsg->getId()<<" and content="<<mmsg->getPayload()
                  <<" at "<<simTime().dbl()<<endl;
        if(this->getParityByte(mmsg)){
            MessageM_Base* ack = new MessageM_Base();
            ack->setId(1-this->expectedMessageId);
            ack->setType(1);
            ack->setSendingTime(simTime().dbl());
            outputFile<<"- Receiver sends message with type="<<ack->getType()
                  <<", id="<<ack->getId()<<" and content="<<ack->getPayload()
                  <<" at "<<simTime().dbl()<<endl;
            sendDelayed(ack, simTime().dbl()+this->sendingDelay, "out");
            this->expectedMessageId = 1- this->expectedMessageId;
        } else {
            MessageM_Base* nack = new MessageM_Base();
            nack->setId(this->expectedMessageId);
            nack->setType(2);
            nack->setSendingTime(simTime().dbl());
            outputFile<<"- Receiver sends message with type="<<nack->getType()
                  <<", id="<<nack->getId()<<" and content="<<nack->getPayload()
                  <<" at "<<simTime().dbl()<<endl;
            sendDelayed(nack, simTime().dbl()+this->sendingDelay, "out");
        }
    }
    outputFile.close();
}
