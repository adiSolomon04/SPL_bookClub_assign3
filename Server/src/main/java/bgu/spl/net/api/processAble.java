package bgu.spl.net.api;

import bgu.spl.net.srv.Connections;

public interface processAble {

    /**
     * process the given messageBody and send message back to client if needed
     * @param messageBody the received message body
     * @param connections the connections that send t
     * @return ture - if success in the processe , false if sended an ERROR msg
     */
    public boolean processeMessage(String messageBody, Connections<String> connections);

    //todo impliment if needed for StompMwssageProtocolimp
}
