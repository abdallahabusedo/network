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
        }else if (temp[1]== '1'){
            this->duple = true;
        }else if (temp[2] == '1'){
            this->delay = true;
        }else if (temp[3] == '1'){
            this->loss = true;
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
    message.insert(0,to_string(type));
    message.insert(0,to_string(sending_time));
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
        for (int j = 0; j < 8; ++j)
            b[j] = b[j] ^ a[j];
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
    string byteStuffed = this->byteStuffing(message);
    string messageToSend = this->addHeader(byteStuffed, id, 0, par("start_transmission_time"));
    this->extractErrorBytes(message);
    int i = parityBit(messeages[0]);
    messageToSend.append(to_string(i));
    if(this->mode){
      messageToSend = this->modeification(messageToSend);
    }

    return new cMessage(messageToSend.c_str());

}

void Sender::initialize()
{
    // TODO - Generated method body
    this->currentTime = par("start_transmission_time").intValue();
    this->readFile(par("input_file"));
    cMessage * msg = operations(this->messeages[0], 0);
    send(msg,"out");

}

void Sender::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    send(msg ,"out");
    EV << "Resending message"<< endl;

}
