package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.passiveObject.DataBaseSingleton;
import bgu.spl.net.impl.stomp.passiveObject.User;
import bgu.spl.net.srv.Connections;

import java.util.concurrent.ConcurrentHashMap;

public class StompMessagingProtocolimpl implements StompMessagingProtocol<String> {
    private int OwnerId;
    private String OwnerUsername;
    private Connections<String> connections;

    private ConcurrentHashMap<Integer, String> IdSubscribeToGenre; // idSubscribe-> GenreName//maby need to  save the topic subscribe?

    public void setTerminate(boolean terminate) {
        this.terminate = terminate;
    }

    private boolean terminate = false;


    private boolean isLoggedIn = false;

    @Override
    public void start(int connectionId, Connections<String> connections) {
        OwnerId = connectionId;
        this.connections = connections;
        IdSubscribeToGenre = new ConcurrentHashMap<>();

    }

    @Override
    public void process(String message) {
        String StompCommand = message.substring(0, message.indexOf("\n"));
        String messageBody="";
        if(message!="") {
             messageBody = message.substring(message.indexOf("\n") + 1);
        }
        switch (StompCommand) {
            case "CONNECT": {
                this.Connect(messageBody);
                break;
            }
            //message, RECEIPT, ERROR from server to client??
            case "SEND": {
                String dest = messageBody.substring(messageBody.indexOf("destination:"));
                dest= dest.substring(dest.indexOf(":") + 1, dest.indexOf("\n"));
                String body = messageBody.substring(messageBody.indexOf("\n\n") + 2);
                //sends a message to a destination - a topic
                this.send(dest, body);
                break;
            }
            case "SUBSCRIBE": {
                String dest = messageBody.substring(messageBody.indexOf("destination:"));
                dest= dest.substring(dest.indexOf(":") + 1, dest.indexOf("\n"));
                String id = messageBody.substring(messageBody.indexOf("id:"));
                id= id.substring(id.indexOf(":") + 1, id.indexOf("\n"));
                String receipt = messageBody.substring(messageBody.indexOf("receipt:"));
                receipt= receipt.substring(receipt.indexOf(":") + 1, receipt.indexOf("\n"));
                this.subscribe(dest, Integer.parseInt(id), receipt);
                //Subscribe to topic for client id
                break;
            }
            case "UNSUBSCRIBE": {
                //String dest = messageBody.substring(messageBody.indexOf("destination:"));
                //dest.substring(dest.indexOf(":") + 1, dest.indexOf("\n"));
                String id = messageBody.substring(messageBody.indexOf("id:"));
                id= id.substring(id.indexOf(":") + 1, id.indexOf("\n"));
                String receipt = messageBody.substring(messageBody.indexOf("receipt:"));
                receipt= receipt.substring(receipt.indexOf(":") + 1, receipt.indexOf("\n"));
                this.unsubscribe(Integer.parseInt(id), receipt);
                //Will unsubscribe from a topic
                break;
            }
            case "DISCONNECT": {
                String receipt = messageBody.substring(messageBody.indexOf("receipt:"));
                receipt = receipt.substring(receipt.indexOf(":") + 1, receipt.indexOf("\n"));
                this.disconnect(OwnerId, OwnerUsername,receipt);
                //receipt can be added to any frame that needs response to the client
                break;
            }
        }
        //protocol.process(message)
        //process the message as needed in STOMP protocol

    }



    @Override
    public boolean shouldTerminate() {
        return terminate;
    }


    private void Connect(String message) {
        String[] split = message.split("\n");
        boolean success = true;
        String accept_version = null;
        String host;
        String passcode = "";
        if (split.length >= 4) {
            if (split[0].indexOf("accept-version:") == 0) {// what to do with this shit
                accept_version = split[0].substring(split[0].indexOf(":")+1);
            } else {//error
            }
            if (split[1].indexOf("host:") == 0) {
                host = split[1].substring(split[1].indexOf(":")+1);
            } else {//error
            }
            if (split[2].indexOf("login:") == 0) {
                OwnerUsername = split[2].substring(split[2].indexOf(":")+1);
            } else {//error
            }
            if (split[3].indexOf("passcode:") == 0) {
                passcode = split[3].substring(split[3].indexOf(":")+1);
            } else {//error
            }
        } else {//error
        }
        //check body???
        //send connected
        isLoggedIn = connect(OwnerUsername, passcode, accept_version);
        terminate = !isLoggedIn;
    }

    private boolean connect(String OwnerUsername, String passcode, String accept_version) {
        //TODO: synchronize
        //TODO: when ERROR ", it MUST then close the connection "! should close the socket?
        DataBaseSingleton dataBaseSingleton = DataBaseSingleton.getSingleton();
        synchronized (dataBaseSingleton) {
            if (dataBaseSingleton.userExist(OwnerUsername)) {
                User user = dataBaseSingleton.getUser(OwnerUsername);
                if (user.isConnected()) {
                    //send “User already logged in”.
                    connections.send(OwnerId, "ERROR\n" + "message: User already logged in" + "\n\n\u0000");
                    return false;
                } else {
                    if (!user.getPasscode().equals(passcode)) {
                        //send “Wrong password”.
                        connections.send(OwnerId, "ERROR\n" + "message: Wrong password" + "\n\n\u0000");
                        return false;
                    } else {
                        //CONNECTED
                        connections.send(OwnerId, "CONNECTED\n" + "version:" + accept_version + "\n\n\u0000");
                        user.setConnected(true);
                        // send "Login successful.”
                        return true;
                    }
                }
            } else {
                dataBaseSingleton.addNewUser(OwnerUsername, new User(OwnerUsername, passcode, true, OwnerId));
                //CONNECTED
                connections.send(OwnerId, "CONNECTED\n" + "version:" + accept_version + "\n\n\u0000");
                //send "Login successful”
                return true;
            }
        }
    }

    private void subscribe(String destination, Integer SubscribeId, String receipt) {

        ((ConnectionImpl) connections).subscribe(destination, OwnerId, SubscribeId);
        IdSubscribeToGenre.put(SubscribeId, destination);
        connections.send(OwnerId, "RECEIPT\nreceipt-id:" + receipt + "\n\n\u0000");
    }

    private void unsubscribe(Integer UnSubscribeId, String receiptId) {
        String topic = IdSubscribeToGenre.remove(UnSubscribeId);
        ((ConnectionImpl)connections).unsubscribe(OwnerId,UnSubscribeId,topic);
        connections.send(OwnerId, "RECEIPT\nreceipt-id:" + receiptId + "\n\n\u0000");

    }
    private void disconnect(int ownerId, String ownerUsername, String receipt) {
        for (Integer idSubscribe : IdSubscribeToGenre.keySet()) {
            String topic = IdSubscribeToGenre.get(idSubscribe);
            ((ConnectionImpl)connections).unsubscribe(OwnerId,idSubscribe,topic);
        }
        IdSubscribeToGenre.clear(); //can change get to remove? or doing problem in iterator?


        connections.send(OwnerId, "RECEIPT\nreceipt-id:" + receipt + "\n\n\u0000");
        DataBaseSingleton dataBaseSingleton = DataBaseSingleton.getSingleton();
        User user = dataBaseSingleton.getUser(OwnerUsername);
        user.setConnected(false);
        connections.disconnect(ownerId);

    }
    private void send(String dest, String body) {
        //maybe need to send all function
        connections.send(dest,body);
    }


}
