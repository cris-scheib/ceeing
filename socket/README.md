## Socket

<p>This project is a Client/Server paradigm for monitor the temperature of various equipment in a factory/industry. The purpose of it is to maintain communication between the devices and the server. The server stays monitoring (receiving information) each device and printing the information on the screen received. 
</p>
<p>Clients must send three pieces of information to the server every 5 seconds:</p>

- ID: A unique identification number (one per device, cannot be repeated);
- Sequence: Message sequencing number;
- Value: Equipment temperature value.
