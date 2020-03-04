package bgu.spl.net.impl.stomp.passiveObject;

public class User {
    private String OwnerUsername;
    private String passcode;
    private boolean isConnected;
    private int OwnerId;



    public User(String ownerUsername, String passcode, boolean isConnected, int ownerId) {
        this.OwnerUsername = ownerUsername;
        this.passcode = passcode;
        this.isConnected = isConnected;
        this.OwnerId = ownerId;
    }

    public String getOwnerUsername() {
        return OwnerUsername;
    }

    public void setOwnerUsername(String ownerUsername) {
        OwnerUsername = ownerUsername;
    }

    public String getPasscode() {
        return passcode;
    }

    public void setPasscode(String passcode) {
        this.passcode = passcode;
    }

    public boolean isConnected() {
        return isConnected;
    }

    public void setConnected(boolean connected) {
        isConnected = connected;
    }

    public int getOwnerId() {
        return OwnerId;
    }

    public void setOwnerId(int ownerId) {
        OwnerId = ownerId;
    }
}
