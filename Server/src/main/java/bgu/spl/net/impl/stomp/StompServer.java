package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.newsfeed.NewsFeed;
import bgu.spl.net.impl.rci.ObjectEncoderDecoder;
import bgu.spl.net.impl.rci.RemoteCommandInvocationProtocol;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {

        if (args[1].equals("tpc")) {
//         you can use any server...
            Server.threadPerClient(
                    Integer.parseInt(args[0]), //port
                    StompMessagingProtocolimpl::new, //protocol factory
                    StompMessageEncoderDecoder::new //message encoder decoder factory
            ).serve();
        } else if (args[1].equals("reactor")) {
            Server.reactor(
                    Runtime.getRuntime().availableProcessors(),
                    Integer.parseInt(args[0]), //port
                    StompMessagingProtocolimpl::new, //protocol factory
                    StompMessageEncoderDecoder::new //message encoder decoder factory
            ).serve();

        }
    }
}
