package bgu.spl.net.impl.stomp.passiveObject;

import java.util.concurrent.ConcurrentHashMap;

public class DataBaseSingleton {
    static private DataBaseSingleton singleton = new DataBaseSingleton();
    private ConcurrentHashMap<String, User> usersHashMap = new ConcurrentHashMap();; //userName -> User

    public static DataBaseSingleton getSingleton() {
        return singleton;
    }

    public ConcurrentHashMap<String, User> getUsersHashMap() {
        return usersHashMap;
    }

    public boolean userExist(String OwnerUsername){
        return usersHashMap.containsKey(OwnerUsername);
    }
    public User getUser(String OwnerUsername){
        return usersHashMap.get(OwnerUsername);
    }
    public void addNewUser(String ownerUsername, User user) {
        usersHashMap.put(ownerUsername,user);
    }

}
