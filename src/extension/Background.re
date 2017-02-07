open Core;
open RefmtProtocol;

Chrome.Runtime.addMessageListener
	(fun request _ respond =>
    respond { output: (Refmt.refmt request.input) });
