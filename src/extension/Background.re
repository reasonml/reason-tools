open Core;

module Refmt = {
  external refmt : string => array string = "refmt" [@@bs.module "../../../../_build/refmt/src/app.js"];
};

type request = {
  in_: string
};

type response = {
  out: array string
};

Chrome.Runtime.addMessageListener
	(fun request _ respond =>
     	respond { out: (Refmt.refmt request.in_) });
