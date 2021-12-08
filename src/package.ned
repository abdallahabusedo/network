package miniproject;

@license(LGPL);
//
// TODO documentation
//
network simplex
{
    @display("bgb=470,288");
    submodules:
        	sender: Sender {
            @display("p=137,134");
        }
        receiver: Receiver {
            @display("p=348,135");
        }
    connections:
        receiver.out --> sender.in;
        sender.out --> receiver.in;
}

