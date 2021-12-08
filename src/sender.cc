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

cMessage * extractFromMessage(string message,int start , int end ){
    string temp = message .substr(start,end);
    return new cMessage(temp.c_str());
}

void Sender::readFile(string fileName){
    ifstream inputFile(fileName);
        string line ;
        while(getline(inputFile,line)){
            this->messeages.push_back(line);
        }
        inputFile.close();
}

cMessage * Sender::byteStuffing (string message){
    for (int i  = 0; i  < message.size(); i++ ) {
           if (message[i]== '$'){
               message.insert(i, "/");
                i++;
            }
    }
    return new cMessage(message.c_str());
}

cMessage * Sender::addHeader(string message, int id, int type,int sending_time){
    message.insert(0,to_string(type));
    message.insert(0,to_string(sending_time));
    message.insert(0,to_string(id));
    return new cMessage(message.c_str());
}

cMessage * Sender::modeification(string message){
    int count = message.size();
    int ind = rand()%(count+1);
    bitset<8> b(message[ind]);
//    int ind2 = rand()%(9);
    int ind2 = 1;
    EV  << char(b.to_ullong())<<" "<<b.to_ullong()<< endl;
    b[ind2-1].flip();
    message[ind]= char(b.to_ullong());
    EV  << char(b.to_ullong())<<" "<<b.to_ullong() <<" " <<ind<< endl;
    return new cMessage(message.c_str());

}

void Sender::initialize()
{
    // TODO - Generated method body
//    string x = "1010 A flower, sometimes known as a bloom or blossom";
//    string errorbytes = x.substr(0,4);
//    cMessage * msg = new cMessage(errorbytes.c_str());
      this->currentTime = par("start_transmission_time").intValue();
      this->readFile(par("input_file"));
//      cMessage * msg = extractFromMessage(this->messeages[0],5,this->messeages[0].size());
      cMessage * msg = modeification(this->messeages[0]);
      send(msg,"out");
}

void Sender::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    send(msg ,"out");
    EV << "Resending message"<< endl;

}
