package bgu.spl.net.impl.stomp.passiveObject;

import java.util.Objects;

public class Pair_IdAndSubscribeId {
    private int Id;
    private int SubscribeId;

    public Pair_IdAndSubscribeId(int id, int subscribeId) {
        Id = id;
        SubscribeId = subscribeId;
    }

    public int getId() {
        return Id;
    }

    public int getSubscribeId() {
        return SubscribeId;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Pair_IdAndSubscribeId that = (Pair_IdAndSubscribeId) o;
        return Id == that.Id &&
                SubscribeId == that.SubscribeId;
    }

    @Override
    public int hashCode() {
        return Objects.hash(Id, SubscribeId);
    }

}
