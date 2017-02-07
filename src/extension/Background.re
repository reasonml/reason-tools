open Core;
open RefmtProtocol;

Chrome.Runtime.addMessageListener
	(fun request _ respond =>
    respond { out: (Refmt.refmt request.in_) });
