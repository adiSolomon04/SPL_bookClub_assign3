package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.stomp.passiveObject.Pair_IdAndSubscribeId;
import bgu.spl.net.srv.BlockingConnectionHandler;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

import java.io.IOException;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;


public class ConnectionImpl implements Connections<String> {
    //private List<ConnectionHandlerImpl<T>> ConnectionHandlerList;
    private ConcurrentHashMap<Integer, ConnectionHandler<String>> ClientHashMap; //todo why non blocking??? or both???
    private ConcurrentHashMap<String,List<Pair_IdAndSubscribeId>> channelHashMap;// topic -> id & SubscribeId //todo: mabye move to DataBase
    private static AtomicInteger MessageId;

    public ConnectionImpl() {
        ClientHashMap = new ConcurrentHashMap<>();
        channelHashMap = new ConcurrentHashMap<>();
        MessageId = new AtomicInteger(1);
    }


    @Override
    public boolean send(int connectionId, String msg) {
        if(!ClientHashMap.containsKey(connectionId))
            return false;
        ClientHashMap.get(connectionId).send(msg);
        return true;
    }

    @Override
    public void send(String channel, String msg) {
        int tempMessageId;
            tempMessageId = MessageId.getAndAdd(1);
        if (channelHashMap.containsKey(channel)) {
            for (Pair_IdAndSubscribeId idAndSubscribeId : channelHashMap.get(channel))
                //DEBUGGING: check for msg \n in the end!!
                ClientHashMap.get(idAndSubscribeId.getId()).send("MESSAGE\nsubscription:" + idAndSubscribeId.getSubscribeId() + "\nMessage-id:" + tempMessageId + "\ndestination:" + channel + "\n\n" + msg);
        }
    }

    @Override
    public void disconnect(int connectionId) {
            ClientHashMap.remove(connectionId).terminateProtocol();
            //close();
    }

    @Override
    public void addCleint(int connectionId, ConnectionHandler<String> handler) {
        ClientHashMap.put(connectionId,handler);
    }

    public void subscribe(String destination, int id, int subscribeId) {
        channelHashMap.putIfAbsent(destination, new LinkedList<>()); //adi-16.1 debugging
        channelHashMap.get(destination).add(new Pair_IdAndSubscribeId(id,subscribeId));
    }

    public void unsubscribe(int ownerId, Integer UnSubscribeId, String topic) {
        channelHashMap.get(topic).remove(new Pair_IdAndSubscribeId(ownerId,UnSubscribeId));//I add equals to Pair_IdAndSubscribeId in order to do the search in list
        //find the (id,unSubscribeId) from list and remove them
    }

}
